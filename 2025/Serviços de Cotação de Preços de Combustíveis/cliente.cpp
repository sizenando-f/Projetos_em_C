#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <sstream>

using namespace std;

struct Pacote
{
    char tipo_msg; // D ou P
    int id_msg;
    bool erro;

    union
    {
        // Para tipo D (Dados)
        struct
        {
            int tipo_combustivel; // 0 (Diesel), 1 (Álcool), 2 (Gasolina)
            int preco;            // 4449 -> 4,449
            double latitude;
            double longitude;
        } dados;

        // Para tipo P (Pesquisa)
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

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        cerr << "Uso: " << argv[0] << " <ip/nome> <porta>" << endl;
    }
    else
    {
        srand(time(NULL));
        // Cria o socket (telefone) do cliente
        int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

        if (client_socket == -1)
        {
            cerr << "Erro ao criar o socket do cliente!" << endl;
        }
        else
        {
            // Seta o timeout para caso o pacote se perder na rede
            struct timeval timeout;
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;
            setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

            // Define que tipo de endereço o cliente vai se conectar e a porta do servidor
            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(atoi(argv[2]));

            // Converte o endereço em string para o formato binario
            if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
            {
                cerr << "Endereço IP inválido ou não suportado" << endl;
            }
            else
            {
                cout << "Escreva <exit> para sair" << endl;
                string input = "";
                cout << "> ";
                getline(cin, input);
                while (input != "exit")
                {
                    stringstream linha(input);
                    Pacote pacote;
                    string tempChar;
                    double tempValue;

                    double prob = (double)rand() / RAND_MAX;
                    if (prob > 0.5)
                    {
                        pacote.erro = true;
                    }
                    else
                    {
                        pacote.erro = false;
                    }

                    pacote.id_msg = rand() % 100;
                    linha >> tempChar;

                    if (tempChar == "D")
                    {
                        pacote.tipo_msg = 'D';
                        linha >> tempValue;
                        pacote.payload.dados.tipo_combustivel = tempValue;
                        linha >> tempValue;
                        pacote.payload.dados.preco = tempValue;
                        linha >> tempValue;
                        pacote.payload.dados.latitude = tempValue;
                        linha >> tempValue;
                        pacote.payload.dados.longitude = tempValue;
                    }
                    else if (tempChar == "P")
                    {
                        pacote.tipo_msg = 'P';
                        linha >> tempValue;
                        pacote.payload.pesquisa.tipo_combustivel = tempValue;
                        linha >> tempValue;
                        pacote.payload.pesquisa.raio_busca = tempValue;
                        linha >> tempValue;
                        pacote.payload.pesquisa.latitude_centro = tempValue;
                        linha >> tempValue;
                        pacote.payload.pesquisa.longitude_centro = tempValue;
                    }

                    bool ack_recebido = false;
                    while (!ack_recebido)
                    {
                        char buffer_envio[sizeof(Pacote)];
                        memcpy(buffer_envio, &pacote, sizeof(Pacote));
                        sendto(client_socket, buffer_envio, sizeof(Pacote), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
                        cout << "Pacote " << pacote.id_msg << " enviado. Aguardando ACK/NAK" << endl;

                        char buffer_resposta[sizeof(Resposta)];
                        socklen_t server_len = sizeof(server_addr);
                        ssize_t bytes_received = recvfrom(client_socket, buffer_resposta, sizeof(Resposta), 0, (struct sockaddr *)&server_addr, &server_len);

                        if (bytes_received < 0)
                        {
                            cout << "Timeout! O ACK/NAK foi perdido. Retransmitindo pacote " << pacote.id_msg << "..." << endl;
                            pacote.erro = false;
                        }
                        else
                        {
                            Resposta resposta_servidor;
                            memcpy(&resposta_servidor, buffer_resposta, sizeof(Resposta));

                            if (resposta_servidor.id_msg_original == pacote.id_msg)
                            {
                                if (resposta_servidor.is_nak)
                                {
                                    cout << "NAK recebido. Retransmitindo pacote " << pacote.id_msg << "..." << endl;
                                    pacote.erro = false;
                                }
                                else
                                {
                                    cout << "ACK recebido para o pacote " << pacote.id_msg << "! Sucesso!" << endl;
                                    ack_recebido = true;
                                }
                            }
                        }
                    }

                    cout << "> ";
                    getline(cin, input);
                }

                close(client_socket);
            }
        }
    }

    return 0;
}