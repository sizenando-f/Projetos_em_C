/**
 * @file Servidor.cpp
 *
 * Implementação do servidor com threads.
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
#include <thread>
#include <mutex>

using namespace std;

// Mutex para proteger o acesso ao arquivo dados_postos.csv
mutex file_mutex;

// As structs (Pacote, Resposta, etc.) permanecem exatamente as mesmas
struct Pacote {
    char tipo_msg;
    int id_msg;
    bool erro;
    union {
        struct { 
            int tipo_combustivel; 
            int preco; 
            double latitude; 
            double longitude; 
        } dados;
        struct { int tipo_combustivel; 
            double raio_busca; 
            double latitude_centro; 
            double longitude_centro; 
        } pesquisa;
    } tipo_pacote;
};

struct Resposta {
    int id_msg_original;
    bool is_nak;
};

struct RespostaPesquisa {
    int preco_encontrado;
};

struct Posto {
    int tipo_combustivel;
    int preco;
    double latitude;
    double longitude;
};

double haversine(double lat1, double lon1, double lat2, double lon2) {
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

/**
 * @brief Função que processa a requisição de um cliente em uma thread separada.
 * * @param server_socket_fd File descriptor do socket do servidor.
 * @param pacote_recebido O pacote de dados recebido do cliente.
 * @param cliente_endereco A estrutura de endereço do cliente que enviou o pacote.
 */
void processar_requisicao(int server_socket_fd, Pacote pacote_recebido, sockaddr_in cliente_endereco) {
    cout << "Processando pacote de ID:" << pacote_recebido.id_msg << endl;

    if (pacote_recebido.tipo_msg == 'D') {
        if (pacote_recebido.erro) {
            cout << "PACOTE 'D' CORROMPIDO! Enviando NAK." << endl;
            Resposta resposta_nak;
            resposta_nak.id_msg_original = pacote_recebido.id_msg;
            resposta_nak.is_nak = true;
            sendto(server_socket_fd, &resposta_nak, sizeof(Resposta), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
        } else {
            cout << "PACOTE 'D' OK. Salvando e enviando ACK..." << endl;
            
            // Ativa mutex para garantir que o acesso seja feito apenas por uma thread por vez
            file_mutex.lock(); // Adquire o "cadeado"
            ofstream arquivo_dados("dados_postos.csv", ios::app);
            if (arquivo_dados.is_open()) {
                arquivo_dados << pacote_recebido.tipo_pacote.dados.tipo_combustivel << ","
                              << pacote_recebido.tipo_pacote.dados.preco << ","
                              << pacote_recebido.tipo_pacote.dados.latitude << ","
                              << pacote_recebido.tipo_pacote.dados.longitude << endl;
                arquivo_dados.close();
            }
            file_mutex.unlock(); 

            Resposta resposta_ack;
            resposta_ack.id_msg_original = pacote_recebido.id_msg;
            resposta_ack.is_nak = false;
            sendto(server_socket_fd, &resposta_ack, sizeof(Resposta), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
        }
    } else if (pacote_recebido.tipo_msg == 'P') {
        RespostaPesquisa resposta_p;
        if (pacote_recebido.erro) {
            cout << "PACOTE 'P' CORROMPIDO! Respondendo com falha." << endl;
            resposta_p.preco_encontrado = -2; // Código para NAK
        } else {
            cout << "PACOTE 'P' OK. Iniciando busca..." << endl;
            vector<Posto> postos;
            
            // Protege com mutex novamente
            file_mutex.lock(); // Adquire o "cadeado" para leitura segura
            ifstream arquivo_dados("dados_postos.csv");
            if (arquivo_dados.is_open()) {
                string linha;
                while (getline(arquivo_dados, linha)) {
                    stringstream ss(linha);
                    string item;
                    Posto posto_atual;
                    getline(ss, item, ','); posto_atual.tipo_combustivel = stoi(item);
                    getline(ss, item, ','); posto_atual.preco = stoi(item);
                    getline(ss, item, ','); posto_atual.latitude = stod(item);
                    getline(ss, item, ','); posto_atual.longitude = stod(item);
                    postos.push_back(posto_atual);
                }
                arquivo_dados.close();
            }
            file_mutex.unlock();
            
            int menor_preco = -1;
            for (const auto &posto : postos) {
                if (posto.tipo_combustivel == pacote_recebido.tipo_pacote.pesquisa.tipo_combustivel) {
                    double distancia = haversine(pacote_recebido.tipo_pacote.pesquisa.latitude_centro, pacote_recebido.tipo_pacote.pesquisa.longitude_centro, posto.latitude, posto.longitude);
                    if (distancia <= pacote_recebido.tipo_pacote.pesquisa.raio_busca) {
                        if (menor_preco == -1 || posto.preco < menor_preco) {
                            menor_preco = posto.preco;
                        }
                    }
                }
            }
            resposta_p.preco_encontrado = menor_preco;
            cout << "Busca finalizada. Resultado: " << menor_preco << endl;
        }
        sendto(server_socket_fd, &resposta_p, sizeof(RespostaPesquisa), 0, (struct sockaddr *)&cliente_endereco, sizeof(cliente_endereco));
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <porta>" << endl;
        return 1;
    }

    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        cerr << "Erro ao criar o socket do servidor" << endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Erro ao fazer o bind" << endl;
        close(server_socket);
        return 1;
    }

    cout << "Servidor UDP ouvindo na porta " << argv[1] << "..." << endl;

    while (true) {
        char buffer_recebido[sizeof(Pacote)];
        sockaddr_in cliente_endereco;
        socklen_t cliente_tam = sizeof(cliente_endereco);
        
        ssize_t bytes_recebidos = recvfrom(server_socket, buffer_recebido, sizeof(Pacote), 0, (struct sockaddr *)&cliente_endereco, &cliente_tam);

        if (bytes_recebidos == sizeof(Pacote)) {
            Pacote pacote_copiado;
            memcpy(&pacote_copiado, buffer_recebido, sizeof(Pacote));
            
            // Cria uma nova thread para processar a requisição e a "solta" para o loop principal esperar pela próxima requisição.
            thread cliente_thread(processar_requisicao, server_socket, pacote_copiado, cliente_endereco);
            cliente_thread.detach();
        }
    }

    close(server_socket);
    return 0;
}
