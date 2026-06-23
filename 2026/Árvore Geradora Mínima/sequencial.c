#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/**
 * @brief Uma aresta do grafo
 * @param origem De onde a aresta sai
 * @param destino Aonde a aresta chega
 * @param peso Peso da aresta
 */
struct Aresta {
    int origem;
    int destino;
    double peso; // Atualizado para double
};

/**
 * @brief Representação do grafo
 * @param V Quantidade de vértices/nós
 * @param E Quantidade de arestas
 */
struct Grafo {
    int V;
    long long E;
};

/**
 * @brief Descobrir o chefe de um vértice
 */
int encontraChefe(int vertice, int chefe[]){
    while(chefe[vertice] != vertice){
        vertice = chefe[vertice];
    }
    return vertice;
}

/**
 * @brief Une um vértice com outro
 */
void unir(int verticeU, int verticeV, int chefe[]){
    int chefeU = encontraChefe(verticeU, chefe);
    int chefeV = encontraChefe(verticeV, chefe);

    if(chefeU != chefeV){
        chefe[chefeU] = chefeV; 
    }
}


int main(int argc, char **argv){
    // Para medir o tempo
    struct timeval inicio, fim;
    double tempo_execucao;

    // Inicia o cronômetro
    gettimeofday(&inicio, NULL);

    // Verifica número de parâmetros (Exige arquivo binário e a quantidade de vértices)
    if(argc != 3){
        printf("[ #<- ] Uso: %s <nome_arquivo.bin> <quantidade_vertices>\n", argv[0]);
        return 1;
    }

    // Abre arquivo no modo de leitura binária
    FILE *arquivo = fopen(argv[1], "rb");
    if(arquivo == NULL){
        perror("[ ERRO ] Erro ao abrir o arquivo\n");
        return 1;
    }

    struct Grafo grafo;
    grafo.V = atoi(argv[2]); // Pega os vértices do terminal

    if (grafo.V < 2) {
        printf("[ ERRO ] Quantidade de vertices invalida.\n");
        fclose(arquivo);
        return 1;
    }

    // Descobre as arestas dinamicamente analisando o tamanho físico do arquivo no HD
    // Altere o trecho de medição para:
    fseeko(arquivo, 0, SEEK_END);
    off_t tamanho_bytes = ftello(arquivo);
    grafo.E = tamanho_bytes / sizeof(struct Aresta);
    
    printf("   - Vertices: %d\n", grafo.V);
    printf("   - Arestas : %lld\n", grafo.E);

    int *chefe = (int*) malloc(grafo.V * sizeof(int));
    // Inicialmente todos são chefes de si mesmos
    for(int i = 0; i < grafo.V; i++){
        chefe[i] = i;
    }

    // Bloco de notas usando a STRUCT inteira para não depender de índices locais
    struct Aresta *aresta_mais_barata = (struct Aresta*) malloc(grafo.V * sizeof(struct Aresta));

    int num_arvore = grafo.V;
    double peso_total = 0.0;
    
    struct Aresta* arvore_final = (struct Aresta*) malloc((grafo.V - 1) * sizeof(struct Aresta));
    int conta_arestas_arvore = 0;

    // Carrega 100 mil arestas por vez
    #define TAM_BUFFER 100000 
    struct Aresta *buffer_arestas = (struct Aresta*) malloc(TAM_BUFFER * sizeof(struct Aresta));
    if(buffer_arestas == NULL){
        perror("[ ERRO ] Falha ao alocar buffer de leitura na RAM.\n");
        return 1;
    }

    // Enquanto não formarmos a árvore única
    while(num_arvore > 1){
        // Inicializa a aresta de menor peso com simulação de infinito
        for(int v = 0; v < grafo.V; v++){
            aresta_mais_barata[v].origem = -1;
            aresta_mais_barata[v].destino = -1;
            aresta_mais_barata[v].peso = 999999999999999.0;
        }

        // Retorna a cabeça de leitura para o primeiro byte do arquivo!
        rewind(arquivo);
        long long arestas_processadas = 0;
        long long contador_blocos = 0;

        // --- INÍCIO DA LEITURA DE ALTA VELOCIDADE EM LOTES ---
        while(arestas_processadas < grafo.E){
            
            // Calcula se enchemos o balde inteiro ou pegamos apenas o resto do arquivo
            int ler_agora = (grafo.E - arestas_processadas < TAM_BUFFER) ? (grafo.E - arestas_processadas) : TAM_BUFFER;

            // Suga o bloco do HD direto para a RAM
            fread(buffer_arestas, sizeof(struct Aresta), ler_agora, arquivo);

            // Analisa apenas o lote que está na memória RAM agora
            for(int i = 0; i < ler_agora; i++){
                int chefeU = encontraChefe(buffer_arestas[i].origem, chefe);
                int chefeV = encontraChefe(buffer_arestas[i].destino, chefe);

                // Se as arestas conectam componentes diferentes
                if(chefeU != chefeV){
                    // Compara e guarda a aresta inteira no bloco de notas do Chefe U
                    if(aresta_mais_barata[chefeU].origem == -1 || buffer_arestas[i].peso < aresta_mais_barata[chefeU].peso){
                        aresta_mais_barata[chefeU] = buffer_arestas[i];
                    }
                    // Compara e guarda a aresta inteira no bloco de notas do Chefe V
                    if (aresta_mais_barata[chefeV].origem == -1 || buffer_arestas[i].peso < aresta_mais_barata[chefeV].peso){
                        aresta_mais_barata[chefeV] = buffer_arestas[i];
                    }
                }
            }
            
            arestas_processadas += ler_agora;

            contador_blocos++;
            if (contador_blocos % 500 == 0) {
                double porcentagem = ((double)arestas_processadas / grafo.E) * 100.0;
                // \r faz o cursor voltar pro inicio da linha. fflush garante a exibição imediata
                printf("\r   -> Sub-Redes: %d | Lendo Grafo: %.2f%%", num_arvore, porcentagem);
                fflush(stdout); 
            }
        }

        printf("\n   [+] Iteracao concluida. Fundindo componentes...\n");

        // Para cada componente (Fase de União)
        for(int v = 0; v < grafo.V; v++){
            // Pega a aresta vencedora diretamente do bloco
            struct Aresta aresta_escolhida = aresta_mais_barata[v];

            // É válida? (origem diferente da inicialização -1)
            if(aresta_escolhida.origem != -1){
                int origem = aresta_escolhida.origem;
                int destino = aresta_escolhida.destino;

                int chefeOrigem = encontraChefe(origem, chefe);
                int chefeDestino = encontraChefe(destino, chefe);

                // Une as componentes
                if(chefeOrigem != chefeDestino){
                    peso_total += aresta_escolhida.peso;
                    unir(chefeOrigem, chefeDestino, chefe);
                    num_arvore--;

                    // Adiciona para saída
                    arvore_final[conta_arestas_arvore] = aresta_escolhida;
                    conta_arestas_arvore++;
                }
            }
        }
    }

    // Criação do arquivo de saída
    FILE *arquivo_saida = fopen("arvore_saida.txt", "w");
    if(arquivo_saida == NULL){
        printf("[ ERRO ] Nao foi possivel criar o arquivo de saida.");
        return 1;
    }

    for(int i = 0; i < conta_arestas_arvore; i++){
        fprintf(arquivo_saida, "%d %d %f\n", arvore_final[i].origem, arvore_final[i].destino, arvore_final[i].peso);
    }

    fclose(arquivo_saida);

    // Encerra o cronômetro
    gettimeofday(&fim, NULL);
    tempo_execucao = (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec) / 1000000.0;

    printf("Peso total: %f\n", peso_total);
    printf("Tempo de execucao sequencial: %f segundos\n", tempo_execucao);

    // Liberação final da memória
    free(chefe);
    free(aresta_mais_barata);
    free(buffer_arestas); 
    free(arvore_final);
    
    return 0;
}