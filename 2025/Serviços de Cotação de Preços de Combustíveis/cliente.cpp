/**
 * @file Cliente.cpp
 *
 * Implementação do cliente. O cliente envia ao servidor Dados ou realiza uma Pesquisa
 *
 * @author Sizenando S. França | RGM: 50575
 */

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <sstream>

using namespace std;

// Pacote para ser enviado ao servidor
struct Pacote
{
    char tipo_msg; // D ou P
    int id_msg;    // Identificador fornecido pelo usuário
    bool erro;     // Se o pacote possui erro ou não

    // Pacote pode ser do tipo Dados ou Pesquisa ao mesmo tempo
    union
    {
        struct
        {
            int tipo_combustivel; // 0 (diesel), 1 (alcool), 2 (gasolina)
            int preco;
            double latitude;
            double longitude;
        } dados;
        struct
        {
            int tipo_combustivel; // 0 (diesel), 1 (alcool), 2 (gasolina)
            int raio_busca;       // Raio de busca em Km
            double latitude_centro;
            double longitude_centro;
        } pesquisa;
    } payload;
};

// Resposta de ACK/NAK que será recebida do servidor
struct Resposta
{
    int id_msg_original;
    bool is_nak; // true se for NAK e false se for ACK
};

// Resposta com menor valor de busca pelo servidor
struct RespostaPesquisa
{
    int preco_encontrado; // -1 se não encontrou, -2 para NAK
};

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cerr << "Uso: " << argv[0] << " <ip/nome> <porta>" << endl;
        return 1;
    }

    srand(time(NULL)); // Inicia aleatoriazação de valores
    // Cria o socket do cliente. AF_INET = IPv4, SOCK_DGRAM = UDP
    int cliente_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (cliente_socket == -1)
    {
        cerr << "Erro ao criar o socket do cliente!" << endl;
        return 1;
    }

    // Para cálculo de timeout caso o pacote demore demais para chegar ao servidor
    struct timeval timeout;
    timeout.tv_sec = 2; // Definido tempo máximo de espera em 2 segundos
    timeout.tv_usec = 0;
    // Configura o tempo de timeout no socket criado
    setsockopt(cliente_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Configura o servidor a qual será conectado
    sockaddr_in servidor_endereco;
    servidor_endereco.sin_family = AF_INET;            // IPv4
    servidor_endereco.sin_port = htons(atoi(argv[2])); // Porta convertida para formato correto

    if (inet_pton(AF_INET, argv[1], &servidor_endereco.sin_addr) <= 0)
    {
        cerr << "Endereço IP inválido ou não suportado" << endl;
        return 1;
    }
    cout << "-----------------------------------------------------------------------" << endl;
    cout << "Padrão de entrada para tipo D e tipo P" << endl;
    cout << "> <'D'> <ID> <(0,2)> <preco> <latitude> <longitude>" << endl;
    cout << "> <'P'> <ID> <(0,2)> <raio_busca> <latitude_centro> <longitude_centro>" << endl;
    cout << "Escreva <exit> para sair" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    string input;
    while (cout << "> ", getline(cin, input) && input != "exit")
    {
        stringstream linha(input);
        Pacote pacote;
        string tipo_str;

        linha >> tipo_str; // Primeira palavra da string lida é o tipo (D ou P)
        if (tipo_str.empty())
            continue;

        pacote.tipo_msg = tipo_str[0];
        pacote.erro = ((double)rand() / RAND_MAX) > 0.5; // Realiza a randomização do erro a ser gerado

        // Preenche pacote com cada palavra da linha lida
        if (pacote.tipo_msg == 'D')
        {
            linha >> pacote.id_msg >> pacote.payload.dados.tipo_combustivel >> pacote.payload.dados.preco >> pacote.payload.dados.latitude >> pacote.payload.dados.longitude;
        }
        else if (pacote.tipo_msg == 'P')
        {
            linha >> pacote.id_msg >> pacote.payload.pesquisa.tipo_combustivel >> pacote.payload.pesquisa.raio_busca >> pacote.payload.pesquisa.latitude_centro >> pacote.payload.pesquisa.longitude_centro;
        }
        else
        {
            cerr << "Tipo de mensagem inválido. Use 'D' ou 'P'." << endl;
            continue;
        }

        bool sucesso = false;
        while (!sucesso)
        {
            // Envia o pacote para o servidor
            cout << "Enviando pacote " << pacote.id_msg << " (erro=" << (pacote.erro ? "sim" : "não") << ")..." << endl;
            sendto(cliente_socket, &pacote, sizeof(Pacote), 0, (struct sockaddr *)&servidor_endereco, sizeof(servidor_endereco));

            if (pacote.tipo_msg == 'D')
            {
                Resposta resposta_d;
                ssize_t bytes = recvfrom(cliente_socket, &resposta_d, sizeof(Resposta), 0, NULL, NULL);

                if (bytes < 0)
                {
                    cout << "Timeout! Retransmitindo..." << endl;
                    pacote.erro = false;
                }
                else if (resposta_d.id_msg_original == pacote.id_msg)
                {
                    if (resposta_d.is_nak)
                    {
                        cout << "NAK recebido. Retransmitindo..." << endl;
                        pacote.erro = false;
                    }
                    else
                    {
                        cout << "ACK recebido! Sucesso!" << endl;
                        sucesso = true;
                    }
                }
            }
            else if (pacote.tipo_msg == 'P')
            {
                RespostaPesquisa resposta_p;
                ssize_t bytes = recvfrom(cliente_socket, &resposta_p, sizeof(RespostaPesquisa), 0, NULL, NULL);

                if (bytes < 0)
                {
                    cout << "Timeout! Retransmitindo..." << endl;
                    pacote.erro = false;
                }
                else
                {
                    if (resposta_p.preco_encontrado == -2)
                    {
                        cout << "NAK recebido para pesquisa. Retransmitindo..." << endl;
                        pacote.erro = false;
                    }
                    else
                    {
                        if (resposta_p.preco_encontrado == -1)
                        {
                            cout << "Resposta do servidor: Nenhum posto encontrado." << endl;
                        }
                        else
                        {
                            cout << "Resposta do servidor: Menor preço encontrado: " << resposta_p.preco_encontrado << endl;
                        }
                        sucesso = true;
                    }
                }
            }
        }
    }
    close(cliente_socket);

    return 0;
}
