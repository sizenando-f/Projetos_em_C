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

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        cerr << "Uso: " << argv[0] << " <ip/nome> <porta>" << endl;
    }
    else
    {
        // Cria o socket (telefone) do cliente
        int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

        if (client_socket == -1)
        {
            cerr << "Erro ao criar o socket do cliente!" << endl;
        }
        else
        {
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

                    srand(time(NULL));
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

                    // Se comunica com o servidor seja enviando uma mensagem ou recebendo
                    char buffer_envio[sizeof(Pacote)];
                    memcpy(buffer_envio, &pacote, sizeof(Pacote));
                    sendto(client_socket, buffer_envio, sizeof(Pacote), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
                    cout << "Pacote convertido e enviado para o servidor" << endl;

                    char buffer[1024] = {0};
                    socklen_t server_len = sizeof(server_addr);
                    recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_len);
                    cout << "Resposta do servidor:" << buffer << endl;

                    cout << "> ";
                    getline(cin, input);
                }

                close(client_socket);
            }
        }
    }

    return 0;
}