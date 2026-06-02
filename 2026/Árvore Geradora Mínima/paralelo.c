#include <stdio.h>
// #include <stdlib.h>
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
// struct Aresta {
//     int origem;
//     int destino;
//     long long int peso;
// };

/**
 * @brief Representação do grafo
 * @param V Quantidade de vértices/nós
 * @param E Quantidade de arestas
 * @param arestas Todas as arestas do grafo
 */
// struct Grafo {
//     int V;
//     int E;
//     struct Aresta* arestas;
// };

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
// int encontraChefe(int vertice, int chefe[]){
//     while(chefe[vertice] != vertice){
//         vertice = chefe[vertice];
//     }
//     return vertice;
// }

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
// void unir(int verticeU, int verticeV, int chefe[]){
//     int chefeU = encontraChefe(verticeU, chefe);
//     int chefeV = encontraChefe(verticeV, chefe);

//     if(chefeU != chefeV){
//         chefe[chefeU] = chefeV; 
//     }
// }


int main(int argc, char **argv){
    int rank, size;

    // Inicializa o sistema paralelo
    MPI_Init(&argc, &argv);

    // Descobre quantas máquinas estão rodando
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Descobre o rank da máquina atual
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        printf("Sou a máquina %d de %d. Sou a máquina MESTRE\n", rank, size);
    } else {
        printf("Sou a máquina %d de %d. Sou apenas um operário\n", rank, size);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();
    return 0;
}