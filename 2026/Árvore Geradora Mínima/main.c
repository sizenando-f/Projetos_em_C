#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int peso;
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
    while(fscanf(arquivo, "%d %d %d", &grafo.arestas[i].origem, &grafo.arestas[i].destino, &grafo.arestas[i].peso) == 3){
        i++;
    }

    printf("%d arestas carregadas na memória.\n", i);
    if(grafo.E == i){
        printf("A quantidade de arestas lidas eh a mesma da informada\n");
    } else {
        printf("A quantidade de arestas lidas nao condiz com a informada\n");
    }

    fclose(arquivo);

    int *chefe = (int*) malloc(grafo.V * sizeof(int));
    for(int i = 0; i < grafo.V; i++){
        chefe[i] = i;
    }

    int *aresta_mais_barata = (int*) malloc(grafo.V * sizeof(int));

    // Inicialmente o número de árvore é o mesmo que o número de vértices
    int num_arvore = grafo.V;

    while(num_arvore > 1){
        memset(aresta_mais_barata, -1, grafo.V * sizeof(int)); // Inicializa com -1 indicando vazio

        // Verifica quais arestas possui menor peso
        for(int i = 0; i < grafo.E; i++){
            int chefeU = encontraChefe(grafo.arestas[i].origem, chefe);
            int chefeV = encontraChefe(grafo.arestas[i].destino, chefe);

            int aresta_barata_U = aresta_mais_barata[chefeU];
            int aresta_barata_V = aresta_mais_barata[chefeV];

            if(chefeU != chefeV){
                if(aresta_barata_U == -1 || grafo.arestas[i].peso < grafo.arestas[aresta_barata_U].peso){
                    aresta_mais_barata[chefeU] = i;
                }
                if (aresta_barata_V == -1 || grafo.arestas[i].peso < grafo.arestas[aresta_barata_V].peso){
                    aresta_mais_barata[chefeV] = i;
                }
            }
        }

        for(int i = 0; i < grafo.V; i++){
            
        }

    }

    free(chefe);
    free(aresta_mais_barata);
    free(grafo.arestas);
    return 0;
}