#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/**
 * @brief Uma aresta do grafo
 * 
 * @param origem De onde a aresta sai
 * @param destino Aonde a aresta chega
 * @param peso Peso da aresta
 */
struct Aresta {
    int origem;
    int destino;
    long long int peso;
};

/**
 * @brief Representação do grafo
 * @param V Quantidade de vértices/nós
 * @param E Quantidade de arestas
 * @param arestas Todas as arestas do grafo
 */
struct Grafo {
    int V;
    int E;
    struct Aresta* arestas;
};

/**
 * @brief Descobrir o chefe de um vértice
 * 
 * Escala iterativamente pelos vértices em busca do seu chefe,
 * ele possuir um chefe indica que está no grupo de tal chefe,
 * caso contrário ele está sozinho ou o grupo encerra nele
 * 
 * @param vertice Vértice a qual deseja descobrir o chefe
 * @param chefe Vetor de chefes
 */
int encontraChefe(int vertice, int chefe[]){
    while(chefe[vertice] != vertice){
        vertice = chefe[vertice];
    }
    return vertice;
}

/**
 * @brief Une um vértice com outro
 * 
 * Verifica ambos os chefes e caso forem diferentes realiza a união,
 * caso contrário ambos estão no mesmo grupo
 * 
 * @param verticeU Vértice u
 * @param verticeV Vértice v
 * @param chefe Vetor de chefes
 */
void unir(int verticeU, int verticeV, int chefe[]){
    int chefeU = encontraChefe(verticeU, chefe);
    int chefeV = encontraChefe(verticeV, chefe);

    if(chefeU != chefeV){
        chefe[chefeU] = chefeV; 
    }
}


int main(int argc, char **argv){
    // Verifica número de parâmetros
    if(argc != 2){
        printf("[ #<- ] Uso: %s <nome_arquivo>\n", argv[0]);
        return 1;
    }

    // Abre arquivo
    FILE *arquivo = fopen(argv[1], "r");
    if(arquivo == NULL){
        perror("[ ERRO ] Erro ao abrir o arquivo\n");
        return 1;
    }

    char buffer[256];
    struct Grafo grafo;

    int primeiraLinha = 0;
    while(fgets(buffer, sizeof(buffer), arquivo)){
        if(!primeiraLinha){
            grafo.V = strtol(buffer, NULL, 10); // Armazena os vértices
            primeiraLinha++;    // Para saber qual linha está
            continue;
        }
        printf("Vertices: %d\n", grafo.V);

        grafo.E = strtol(buffer, NULL, 10); // Armazena as arestas
        printf("Arestas: %d\n", grafo.E);

        grafo.arestas = (struct Aresta*) malloc(grafo.E * sizeof(struct Aresta)); // Aloca arestas
        if(grafo.arestas == NULL){
            perror("[ ERRO ] Falha ao alocar memoria para arestas.\n");
            return 1;
        }
        break;
    }

    // Armazena todas as arestas do grafo
    int i = 0;
    while(fscanf(arquivo, "%d %d %lld", &grafo.arestas[i].origem, &grafo.arestas[i].destino, &grafo.arestas[i].peso) == 3){
        i++;
    }

    printf("%d arestas carregadas na memoria.\n", i);
    if(grafo.E == i){
        printf("A quantidade de arestas lidas eh a mesma da informada\n");
    } else {
        printf("A quantidade de arestas lidas nao condiz com a informada\n");
    }

    fclose(arquivo);

    int *chefe = (int*) malloc(grafo.V * sizeof(int));
    // Inicialmente todos é o chefe de si mesmo
    for(int i = 0; i < grafo.V; i++){
        chefe[i] = i;
    }

    int *aresta_mais_barata = (int*) malloc(grafo.V * sizeof(int));

    // Inicialmente o número de árvore é o mesmo que o número de vértices
    int num_arvore = grafo.V;

    long long int peso_total = 0;

    // Para medir o tempo
    struct timeval inicio, fim;
    double tempo_execucao;

    // Inicia o cronômetro
    gettimeofday(&inicio, NULL);

    // Enquanto não concluído
    while(num_arvore > 1){
        // Inicializa a aresta de menor peso para cada componente como -1 (Nenhuma)
        memset(aresta_mais_barata, -1, grafo.V * sizeof(int)); // Inicializa com -1 indicando vazio

        // Para cada aresta uv em E
        for(int i = 0; i < grafo.E; i++){
            int chefeU = encontraChefe(grafo.arestas[i].origem, chefe);
            int chefeV = encontraChefe(grafo.arestas[i].destino, chefe);

            int aresta_barata_U = aresta_mais_barata[chefeU];
            int aresta_barata_V = aresta_mais_barata[chefeV];

            // Onde uv estão em componentes diferentes
            if(chefeU != chefeV){
                // Deixe wx ser a aresta de menor peso
                if(aresta_barata_U == -1 || grafo.arestas[i].peso < grafo.arestas[aresta_barata_U].peso){
                    aresta_mais_barata[chefeU] = i;
                }
                if (aresta_barata_V == -1 || grafo.arestas[i].peso < grafo.arestas[aresta_barata_V].peso){
                    aresta_mais_barata[chefeV] = i;
                }
            }
        }

        // Para cada componente
        for(int v = 0; v < grafo.V; v++){
            // Cuja aresta de menor peso
            int indice_aresta = aresta_mais_barata[v];

            // É -1 ("nenhuma")
            if(indice_aresta != -1){
                int origem = grafo.arestas[indice_aresta].origem;
                int destino = grafo.arestas[indice_aresta].destino;

                int chefeOrigem = encontraChefe(origem, chefe);
                int chefeDestino = encontraChefe(destino, chefe);

                // Adicione sua aresta de menor peso
                if(chefeOrigem != chefeDestino){
                    peso_total += grafo.arestas[indice_aresta].peso;
                    unir(chefeOrigem, chefeDestino, chefe);
                    num_arvore--;
                }
            }
        }
    }

    // Encerra o cronômetro
    gettimeofday(&fim, NULL);
    tempo_execucao = (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec) / 1000000.0;

    printf("Peso total: %lld\n", peso_total);
    printf("Tempo de execucao sequencial: %f segundos\n", tempo_execucao);

    free(chefe);
    free(aresta_mais_barata);
    free(grafo.arestas);
    return 0;
}