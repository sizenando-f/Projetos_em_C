#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main()
{
    // Cria o socket (telefone) do cliente
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    ;
    if (client_socket == -1)
    {
        cerr << "Erro ao criar o socket do cliente!" << endl;
        return 1;
    }

    // Define que tipo de endereço o cliente vai se conectar e a porta do servidor
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    // Converte o endereço em string para o formato binario
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        cerr << "Endereço IP inválido ou não suportado" << endl;
        return 1;
    }

    // Conecta o socket criado do cliente no endereço desejado
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Erro na conexão com o servidor" << endl;
        return 1;
    }

    cout << "Conectado ao servidor!" << endl;

    // Se comunica com o servidor seja enviando uma mensagem ou recebendo
    const char *message = "Olá, servidor!";
    send(client_socket, message, strlen(message), 0);
    cout << "Mensagem enviada ao servidor!" << endl;

    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    cout << "Resposta do servidor:" << buffer << endl;

    close(client_socket);

    return 0;
}