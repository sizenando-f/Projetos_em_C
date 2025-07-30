/**
 * @file Servidor.cpp
 *
 * Implementação do servidor. Espera por acessos tipos D ou P, armazena resultados e retorna postos
 * no raio desejado com o menor preço do combustível procurado
 *
 * @author Sizenando S. França | RGM: 50575
 */

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

// Pacote que será recebido podendo ser de dado ou pesquisa
struct Pacote
{
    char tipo_msg; // D ou P
    int id_msg;    // Identificador gerado pelo cliente
    bool erro;     // Se o pacote possui erro

    // Permite ser um dos dois na mesma struct
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
            int tipo_combustivel;
            int raio_busca; // Raio em Km
            double latitude_centro;
            double longitude_centro;
        } pesquisa;
    } tipo_pacote;
};

// Resposta de ACK ou NAK para o cliente
struct Resposta
{
    int id_msg_original; // Identificador da mensagem que receberá o ACK/NAK
    bool is_nak;         // true se for NAK e false se for ACK
};

// Pacote contendo o preço encontrado
struct RespostaPesquisa
{
    int preco_encontrado; // -1 se não encontrou, -2 para NAK
};

// Posto a ser armazenado
struct Posto
{
    int tipo_combustivel;
    int preco;
    double latitude;
    double longitude;
};

/**
 * @brief Cálculo Haversine
 *
 * A função haversine determina a distância entre dois pontos numa esfera
 *
 * @param lat1 Latitude do centro de busca
 * @param lon1 Longitude do centro de busca
 * @param lat2 Latitude qualquer
 * @param lon2 Longitude qualquer
 *
 * @return Distância em Km
 */
double haversine(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371.0; // Distância da circuferência da Terr em Km

    // Conversão dos valores para radiano
    double lat_rad1 = lat1 * M_PI / 180.0;
    double lon_rad1 = lon1 * M_PI / 180.0;
    double lat_rad2 = lat2 * M_PI / 180.0;
    double lon_rad2 = lon2 * M_PI / 180.0;

    // Da distância entre um ponto e outro
    double dlon = lon_rad2 - lon_rad1;
    double dlat = lat_rad2 - lat_rad1;

    // Cálculo de haversine
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos(lat_rad1) * cos(lat_rad2) * sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}

int main(int argc, char **argv)
{
    // Se não houver o número de parâmetros corretos
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " <porta>" << endl;
        return 1;
    }
    else
    {
        // Cria o socket do servidor. AF_INET é a família IPv4 e SOCK_DGRAM o UDP
        int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (server_socket == -1)
        {
            cerr << "Erro ao criar o socket do servidor" << endl;
            return 1;
        }
        else
        {
            // Configura endereõ do servidor
            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;            // Ipv4
            server_addr.sin_addr.s_addr = INADDR_ANY;    // Qualquer endereço
            server_addr.sin_port = htons(atoi(argv[1])); // Converte porta para formato correto

            // Realiza a fusão entre socket e endereço do servidor
            if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            {
                cerr << "Erro ao fazer o bind" << endl;
                close(server_socket);
                return 1;
            }
            else
            {
                cout << "Servidor UDP ouvindo na porta " << argv[1] << "..." << endl;

                // Looping principal
                while (true)
                {
                    // Configura recebimento do pacote
                    char buffer_recebido[sizeof(Pacote)];
                    sockaddr_in cliente_endereco;
                    socklen_t cliente_tam = sizeof(cliente_endereco);
                    // Recebe o pacote em bytes através do recvfrom por ser UDP
                    ssize_t bytes_recebidos = recvfrom(server_socket, buffer_recebido, sizeof(Pacote), 0, (struct sockaddr *)&cliente_endereco, &cliente_tam);

                    if (bytes_recebidos == sizeof(Pacote))
                    {
                        // Converte os bytes recebidos para struct
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
                                // Envia para o cliente a resposta contendo o NAK
                                sendto(server_socket, &resposta_nak, sizeof(Resposta), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
                            }
                            else
                            {
                                cout << "PACOTE 'D' OK. Salvando e enviando ACK..." << endl;
                                // Salva os dados numa planilha
                                ofstream arquivo_dados("dados_postos.csv", ios::app);
                                if (arquivo_dados.is_open())
                                {
                                    arquivo_dados << pacote_recebido.tipo_pacote.dados.tipo_combustivel << ","
                                                  << pacote_recebido.tipo_pacote.dados.preco << ","
                                                  << pacote_recebido.tipo_pacote.dados.latitude << ","
                                                  << pacote_recebido.tipo_pacote.dados.longitude << endl;
                                    arquivo_dados.close();
                                }
                                Resposta resposta_ack;
                                resposta_ack.id_msg_original = pacote_recebido.id_msg;
                                resposta_ack.is_nak = false;
                                // Envia uma resposta ao cliente contendo um ACK
                                sendto(server_socket, &resposta_ack, sizeof(Resposta), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
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
                                vector<Posto> postos; // Vetor onde ficará os postos do arquivo para busca
                                ifstream arquivo_dados("dados_postos.csv");
                                if (arquivo_dados.is_open())
                                {
                                    string linha;
                                    // Monta as postos e coloca no vetor
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
                                    // Filtra primeiro pelo tipo de combustível pelo procurado
                                    if (posto.tipo_combustivel == pacote_recebido.tipo_pacote.pesquisa.tipo_combustivel)
                                    {
                                        // Realiza o cálculo da distância em Km
                                        double distancia = haversine(pacote_recebido.tipo_pacote.pesquisa.latitude_centro, pacote_recebido.tipo_pacote.pesquisa.longitude_centro, posto.latitude, posto.longitude);
                                        // Distância menor que o raio encontrado
                                        if (distancia <= pacote_recebido.tipo_pacote.pesquisa.raio_busca)
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
                            // Envia pacote contendo o menor preço encontrado para o cliente
                            sendto(server_socket, &resposta_p, sizeof(RespostaPesquisa), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
                        }
                    }
                }
                close(server_socket);
            }
        }
    }

    return 0;
}
