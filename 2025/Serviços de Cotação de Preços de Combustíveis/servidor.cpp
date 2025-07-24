#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <fstream>
#include <cmath>
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

struct Posto
{
    int tipo_combustivel;
    int preco;
    double latitude;
    double longitude;
};

// Função haversine permanece a mesma
double haversine(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371.0;
    double lat_rad1 = lat1 * M_PI / 180.0;
    double lon_rad1 = lon1 * M_PI / 180.0;
    double lat_rad2 = lat2 * M_PI / 180.0;
    double lon_rad2 = lon2 * M_PI / 180.0;
    double dlon = lon_rad2 - lon_rad1;
    double dlat = lat_rad2 - lat_rad1;
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos(lat_rad1) * cos(lat_rad2) * sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " <porta>" << endl;
        return 1;
    }

    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1)
    {
        cerr << "Erro ao criar o socket do servidor" << endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Erro ao fazer o bind" << endl;
        close(server_fd);
        return 1;
    }

    cout << "Servidor UDP ouvindo na porta " << argv[1] << "..." << endl;

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
            cout << "----------------------------------------" << endl;
            cout << "Pacote " << pacote_recebido.id_msg << " deserializado. Tipo: " << pacote_recebido.tipo_msg << endl;

            // Lógica principal
            if (pacote_recebido.tipo_msg == 'D')
            {
                if (pacote_recebido.erro)
                {
                    cout << "PACOTE 'D' CORROMPIDO! Enviando NAK." << endl;
                    Resposta resposta_nak;
                    resposta_nak.id_msg_original = pacote_recebido.id_msg;
                    resposta_nak.is_nak = true;
                    sendto(server_fd, &resposta_nak, sizeof(Resposta), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                }
                else
                {
                    cout << "PACOTE 'D' OK. Salvando e enviando ACK..." << endl;
                    ofstream arquivo_dados("dados_postos.csv", ios::app);
                    if (arquivo_dados.is_open())
                    {
                        arquivo_dados << pacote_recebido.payload.dados.tipo_combustivel << ","
                                      << pacote_recebido.payload.dados.preco << ","
                                      << pacote_recebido.payload.dados.latitude << ","
                                      << pacote_recebido.payload.dados.longitude << endl;
                        arquivo_dados.close();
                    }
                    Resposta resposta_ack;
                    resposta_ack.id_msg_original = pacote_recebido.id_msg;
                    resposta_ack.is_nak = false;
                    sendto(server_fd, &resposta_ack, sizeof(Resposta), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                }
            }
            else if (pacote_recebido.tipo_msg == 'P')
            {
                RespostaPesquisa resposta_p;
                if (pacote_recebido.erro)
                {
                    cout << "PACOTE 'P' CORROMPIDO! Respondendo com falha." << endl;
                    resposta_p.preco_encontrado = -2; // Código para NAK
                }
                else
                {
                    cout << "PACOTE 'P' OK. Iniciando busca..." << endl;
                    vector<Posto> postos;
                    ifstream arquivo_dados("dados_postos.csv");
                    if (arquivo_dados.is_open())
                    {
                        string linha;
                        while (getline(arquivo_dados, linha))
                        {
                            stringstream ss(linha);
                            string item;
                            Posto posto_atual;
                            getline(ss, item, ',');
                            posto_atual.tipo_combustivel = stoi(item);
                            getline(ss, item, ',');
                            posto_atual.preco = stoi(item);
                            getline(ss, item, ',');
                            posto_atual.latitude = stod(item);
                            getline(ss, item, ',');
                            posto_atual.longitude = stod(item);
                            postos.push_back(posto_atual);
                        }
                        arquivo_dados.close();
                    }

                    int menor_preco = -1;
                    for (const auto &posto : postos)
                    {
                        if (posto.tipo_combustivel == pacote_recebido.payload.pesquisa.tipo_combustivel)
                        {
                            double distancia = haversine(pacote_recebido.payload.pesquisa.latitude_centro, pacote_recebido.payload.pesquisa.longitude_centro, posto.latitude, posto.longitude);
                            if (distancia <= pacote_recebido.payload.pesquisa.raio_busca)
                            {
                                if (menor_preco == -1 || posto.preco < menor_preco)
                                {
                                    menor_preco = posto.preco;
                                }
                            }
                        }
                    }
                    resposta_p.preco_encontrado = menor_preco;
                    cout << "Busca finalizada. Resultado: " << menor_preco << endl;
                }
                sendto(server_fd, &resposta_p, sizeof(RespostaPesquisa), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            }
        }
    }
    close(server_fd);
    return 0;
}
