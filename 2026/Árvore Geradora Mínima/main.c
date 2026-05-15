#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Uma aresta do grafo
 * origem = De onde a aresta sai
 * destino = Aonde a aresta chega
 * peso = Peso da aresta
 */
struct Aresta {
    int origem;
    int destino;
    int peso;
};

/**
 * Representação do grafo
 * V = Quantidade de vértices/nós
 * E = Quantidade de arestas
 * arestas = Todas as arestas do grafo
 */
struct Grafo {
    int V;
    int E;
    struct Aresta* arestas;
};


int main(int argc, char **argv){
    // Verifica número de parâmetros
    if(argc != 2){
        printf("[ #<- ] Uso: %s <nome_arquivo>", argv[0]);
        return 1;
    }

    // Abre arquivo
    FILE *arquivo = fopen(argv[1], "r");
    if(arquivo == NULL){
        perror("[ ERRO ] Erro ao abrir o arquivo");
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
        printf("Vertices: %d", grafo.V);

        grafo.E = strtol(buffer, NULL, 10); // Armazena as arestas
        printf("Arestas: %d", grafo.E);

        grafo.arestas = (struct Aresta*) malloc(grafo.E * sizeof(struct Aresta)); // Aloca arestas
        if(grafo.arestas == NULL){
            perror("[ ERRO ] Falha ao alocar memoria para arestas.");
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
    free(grafo.arestas);
    return 0;
}