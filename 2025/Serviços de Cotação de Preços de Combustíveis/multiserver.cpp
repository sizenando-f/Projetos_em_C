#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

struct Ponto
{
    double x, y;
    int id;
};

struct No
{
    Ponto p;
    unique_ptr<No> esquerda;
    unique_ptr<No> direita;

    No(Ponto pt) : p(pt), esquerda(nullptr), direita(nullptr) {}
};

unique_ptr<No> construir_kdtree(vector<Ponto> &pontos, int depth)
{
    if (pontos.empty())
    {
        return nullptr;
    }

    int eixo = depth % 2;

    sort(pontos.begin(), pontos.end(), [eixo](const Ponto &a, const Ponto &b)
         { return (eixo == 0) ? a.x < b.x : a.y < b.y; });

    size_t mediana_idx = pontos.size() / 2;
    Ponto ponto_mediana = pontos[mediana_idx];

    vector<Ponto> pontos_esquerda(pontos.begin(), pontos.begin() + mediana_idx);
    vector<Ponto> pontos_direita(pontos.begin() + mediana_idx + 1, pontos.end());

    auto no_raiz = make_unique<No>(ponto_mediana);
    no_raiz->esquerda = construir_kdtree(pontos_esquerda, depth + 1);
    no_raiz->direita = construir_kdtree(pontos_direita, depth + 1);

    return no_raiz;
}

struct CaixaBusca
{
    double x_min, x_max, y_min, y_max;
};

void buscar_kdtree(No *no_atual, const CaixaBusca &caixa, int depth, vector<Ponto> &resultados)
{
    if (no_atual == nullptr)
    {
        return;
    }

    Ponto p = no_atual->p;
    if (p.x >= caixa.x_min && p.x <= caixa.x_max && p.y >= caixa.y_min && p.y <= caixa.y_max)
    {
        resultados.push_back(p);
    }

    int eixo = depth % 2;
    double valor_ponto_no_eixo = (eixo == 0) ? p.x : p.y;
    double min_caixa_no_eixo = (eixo == 0) ? caixa.x_min : caixa.y_min;
    double max_caixa_no_eixo = (eixo == 0) ? caixa.x_max : caixa.y_max;

    if (min_caixa_no_eixo < valor_ponto_no_eixo)
    {
        buscar_kdtree(no_atual->esquerda.get(), caixa, depth + 1, resultados);
    }

    if (max_caixa_no_eixo > valor_ponto_no_eixo)
    {
        buscar_kdtree(no_atual->direita.get(), caixa, depth + 1, resultados);
    }
}

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