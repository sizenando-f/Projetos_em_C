// Lembrar de compilar usando -pthread

/**
 * @file main.c
 * @brief Programa que lê um vetor de números inteiros de um arquivo binário e calcula a soma dos elementos usando threads.
 * 
 * O programa divide o vetor em partes iguais entre as threads, cada uma responsável por calcular a soma de sua parte.
 * A soma total é obtida somando os resultados de cada thread.
 *
 * @author Sizenando S. França 50575
 * @date 06-05-2025
*/


#include <stddef.h> // Para usar size_t
#include <stdio.h>  // Para usar printf
#include <stdlib.h> // Para usar malloc, free, atoi
#include <pthread.h>    // Para usar pthread
#include <sys/stat.h>   // Para usar stat
#include <unistd.h> // Para usar close


int tamanho;    // Tamanho do vetor de números
int num_threads;    // Número de threads
int *resultado;   // Vetor para armazenar os resultados de cada thread
int *numeros;   // Vetor de números a serem somados

/**
 * @brief Função que calcula a soma de uma parte do vetor de números.
 *
 * Cada thread calcula a soma de uma parte do vetor e armazena o resultado em um vetor de resultados.
 *
 * @param indice Ponteiro para o índice da thread.
 * @return NULL
*/
void* soma(void* indice){
    int base = tamanho / num_threads;   // Tamanho da parte do vetor que cada thread irá somar
    int resto = tamanho % num_threads;  // Resto da divisão do tamanho do vetor pelo número de threads
    int inicio = *(int*)indice * base + (*(int*)indice < resto ? *(int*)indice : resto);    // Início da parte do vetor que a thread irá somar
    int fim = inicio + base + (*(int*)indice < resto ? 1 : 0);   // Fim da parte do vetor que a thread irá somar
    resultado[*(int*)indice] = 0;   // Inicializa o resultado da thread como 0

    // Calcula a soma da parte do vetor que a thread irá somar
    for(int i = inicio; i < fim; i++){
        resultado[*(int*)indice] += numeros[i];   // Soma os elementos do vetor
    }
    return NULL;
}

int main(int argc, char* argv[]){
    // Verifica se o número de argumentos é correto
    if (argc != 3){
        printf("[ ? ] Uso: %s <nome_arquivo> <quant_threads>\n", argv[0]);
        return -1;
    }

    num_threads = atoi(argv[2]);    // Lê o número de threads a partir do terceiro argumento
    
    // Verifica se o número de threads é válido
    if(num_threads <= 0){
        printf("[ #<- ] O número de threads deve ser maior que 0\n");
        return -1;
    }

    FILE *fp = fopen(argv[1], "rb");    // Abre o arquivo binário para leitura

    // Verifica se o arquivo foi aberto corretamente
    if (fp == NULL) {
        printf("[ -># ] Erro ao abrir o arquivo\n");
        return -1;
    }
 
    // Verifica se o tamanho de threads requisitada pelo usuário é maior que o tamanho do vetor de números
    if(num_threads > tamanho){
        printf("[ #<- ] O número de threads é maior que o número de elementos, limitando o número de threads para %d\n", tamanho);
        num_threads = tamanho;    // Limita o número de threads para o tamanho do vetor
    }

    numeros = (int*) malloc((size_t)tamanho * sizeof(int));     // Aloca memória para o vetor de números

    // Verifica se a alocação de memória foi bem-sucedida
    if(numeros == NULL){
        printf("[ -># ] Erro ao alocar memória para o vetor de números\n");
        fclose(fp); // Fecha o arquivo
        return -1;
    }

    resultado = (int*) malloc((size_t)num_threads * sizeof(int));   // Aloca memória para o vetor de resultados de cada thread
    
    // Verifica se a alocação de memória foi bem-sucedida
    if(resultado == NULL){
        printf("[ -># ] Erro ao alocar memória para o vetor de resultados\n");
        fclose(fp); // Fecha o arquivo
        return -1;
    }

    // Lê os números do arquivo e armazena no vetor de números
    if(fread(numeros, sizeof(int), (size_t)tamanho, fp) != (size_t)tamanho){
        printf("[ -># ] Erro ao ler o vetor do arquivo\n");
        free(numeros);    // Libera a memória alocada
        fclose(fp); // Fecha o arquivo
        return -1;
    }

    fclose(fp); // Fecha o arquivo


    pthread_t *thread = (pthread_t*) malloc((size_t)num_threads * sizeof(pthread_t));   // Aloca memória para as threads

    // Verifica se a alocação de memória foi bem-sucedida
    if(thread == NULL){
        printf("[ -># ] Erro ao alocar memória para os threads\n");
        free(numeros);    // Libera a memória do vetor de números
        free(resultado);    // Libera a memória do vetor de resultados
        return -1;
    }

    int *indices = (int*) malloc((size_t)num_threads * sizeof(int));    // Aloca memória para os índices das threads

    // Verifica se a alocação de memória foi bem-sucedida
    if(indices == NULL){
        printf("[ -># ] Erro ao alocar memória para os índices\n");
        free(numeros);    // Libera a memória do vetor de números
        free(thread);     // Libera a memória do vetor das threads
        free(resultado);  // Libera a memória do vetor de resultados
        return -1;
    } 

    // Cria as threads
    for(int i = 0; i < num_threads; i++){
        indices[i] = i;     // Armazena o índice da thread
        pthread_create(&thread[i], NULL, soma, &indices[i]);    // Cria a thread
    }

    // Espera todas as threads terminarem
    for(int i = 0; i < num_threads; i++){
        pthread_join(thread[i], NULL);   // Espera a thread terminar
    }

    int total = 0;  // Variável para armazenar a soma total

    for(int i = 0; i < num_threads; i++){
        total += resultado[i];  // Soma os resultados de cada thread
    }
    printf("[ -> ] Total: %d\n", total);

    // Libera a memória alocada
    free(numeros);    // Libera a memória do vetor de números
    free(resultado);  // Libera a memória do vetor de resultados
    free(thread);     // Libera a memória do vetor das threads
    free(indices);    // Libera a memória dos índices
    return 0;
}