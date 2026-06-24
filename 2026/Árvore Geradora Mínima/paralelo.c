#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
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
    double peso;
};

/**
 * @brief Representação do grafo
 * @param V Quantidade de vértices/nós
 * @param E Quantidade de arestas
 * @param arestas Todas as arestas do grafo
 */
struct Grafo {
    int V;
    long long E;
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

    // Pra enviar a struct de 16 bytes
    MPI_Datatype MPI_ARESTA;
    MPI_Type_contiguous(sizeof(struct Aresta), MPI_BYTE, &MPI_ARESTA);
    MPI_Type_commit(&MPI_ARESTA);

    double tempo_inicio = MPI_Wtime();

    // Verifica número de parâmetros
    if(argc != 3){
        if(rank == 0){
            printf("[ #<- ] Uso: %s <nome_arquivo> <quantidade_vertices>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    struct Grafo grafo;

    // Variáveis controle pra distribuição de trabalho
    int fatia, inicio, fim, quantidade_respectiva;

    if(rank == 0){
        grafo.V = atoi(argv[2]);

        if(grafo.V < 2){
            printf("[ ERRO ] Quantidade de vertices invalida");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        FILE *temp = fopen(argv[1], "rb");
        if(temp != NULL){
            fseeko(temp, 0, SEEK_END);
            off_t tamanho_bytes = ftello(temp);
            grafo.E = tamanho_bytes / sizeof(struct Aresta);
            fclose(temp);
        } else {
            perror("[ ERRO ] Mestre nao encontrou o arquivo para medir o tamanho!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        printf("Vertices: %d\n", grafo.V);
        printf("Arestas: %lld\n", grafo.E);

    } 

    // Compartilha V e E 
    MPI_Bcast(&grafo.V, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grafo.E, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Calcula distribuição de trabalho
    fatia = grafo.E / size;
    inicio = rank * fatia;

    if(rank == size - 1){
        fim = grafo.E;
    } else {
        fim = inicio + fatia;
    }
    quantidade_respectiva = fim - inicio;

    // ALocam espaço da sua fatia
    grafo.arestas = (struct Aresta*) malloc(quantidade_respectiva * sizeof(struct Aresta));
    if(grafo.arestas == NULL){
        perror("[ ERRO ] Falha ao alocar memoria para arestas.\n");
        return 1;
    }

    // Leitura e distribui~çao
    if(rank == 0){
        FILE *arquivo = fopen(argv[1], "rb");
        if(arquivo == NULL){
            perror("[ ERRO ] Erro ao abrir o arquivo original\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Lê a fatia e envia pro escravo
        for(int p = 1; p < size; p++){
            // Calcula o tamanho e a posição do escravo
            long long p_inicio = p * fatia;
            long long p_fim = (p == size - 1) ? grafo.E : p_inicio + fatia;
            long long p_qtd = p_fim - p_inicio;

            // ALoca para a fatia dele
            struct Aresta *temp = (struct Aresta*) malloc(p_qtd * sizeof(struct Aresta));

            // Pula pra posição e preenche a fatia
            long long offset = (long long)p_inicio * sizeof(struct Aresta);
            fseeko(arquivo, offset, SEEK_SET);
            fread(temp, sizeof(struct Aresta), p_qtd, arquivo);

            // Envia pro escravo
            MPI_Send(temp, p_qtd, MPI_ARESTA, p, 0, MPI_COMM_WORLD);
            free(temp);
        }

        // O mestre lê sua própria fatia
        fseeko(arquivo, 0, SEEK_SET);
        fread(grafo.arestas, sizeof(struct Aresta), quantidade_respectiva, arquivo);
        
        fclose(arquivo);
        printf("Lido e distribuido\n");
    } else {
        // Escravo fica esperando receber
        MPI_Recv(grafo.arestas, quantidade_respectiva, MPI_ARESTA, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    }


    int *chefe = (int*) malloc(grafo.V * sizeof(int));
    // Inicialmente todos é o chefe de si mesmo
    for(int i = 0; i < grafo.V; i++){
        chefe[i] = i;
    }

    struct Aresta *aresta_mais_barata = (struct Aresta*) malloc(grafo.V * sizeof(struct Aresta));

    // Inicialmente o número de árvore é o mesmo que o número de vértices
    int num_arvore = grafo.V;

    double peso_total = 0;
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
            aresta_mais_barata[v].peso = 999999999999999.0; // Simulação do infinito
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
        MPI_Gather(aresta_mais_barata, grafo.V, MPI_ARESTA, todos_blocos, grafo.V, MPI_ARESTA, 0, MPI_COMM_WORLD);
        
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
        MPI_Bcast(aresta_mais_barata, grafo.V, MPI_ARESTA, 0, MPI_COMM_WORLD);
    
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
        printf("Peso total: %.12f\n", peso_total);

        // Criação do arquivo de saída
        FILE *arquivo_saida = fopen("arvore_saida.txt", "w");
        if(arquivo_saida != NULL){
            for(int i = 0; i < conta_arestas_arvore; i++){
                fprintf(arquivo_saida, "%d %d %.12f\n", arvore_final[i].origem, arvore_final[i].destino, arvore_final[i].peso);
            }

            fclose(arquivo_saida);
        } else {
            printf("[ ERRO ] Nao fo possivel criar o arquivo de saida.");
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