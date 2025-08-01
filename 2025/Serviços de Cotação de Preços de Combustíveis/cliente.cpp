/**
 * @file Cliente.cpp
 *
 * Implementação do cliente
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
#include <limits>

using namespace std;

struct Pacote
{
    char tipo_msg; // 'D' ou 'P
    int id_msg;    // Para debug
    bool erro;     // Corrupção simulada do pacote

    // Para facilitar acesso do pacote
    union
    {
        // Para mensagens de inserção de dados
        struct
        {
            int tipo_combustivel; // 0 (diesel), 1 (alcool), 2 (gasolina)
            int preco;
            double latitude;
            double longitude;
        } dados;

        // Para mensagens de pesquisa
        struct
        {
            int tipo_combustivel; // 0 (diesel), 1 (alcool), 2 (gasolina)
            double raio;
            double latitude_centro;
            double longitude_centro;
        } pesquisa;
    } tipo_pacote;
};

// Para envios de ACK e NAK
struct Resposta
{
    int id_msg_original;
    bool is_nak;
};

// Para envios de resposta
struct RespostaPesquisa
{
    int preco_encontrado; // -2 se NAK, -1 se não encontrado
};

/**
 * @brief Limpa o buffer do cin
 *
 * Função para limpar o buffer de entrada em caso de erro
 */
void limpar_buffer_cin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main(int argc, char **argv)
{
    // Se o usuário inserir a quantidade inválida de parâmetros
    if (argc != 3)
    {
        cerr << "[ -># ] Uso: " << argv[0] << " <ip/nome> <porta>" << endl;
        return 1;
    }
    else
    {
        // Inicia o timer para randomizar
        srand(time(NULL));

        // Define o socket do cliente
        int cliente_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (cliente_socket == -1)
        {
            cerr << "[ -># ] Erro ao criar o socket do cliente!" << endl;
            return 1;
        }
        else
        {
            // Define tempo de timeout
            struct timeval timeout;
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            // Associa timeout ao socket
            setsockopt(cliente_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            sockaddr_in servidor_endereco;
            servidor_endereco.sin_family = AF_INET;
            servidor_endereco.sin_port = htons(atoi(argv[2]));

            if (inet_pton(AF_INET, argv[1], &servidor_endereco.sin_addr) <= 0)
            {
                cerr << "[ -># ] Endereço IP inválido ou não suportado" << endl;
                return 1;
            }
            else
            {
                cout << "-----------------------------------------------------------------------" << endl;
                cout << "Padrão de entrada para Dados (D) e Pesquisa (P):" << endl;
                cout << "> D <tipo_comb(0-2)> <preco> <latitude> <longitude>" << endl;
                cout << "> P <tipo_comb(0-2)> <raio_busca> <latitude_centro> <longitude_centro>" << endl;
                cout << "Escreva <exit> para sair" << endl;
                cout << "-----------------------------------------------------------------------" << endl;
                string input;

                while (cout << "> ", getline(cin, input) && input != "exit")
                {
                    // Se a pessoa não escrever nada
                    if (input.empty())
                        continue;

                    stringstream linha(input);
                    Pacote pacote;
                    string tipo_str;

                    // Pega primeira palavra
                    linha >> tipo_str;

                    // Verifica se é D ou P
                    if (tipo_str.length() != 1 || (tipo_str[0] != 'D' && tipo_str[0] != 'P'))
                    {
                        cerr << "[ #<- ] Tipo de mensagem inválido. Use 'D' ou 'P'." << endl;
                        continue;
                    }

                    pacote.tipo_msg = tipo_str[0];

                    bool entrada_valida = false;
                    // Insere informações no pacote ao mesmo tempo que verifica entrada inválida
                    if (pacote.tipo_msg == 'D')
                    {
                        if (linha >> pacote.tipo_pacote.dados.tipo_combustivel >> pacote.tipo_pacote.dados.preco >> pacote.tipo_pacote.dados.latitude >> pacote.tipo_pacote.dados.longitude)
                        {
                            // Verifica se há lixo no final da linha
                            string lixo;
                            if (!(linha >> lixo))
                            {
                                if (pacote.tipo_pacote.dados.tipo_combustivel >= 0 && pacote.tipo_pacote.dados.tipo_combustivel <= 2)
                                {
                                    entrada_valida = true;
                                }
                                else
                                {
                                    cerr << "[ #<- ] Tipo de combustível inválido. Use 0 (Diesel), 1 (Álcool) ou 2 (Gasolina)." << endl;
                                }
                            }
                        }
                    }
                    else if (pacote.tipo_msg == 'P')
                    {
                        if (linha >> pacote.tipo_pacote.pesquisa.tipo_combustivel >> pacote.tipo_pacote.pesquisa.raio >> pacote.tipo_pacote.pesquisa.latitude_centro >> pacote.tipo_pacote.pesquisa.longitude_centro)
                        {
                            // Verifica o lixo novamente
                            string lixo;
                            if (!(linha >> lixo))
                            {
                                if (pacote.tipo_pacote.pesquisa.tipo_combustivel >= 0 && pacote.tipo_pacote.pesquisa.tipo_combustivel <= 2)
                                {
                                    entrada_valida = true;
                                }
                                else
                                {
                                    cerr << "[ #<- ] Tipo de combustível inválido. Use 0 (Diesel), 1 (Álcool) ou 2 (Gasolina)." << endl;
                                }
                            }
                        }
                    }

                    if (!entrada_valida)
                    {
                        cerr << "[ #<- ] Formato de entrada incorreto ou dados inválidos. Tente novamente." << endl;
                        continue;
                    }

                    // Randomiza ID para debug e facilidade de encontrar os pacotes
                    pacote.id_msg = rand() % 1000;

                    // Randomiza erro
                    pacote.erro = ((double)rand() / RAND_MAX) > 0.5;

                    bool sucesso = false;
                    while (!sucesso)
                    {
                        cout << "[ -> ] Enviando pacote " << pacote.id_msg << " (erro=" << (pacote.erro ? "sim" : "não") << ")..." << endl;
                        sendto(cliente_socket, &pacote, sizeof(Pacote), 0, (struct sockaddr *)&servidor_endereco, sizeof(servidor_endereco));

                        // Para mensagens tipo D
                        if (pacote.tipo_msg == 'D')
                        {
                            Resposta resposta_d;
                            ssize_t bytes = recvfrom(cliente_socket, &resposta_d, sizeof(Resposta), 0, NULL, NULL);

                            if (bytes < 0)
                            {
                                cout << "[ -># ] Timeout! Retransmitindo..." << endl;
                            }
                            else if (resposta_d.id_msg_original == pacote.id_msg)
                            {
                                if (resposta_d.is_nak)
                                {
                                    cout << "[ -># ] NAK recebido. Retransmitindo..." << endl;
                                }
                                else
                                {
                                    cout << "[ -> ] ACK recebido! Sucesso!" << endl;
                                    sucesso = true;
                                }
                            }
                            // Randomiza erro novamente
                            if (!sucesso)
                            {
                                pacote.erro = ((double)rand() / RAND_MAX) > 0.5;
                            }
                        }
                        // Para mensagens tipo P
                        else if (pacote.tipo_msg == 'P')
                        {
                            RespostaPesquisa resposta_p;
                            ssize_t bytes = recvfrom(cliente_socket, &resposta_p, sizeof(RespostaPesquisa), 0, NULL, NULL);

                            // Se houver timout
                            if (bytes < 0)
                            {
                                cout << "[ -># ] Timeout! Retransmitindo..." << endl;
                            }
                            else
                            {
                                if (resposta_p.preco_encontrado == -2)
                                {
                                    cout << "[ -># ] NAK recebido para pesquisa. Retransmitindo..." << endl;
                                }
                                else
                                {
                                    if (resposta_p.preco_encontrado == -1)
                                    {
                                        cout << "[ -> ] Resposta do servidor: Nenhum posto encontrado." << endl;
                                    }
                                    else
                                    {
                                        cout << "[ -> ] Resposta do servidor: Menor preço encontrado: " << resposta_p.preco_encontrado << endl;
                                    }
                                    sucesso = true;
                                }
                            }
                            if (!sucesso)
                            {
                                pacote.erro = ((double)rand() / RAND_MAX) > 0.5;
                            }
                        }
                    }
                }
            }
        }
        close(cliente_socket);
    }

    return 0;
}