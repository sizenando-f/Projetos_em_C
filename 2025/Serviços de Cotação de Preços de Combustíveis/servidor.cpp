#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <string>

#include <algorithm>
#include <memory>
#include <vector>
#include <arpa/inet.h>

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
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " <porta>" << endl;
    }
    else
    {
        // Cria o socket para o servidor: AF_INET = IPv4, SOCK_DGRAM = UDP, 0 = Protocolo automático
        int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (server_fd == -1)
        {
            cerr << "Erro ao criar o socket do servidor" << endl;
        }
        else
        {
            // Configura o endereço do servidor
            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(atoi(argv[1]));

            // Associa endereço com o socket
            if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            {
                cerr << "Erro ao fazer o bind" << endl;
                close(server_fd);
            }
            else
            {
                cout << "Servidor multi-cliente UDP ouvindo na porta " << argv[1] << "..." << endl;

                // Looping infinito para atender todos os clientes
                while (true)
                {
                    char buffer_recebido[sizeof(Pacote)];
                    sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);

                    ssize_t bytes_received = recvfrom(server_fd, buffer_recebido, sizeof(Pacote), 0, (struct sockaddr *)&client_addr, &client_len);

                    if (bytes_received == sizeof(Pacote))
                    {
                        Pacote pacote_recebido;
                        memcpy(&pacote_recebido, buffer_recebido, sizeof(Pacote));
                        cout << "Pacote deserializado com sucesso!" << endl;
                        cout << "ID da mensagem: " << pacote_recebido.id_msg << endl;
                        cout << "Tipo da mensagem: " << pacote_recebido.tipo_msg << endl;
                        cout << "Veio com erro?" << (pacote_recebido.erro ? "Sim" : "Não") << endl;

                        if (pacote_recebido.tipo_msg == 'D')
                        {
                            cout << "Preço: " << pacote_recebido.payload.dados.preco << endl;
                        }
                    }

                    const char *message = "Olá, eu sou o garçom UDP! Recebido!";
                    sendto(server_fd, message, strlen(message), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                }
            }
        }

        close(server_fd);
    }

    return 0;
}