/**
 * @file servidor.cpp
 *
 * Implementação do servidor thread. Realiza inserção de dados de postos e pesquisas com haversine
 *
 * @author Sizenando Souza França | RGM: 50575
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
#include <thread>
#include <mutex>
#include <iomanip>

using namespace std;

// Mutex para bloquear uso do arquivo nas threads
mutex arquivo_mutex;

// Pacote principal de comunicação
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

struct Posto
{
    int tipo_combustivel; // 0 (diesel), 1 (alcool), 2 (gasolina)
    int preco;
    double latitude;
    double longitude;
};

/**
 * @brief Distância de Haversine
 *
 * Aplica a fórmula de haversine entre dois pontos desejados
 *
 * @param lat1 Latitude do primeiro ponto
 * @param lon1 Longitude do primeiro ponto
 * @param lat2 Latitude do segundo ponto
 * @param lon2 Longitude do segundo ponto
 *
 * @return Distância entre os dois pontos
 */
double haversine(double lat1, double lon1, double lat2, double lon2)
{
    // Raio da terra em quilômetros
    const double R = 6371.0;

    double lat_rad1 = lat1 * M_PI / 180.0;
    double lon_rad1 = lon1 * M_PI / 180.0;
    double lat_rad2 = lat2 * M_PI / 180.0;
    double lon_rad2 = lon2 * M_PI / 180.0;

    double dlon = lon_rad2 - lon_rad1;
    double dlat = lat_rad2 - lat_rad1;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos(lat_rad1) * cos(lat_rad2) * sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    // Retorna a distância entre os dois pontos
    return R * c;
}

/**
 * @brief Processa um requisição
 *
 * Em uma thread separada, a requisição é processada
 *
 * @param servidor_socket Socket do servidor
 * @param pacote_recebido Pacote recebido pelo cliente
 * @param cliente_endereco Endereço do cliente
 */
void processar_requisicao(int servidor_socket, Pacote pacote_recebido, sockaddr_in cliente_endereco)
{
    cout << "[ -> ] Processando pacote de ID: " << pacote_recebido.id_msg << endl;

    if (pacote_recebido.tipo_msg == 'D')
    {
        // Para quando o pacote vier com erro simulado
        if (pacote_recebido.erro)
        {
            cerr << "[ -># ] PACOTE 'D' ESTÁ CORROMPIDO. Retornando NAK." << endl;

            Resposta resposta_nak;
            resposta_nak.id_msg_original = pacote_recebido.id_msg;
            resposta_nak.is_nak = true;

            sendto(servidor_socket, &resposta_nak, sizeof(Resposta), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
        }
        // Pacote está OK
        else
        {
            cout << "[ -> ] PACOTE 'D' ESTÁ OK. Atualizando arquivo..." << endl;

            // Trava para operação do arquivo
            arquivo_mutex.lock();

            // Vetor temporário para os postos existentes no arquivo
            vector<Posto> postos;
            ifstream arquivo_leitura("dados_postos.csv");

            if (arquivo_leitura.is_open())
            {
                string linha;
                // Insere todos os postos do arquivo no vector
                while (getline(arquivo_leitura, linha))
                {
                    stringstream ss(linha);
                    string item;
                    Posto p;
                    getline(ss, item, ',');
                    p.tipo_combustivel = stoi(item);
                    getline(ss, item, ',');
                    p.preco = stoi(item);
                    getline(ss, item, ',');
                    p.latitude = stod(item);
                    getline(ss, item, ',');
                    p.longitude = stod(item);
                    postos.push_back(p);
                }
                arquivo_leitura.close();
            }

            // Verifica se o posto já existe com o mesmo tipo de combustível
            bool posto_encontrado = false;
            for (auto &posto : postos)
            {
                // Se já existir, modifica suas informações
                if (posto.tipo_combustivel == pacote_recebido.tipo_pacote.dados.tipo_combustivel && abs(posto.latitude - pacote_recebido.tipo_pacote.dados.latitude) < 1e-6 && abs(posto.longitude - pacote_recebido.tipo_pacote.dados.longitude) < 1e-6)
                {
                    cout << "[ -> ] O posto já existe. Atualizando o preço de " << posto.preco << " para " << pacote_recebido.tipo_pacote.dados.preco << endl;
                    posto.preco = pacote_recebido.tipo_pacote.dados.preco;
                    posto_encontrado = true;
                    break;
                }
            }

            // Se o posto não existir, cria um novo
            if (!posto_encontrado)
            {
                cout << "[ -> ] Adicionando o novo posto..." << endl;
                Posto novo_posto;
                novo_posto.tipo_combustivel = pacote_recebido.tipo_pacote.dados.tipo_combustivel;
                novo_posto.preco = pacote_recebido.tipo_pacote.dados.preco;
                novo_posto.latitude = pacote_recebido.tipo_pacote.dados.latitude;
                novo_posto.longitude = pacote_recebido.tipo_pacote.dados.longitude;
                postos.push_back(novo_posto);
            }

            // Escrita dos postos no arquivo | trunc apaga o antigo conteúdo e insere o novo
            ofstream arquivo_escrita("dados_postos.csv", ios::trunc);
            if (arquivo_escrita.is_open())
            {
                // Escreve posto por posto
                for (const auto &posto : postos)
                {
                    arquivo_escrita << posto.tipo_combustivel << ","
                                    << posto.preco << ","
                                    << fixed << setprecision(6) << posto.latitude << ","
                                    << fixed << setprecision(6) << posto.longitude << endl;
                }
                arquivo_escrita.close();
            }

            // Desbloqueia para outras threads poderem acessar o arquivo
            arquivo_mutex.unlock();

            // Envia ACK para o cliente
            Resposta resposta_ack;
            resposta_ack.id_msg_original = pacote_recebido.id_msg;
            resposta_ack.is_nak = false;
            sendto(servidor_socket, &resposta_ack, sizeof(Resposta), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
        }
    }
    else if (pacote_recebido.tipo_msg == 'P')
    {
        RespostaPesquisa resposta_p;
        // Verifica se pacote possui o erro simulado
        if (pacote_recebido.erro)
        {
            cout << "[ -># ] PACOTE 'P' ESTÁ CORROMPIDO! Retornando NAK." << endl;
            resposta_p.preco_encontrado = -2; // Código para NAK
        }
        // Se pacote estiver OK
        else
        {
            cout << "[ -> ] PACOTE 'P' ESTÁ OK. Iniciando a busca..." << endl;
            // Vetor temporário para acessar os postos
            vector<Posto> postos;

            arquivo_mutex.lock();

            ifstream arquivo_dados("dados_postos.csv");
            if (arquivo_dados.is_open())
            {
                // Carrega todos postos no arquivo
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
            arquivo_mutex.unlock();

            // Código inicial indicando que não foi encontrado
            int menor_preco = -1;
            for (const auto &posto : postos)
            {
                if (posto.tipo_combustivel == pacote_recebido.tipo_pacote.pesquisa.tipo_combustivel)
                {
                    // Calcula distância entre centro de busca e o posto atual
                    double distancia = haversine(pacote_recebido.tipo_pacote.pesquisa.latitude_centro, pacote_recebido.tipo_pacote.pesquisa.longitude_centro, posto.latitude, posto.longitude);

                    // Se for menor que a distância, muda o preço se for o menor
                    if (distancia <= pacote_recebido.tipo_pacote.pesquisa.raio)
                    {
                        if (menor_preco == -1 || posto.preco < menor_preco)
                        {
                            menor_preco = posto.preco;
                        }
                    }
                }
            }

            // Define o menor preço
            resposta_p.preco_encontrado = menor_preco;
            cout << "[ -> ] Busca finalizada. Menor preço encontrado: " << menor_preco << endl;
        }

        // Retorna o resultado da pesquisa para o cliente
        sendto(servidor_socket, &resposta_p, sizeof(RespostaPesquisa), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
    }
}

int main(int argc, char **argv)
{
    // Se o usuário inserir a quantidade inválida de parâmetros
    if (argc != 2)
    {
        cerr << "[ -> ] Uso: " << argv[0] << " <porta>" << endl;
        return 1;
    }
    else
    {
        // Define o socket do servidor
        int servidor_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (servidor_socket == -1)
        {
            cerr << "[ ->#] Erro ao criar o socket do servidor" << endl;
            return 1;
        }
        else
        {
            // Define endereço do servidor
            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(atoi(argv[1]));

            // Vincula socket com o endereço
            if (bind(servidor_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            {
                cerr << "[ -># ] Erro ao fazer o bind!" << endl;
                close(servidor_socket);
                return 1;
            }
            else
            {
                cout << "[ -> ] Servidor UDP ouvindo na porta " << argv[1] << "..." << endl;

                // Looping principal
                while (true)
                {
                    // Define o buffer para receber os pacotes
                    char buffer_recebido[sizeof(Pacote)];
                    sockaddr_in cliente_endereco;
                    socklen_t cliente_tam = sizeof(cliente_endereco);

                    // Recebe o pacote do cliente
                    ssize_t bytes_recebidos = recvfrom(servidor_socket, buffer_recebido, sizeof(Pacote), 0, (struct sockaddr *)&cliente_endereco, &cliente_tam);

                    // Verifica se o pacote está integro
                    if (bytes_recebidos == sizeof(Pacote))
                    {
                        // Converte buffer de bytes para a struct Pacote
                        Pacote pacote_copiado;
                        memcpy(&pacote_copiado, buffer_recebido, sizeof(Pacote));

                        // Separa uma thread para processar o pacote
                        thread cliente_thread(processar_requisicao, servidor_socket, pacote_copiado, cliente_endereco);

                        // Libera para o servidor continuar rodando
                        cliente_thread.detach();
                    }
                }
            }
        }
        close(servidor_socket);
    }

    return 0;
}