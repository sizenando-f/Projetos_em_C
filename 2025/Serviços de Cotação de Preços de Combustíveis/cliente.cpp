#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <sstream>

using namespace std;

// As structs permanecem as mesmas
struct Pacote
{
    char tipo_msg; // D ou P
    int id_msg;
    bool erro;

    union
    {
        struct
        {
            int tipo_combustivel;
            int preco;
            double latitude;
            double longitude;
        } dados;
        struct
        {
            int tipo_combustivel;
            int raio_busca;
            double latitude_centro;
            double longitude_centro;
        } pesquisa;
    } payload;
};

struct Resposta
{
    int id_msg_original;
    bool is_nak; // true se for NAK e false se for ACK
};

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

    srand(time(NULL));
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1)
    {
        cerr << "Erro ao criar o socket do cliente!" << endl;
        return 1;
    }

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        cerr << "Endereço IP inválido ou não suportado" << endl;
        return 1;
    }

    cout << "Escreva <exit> para sair" << endl;
    string input;
    while (cout << "> ", getline(cin, input) && input != "exit")
    {
        stringstream linha(input);
        Pacote pacote;
        string tipo_str;

        linha >> tipo_str;
        if (tipo_str.empty())
            continue;

        pacote.tipo_msg = tipo_str[0];
        pacote.id_msg = rand() % 1000; // Aumentado para diminuir colisões
        pacote.erro = ((double)rand() / RAND_MAX) > 0.5;

        if (pacote.tipo_msg == 'D')
        {
            linha >> pacote.payload.dados.tipo_combustivel >> pacote.payload.dados.preco >> pacote.payload.dados.latitude >> pacote.payload.dados.longitude;
        }
        else if (pacote.tipo_msg == 'P')
        {
            linha >> pacote.payload.pesquisa.tipo_combustivel >> pacote.payload.pesquisa.raio_busca >> pacote.payload.pesquisa.latitude_centro >> pacote.payload.pesquisa.longitude_centro;
        }
        else
        {
            cerr << "Tipo de mensagem inválido. Use 'D' ou 'P'." << endl;
            continue;
        }

        bool sucesso = false;
        while (!sucesso)
        {
            cout << "Enviando pacote " << pacote.id_msg << " (erro=" << (pacote.erro ? "sim" : "não") << ")..." << endl;
            sendto(client_socket, &pacote, sizeof(Pacote), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

            if (pacote.tipo_msg == 'D')
            {
                Resposta resposta_d;
                ssize_t bytes = recvfrom(client_socket, &resposta_d, sizeof(Resposta), 0, NULL, NULL);

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
                ssize_t bytes = recvfrom(client_socket, &resposta_p, sizeof(RespostaPesquisa), 0, NULL, NULL);

                if (bytes < 0)
                {
                    cout << "Timeout! Retransmitindo..." << endl;
                    pacote.erro = false;
                }
                else
                {
                    if (resposta_p.preco_encontrado == -2)
                    { // Código de NAK
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
    close(client_socket);
    return 0;
}
