/**
 * @file main.c
 * @brief Programa que lê um vetor de inteiros de um arquivo binário e calcula a soma dos elementos usando processos filhos.
 * 
 * Este programa lê um vetor de inteiros de um arquivo binário, divide o trabalho de somar os elementos entre vários processos filhos, e utiliza memória compartilhada para armazenar o resultado total da soma. O número de processos é especificado pelo usuário.
 *
 * @author Sizenando S. França - 50575
 * @date 06-05-2025
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>


/**
 * @brief Função principal que executa o programa.
 *
 * @param argc Número de argumentos da linha de comando.
 * @param argv Array de strings contendo os argumentos da linha de comando.
 *
 * @details O primeiro argumento deve ser o nome do arquivo binário contendo os valores inteiros, e o segundo argumento deve ser o número de processos a serem criados para calcular a soma dos elementos do vetor.
 *
 * @return 0 se o programa for executado com sucesso, -1 em caso de erro.
*/
int main(int argc, char *argv[]) {
    FILE *fp;   // Ponteiro para o arquivo
    int tamanho;    // Variável para armazenar o tamanho do vetor

    // Verifica se o número de argumentos está correto
    if (argc != 3){
        printf("[ ? ] Uso: %s <nome_arquivo> <quant_processos>\n", argv[0]);
        return -1;
    }
    // Armazena a quantidade de processos que o usuário deseja
    int processos = atoi(argv[2]);

    // Verifica se o número de processos é válido
    if(processos <= 0){
        printf("[ #<- ] O número de processos deve ser maior que 0\n");
        return -1;
    }

    // Abre o arquivo binário
    fp = fopen(argv[1], "rb");

    // Verifica se o arquivo foi aberto corretamente
    if (fp == NULL) {
        printf("[ -># ] Erro ao abrir o arquivo\n");
        return -1;
    }

    struct stat info;   // Estrutura para armazenar informações sobre o arquivo
    stat(argv[1], &info); // Obtém informações sobre o arquivo
    tamanho = info.st_size / sizeof(int); // Calcula o tamanho do vetor
    // Verifica se a quantidade de processos é maior que o tamanho do vetor
    if(processos > tamanho){
        printf("[ #<- ] O número de processos é maior que o número de elementos, limitando o número de processos para %d\n", tamanho);
        processos = tamanho;    // Limita o número de processos para o tamanho do vetor
    }

    int *vetor = (int*) malloc((size_t)tamanho * sizeof(int)); // Aloca memória para o vetor
    // Verifica se a alocação de memória foi bem-sucedida
    if(vetor == NULL){
        printf("[ -># ] Erro ao alocar memória para o vetor\n");
        fclose(fp); // Fecha o arquivo
        return -1;
    }

    // Lê o vetor do arquivo
    if(fread(vetor, sizeof(int), (size_t)tamanho, fp) != (size_t)tamanho){
        printf("[ -># ] Erro ao ler o vetor do arquivo\n");
        free(vetor);    // Libera a memória alocada
        fclose(fp); // Fecha o arquivo
        return -1;
    }

    fclose(fp); // Fecha o arquivo

    int shmid = shmget(IPC_PRIVATE,sizeof(int), IPC_CREAT | 0666);  // Cria a memória compartilhada
    // Verifica se a memória compartilhada foi criada corretamente
    if(shmid < 0){
        printf("[ -># ] Erro ao criar a memória compartilhada\n");
        return -1;
    }

    int *total = (int*) shmat(shmid, NULL, 0);  // Anexa a memória compartilhada

    *total = 0; // Inicializa a soma total

    for(int i = 0; i < processos; i++){
        // Cria um processo filho
        if(!fork()){
            int base = tamanho / processos; // Para saber quantos números cada processo vai somar
            int resto = tamanho % processos; // Para saber quantos números sobram para o último processo
            int inicio = i * base + (i < resto ? i : resto);  // Para saber o índice inicial do vetor que cada processo vai somar, se i < resto, o processo vai somar i números a mais
            int fim = inicio + base + (i < resto ? 1 : 0); // Para saber o índice final do vetor que cada processo vai somar, se i < resto, o processo vai somar 1 número a mais
            int soma = 0;   // Para armazenar a soma do processo

            // Realiza a soma dos números do vetor no intervalo [inicio, fim)
            for(int j = inicio; j < fim; j++){
                soma += vetor[j];
            }

            *total += soma; // Atualiza a soma total

            shmdt(total);   // Desanexa a memória compartilhada
            exit(0);    // Encerra o processo filho
        }
    }

    // Espera todos os filhos terminarem
    for(int i = 0; i < processos; i++){
        wait(NULL);
    }

    printf("[ -> ] Soma total: %d\n", *total);

    free(vetor);    // Libera o vetor alocado
    shmdt(total);   // Desanexa a memória compartilhada
    shmctl(shmid, IPC_RMID, NULL);  // Remove a memória compartilhada

    return 0;
}