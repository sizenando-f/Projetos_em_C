#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <sys/time.h>
#include <mpi.h>

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
    int rank, size;

    // Inicializa o sistema paralelo
    MPI_Init(&argc, &argv);

    // Descobre quantas máquinas estão rodando
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Descobre o rank da máquina atual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double tempo_inicio = MPI_Wtime();

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

    char buffer[256]; // Prepara buffer pra receber linha do arquivo
    struct Grafo grafo;

    // Variáveis controle pra distribuição de trabalho
    int fatia, inicio, fim, quantidade_respectiva;

    // Leitura das duas primeiras linhas
    int primeiraLinha = 0;
    while(fgets(buffer, sizeof(buffer), arquivo)){
        // Se for a primeira linha, pega os vértices
        if(!primeiraLinha){
            grafo.V = strtol(buffer, NULL, 10); // Armazena os vértices
            primeiraLinha++;    // Para saber qual linha está
            continue;
        }

        // Segunda linha pega as arestas
        grafo.E = strtol(buffer, NULL, 10); // Armazena as arestas
        if(rank == 0){
            printf("Vertices: %d\n", grafo.V);
            printf("Arestas: %d\n", grafo.E);
        }
        
        fatia = grafo.E / size; // Pra saber quantas arestas cada máquina vai ter
        inicio = rank * fatia;  // Rank é a posição da máquina multiplicado pela fatia pra saber qual intervalo começar
        
        // Se for a última máquina, pega as arestas até o fim
        if(rank == size - 1){
            fim = grafo.E;
        // Senão o fim vai ser o ínicio respectivo + o limite dela
        } else { 
            fim = inicio + fatia;
        }

        quantidade_respectiva = fim - inicio;

        // Aloca apenas a quantidade de arestas dividida pela quantidade das máquinas + 1 por folga
        grafo.arestas = (struct Aresta*) malloc(quantidade_respectiva * sizeof(struct Aresta)); // Aloca arestas
        if(grafo.arestas == NULL){
            perror("[ ERRO ] Falha ao alocar memoria para arestas.\n");
            return 1;
        }
        break;
    }

    // Para pular o ponteiro do arquivo na posição correta
    for(int i = 0; i < inicio; i++){
        fgets(buffer, sizeof(buffer), arquivo);
    }

    for(int i = 0; i < quantidade_respectiva; i++){
        fscanf(arquivo, "%d %d %lld", &grafo.arestas[i].origem, &grafo.arestas[i].destino, &grafo.arestas[i].peso);
    }

    fclose(arquivo);

    int *chefe = (int*) malloc(grafo.V * sizeof(int));
    // Inicialmente todos é o chefe de si mesmo
    for(int i = 0; i < grafo.V; i++){
        chefe[i] = i;
    }

    struct Aresta *aresta_mais_barata = (struct Aresta*) malloc(grafo.V * sizeof(struct Aresta));

    // Inicialmente o número de árvore é o mesmo que o número de vértices
    int num_arvore = grafo.V;

    long long int peso_total = 0;
    struct Aresta* arvore_final = (struct Aresta*) malloc((grafo.V - 1) * sizeof(struct Aresta));
    int conta_arestas_arvore = 0;

    struct Aresta *todos_blocos = NULL;
    // Aloca apenas se for o mestre que é responsável por fundir
    if(rank == 0){
        todos_blocos = (struct Aresta*) malloc(size * grafo.V * sizeof(struct Aresta));
    }

    // Enquanto não concluído
    while(num_arvore > 1){
        // Inicializa a aresta de menor peso para cada componente como "infinito"
        for(int v = 0; v < grafo.V; v++){
            aresta_mais_barata[v].origem = -1;
            aresta_mais_barata[v].destino = -1;
            aresta_mais_barata[v].peso = 999999999999999LL; // Simulação do infinito
        }

        // Para cada aresta uv em E
        for(int i = 0; i < quantidade_respectiva; i++){
            int chefeU = encontraChefe(grafo.arestas[i].origem, chefe);
            int chefeV = encontraChefe(grafo.arestas[i].destino, chefe);

            // Onde uv estão em componentes diferentes
            if(chefeU != chefeV){
                // Deixe wx ser a aresta de menor peso
                if(aresta_mais_barata[chefeU].origem == -1 || grafo.arestas[i].peso < aresta_mais_barata[chefeU].peso){
                    aresta_mais_barata[chefeU] = grafo.arestas[i];
                }
                if (aresta_mais_barata[chefeV].origem == -1 || grafo.arestas[i].peso < aresta_mais_barata[chefeV].peso){
                    aresta_mais_barata[chefeV] = grafo.arestas[i];
                }
            }
        }

        // Gather onde todos enviam as suas arestas mais baratas para o mestre
        MPI_Gather(aresta_mais_barata, grafo.V * sizeof(struct Aresta), MPI_BYTE, todos_blocos, grafo.V * sizeof(struct Aresta), MPI_BYTE, 0, MPI_COMM_WORLD);
        
        // Se for o mestre
        if(rank == 0){
            for(int v = 0; v < grafo.V; v++){
                // Pega a melhor aresta do mestre
                struct Aresta melhor_aresta = aresta_mais_barata[v];

                // Compara com o dos outros
                for(int p = 1; p < size; p++){
                    // PEga o vértice que veio do operário
                    struct Aresta aresta_operario = todos_blocos[p * grafo.V + v];

                    // Verifica se é válida
                    if(aresta_operario.origem != -1){
                        // Se o mestre não encontrou nada ou peso for menor
                        if(melhor_aresta.origem == -1 || aresta_operario.peso < melhor_aresta.peso){
                            melhor_aresta = aresta_operario;
                        }
                    }
                }
                // Atualiza melhor aresta
                aresta_mais_barata[v] = melhor_aresta;
            }
        }

        // Retorna o feedback para as outras máquinas
        MPI_Bcast(aresta_mais_barata, grafo.V * sizeof(struct Aresta), MPI_BYTE, 0, MPI_COMM_WORLD);
    
        // Para cada componente
        for(int v = 0; v < grafo.V; v++){
            // Cuja aresta de menor peso
            struct Aresta aresta_escolhida = aresta_mais_barata[v];

            // É -1 ("nenhuma")
            if(aresta_escolhida.origem != -1){
                int origem = aresta_escolhida.origem;
                int destino = aresta_escolhida.destino;

                int chefeOrigem = encontraChefe(origem, chefe);
                int chefeDestino = encontraChefe(destino, chefe);

                // Adicione sua aresta de menor peso
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

    // Deixar apenas o mestre imprimir e escrever no arquivo
    if(rank == 0){
        printf("Peso total: %lld\n", peso_total);

        // Criação do arquivo de saída
        FILE *arquivo_saida = fopen("arvore_saida.txt", "w");
        if(arquivo_saida != NULL){
            for(int i = 0; i < conta_arestas_arvore; i++){
                fprintf(arquivo_saida, "%d %d %lld\n", arvore_final[i].origem, arvore_final[i].destino, arvore_final[i].peso);
            }

            fclose(arquivo_saida);
        } else {
            printf("[ ERRO ] Nao fo possivel criar o arquivo de saida.");
            return 1;
        }
    }


    if(rank == 0) free(todos_blocos);

    free(chefe);
    free(aresta_mais_barata);
    free(grafo.arestas);
    free(arvore_final);

    double tempo_fim = MPI_Wtime();
    printf("Tempo de execucao: %f, maquina %d\n", tempo_fim - tempo_inicio, rank);

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}