#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>

using namespace std;

void handle_client(int client_socket)
{
    cout << "Novo thread criado para cuidar do cliente socket: " << client_socket << endl;

    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    cout << "Mensagem do cliente [" << client_socket << "]: " << buffer << endl;

    const char *message = "Olá, eu sou o garçom-thread! Bem-vindo!";
    send(client_socket, message, strlen(message), 0);

    close(client_socket);
    cout << "Conexão com o cliente " << client_socket << " fechada" << endl;
}

int main(int argc, char **argv)
{
    // Cria o socket para o servidor: AF_INET = IPv4, SOCK_STREAM = TCP, 0 = Protocolo automático
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        cerr << "Erro ao criar o socket do servidor" << endl;
        return 1;
    }

    // Configura o endereço do servidor
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Associa endereço com o socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Erro ao fazer o bind" << endl;
        return 1;
    }

    // Começa a ouvir por clientes
    if (listen(server_fd, 5) < 0)
    {
        cerr << "Erro no listen" << endl;
        return 1;
    }

    cout << "Servidor multi-cliente ouvindo na porta 8080..." << endl;

    // Looping infinito para atender todos os clientes
    while (true)
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0)
        {
            cerr << "Erro ao aceitar conexão" << endl;
            continue;
        }

        cout << "Cliente conectado! Criando uma thread para ele..." << endl;

        thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    close(server_fd);

    return 0;
}