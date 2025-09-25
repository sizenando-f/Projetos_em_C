#include <stdio.h>     // Entrada, saída, leitura de arquivo
#include <semaphore.h> // Semáforos
#include <pthread.h>   // Threads
#include <stdlib.h>    // Para alocação
#include <unistd.h>

typedef enum {
    CRIADO,
    REMOVIDO_PAR,    // Removido por ser par
    CHECK_PAR,       // Passou no teste de paridade
    REMOVIDO_PRIMO,  // Removido por não ser primo
    CHECK_PRIMO,     // É primo
    IMPRESSO         // Já foi impresso
} Estado;

typedef struct No {
    int valor;
    struct No* proximo;
    sem_t trava_no;
    Estado estado;
} No;

typedef struct Lista {
    No *inicio;
    No *fim;
    sem_t trava_lista;
} Lista;

Lista lista;

void inicializa_lista(Lista *lista){
    lista->inicio = NULL;
    lista->fim = NULL;
    sem_init(&lista->trava_lista, 0, 1);
}

void adiciona_no(Lista *lista, int valor){
    No *novoNo = (No*) malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->proximo = NULL;
    sem_init(&novoNo->trava_no, 0, 1);
    novoNo->estado = CRIADO;

    sem_wait(&lista->trava_lista);

    if(lista->inicio == NULL){
        lista->inicio = novoNo;
        lista->fim = novoNo;
    } else {
        lista->fim->proximo = novoNo;
        lista->fim = novoNo;
    }

    sem_post(&lista->trava_lista);
}

int verifica_primo(int valor)
{
    if (valor <= 1) return 0;
    if (valor == 2) return 1;
    if (valor % 2 == 0) return 0;
    
    for (int i = 3; i * i <= valor; i += 2)
    {
        if (valor % i == 0) return 0;
    }
    return 1;
}

void *remove_pares(void *arg) {
    int sentinela_encontrado = 0;
    
    while (!sentinela_encontrado) {
        No *atual = lista.inicio;
        
        while (atual != NULL && !sentinela_encontrado) {
            sem_wait(&atual->trava_no);
            
            if (atual->valor == -1) {
                sentinela_encontrado = 1;
                sem_post(&atual->trava_no);
                break;
            }
            
            if (atual->estado == CRIADO) {
                if ((atual->valor % 2 == 0) && (atual->valor > 2)) {
                    atual->estado = REMOVIDO_PAR;
                } else {
                    atual->estado = CHECK_PAR;
                }
            }
            
            No *proximo = atual->proximo;
            sem_post(&atual->trava_no);
            atual = proximo;
        }
        
        if (!sentinela_encontrado) {
            usleep(1000);
        }
    }
    return NULL;
}

void *filtra_primos(void *arg){
    int sentinela_encontrado = 0;
    
    while (!sentinela_encontrado){
        No *atual = lista.inicio;
        
        while(atual != NULL && !sentinela_encontrado){
            sem_wait(&atual->trava_no);
            
            if (atual->valor == -1) {
                sentinela_encontrado = 1;
                sem_post(&atual->trava_no);
                break;
            }
            
            if(atual->estado == CHECK_PAR){
                if(verifica_primo(atual->valor)){
                    atual->estado = CHECK_PRIMO;
                } else {
                    atual->estado = REMOVIDO_PRIMO;
                }
            }
            
            No *proximo = atual->proximo;
            sem_post(&atual->trava_no);
            atual = proximo;
        }
        
        if (!sentinela_encontrado) {
            usleep(1000);
        }
    }
    return NULL;
}

void *imprime_primos(void *arg){
    int sentinela_encontrado = 0;
    
    while (!sentinela_encontrado){
        No *atual = lista.inicio;
        
        while(atual != NULL && !sentinela_encontrado){
            sem_wait(&atual->trava_no);
            
            if (atual->valor == -1) {
                sentinela_encontrado = 1;
                sem_post(&atual->trava_no);
                break;
            }
            
            if(atual->estado == CHECK_PRIMO){
                printf("%d ", atual->valor);
                fflush(stdout);
                atual->estado = IMPRESSO;
            }
            
            No *proximo = atual->proximo;
            sem_post(&atual->trava_no);
            atual = proximo;
        }
        
        if (!sentinela_encontrado) {
            usleep(1000);
        }
    }
    
    printf("\n"); // Quebra de linha após imprimir todos os primos
    return NULL;
}

void libera_memoria(Lista *lista){
    No *atual;

    sem_wait(&lista->trava_lista);
    atual = lista->inicio;
    lista->inicio = NULL;
    lista->fim = NULL;
    sem_post(&lista->trava_lista);

    while(atual != NULL){
        No *proximo = atual->proximo;
        sem_destroy(&atual->trava_no);
        free(atual);
        atual = proximo;
    }

    sem_destroy(&lista->trava_lista);
}

int main(){
    pthread_t thread_impares, thread_primos, thread_imprime;

    inicializa_lista(&lista);

    // Cria as threads
    pthread_create(&thread_impares, NULL, remove_pares, NULL);
    pthread_create(&thread_primos, NULL, filtra_primos, NULL);
    pthread_create(&thread_imprime, NULL, imprime_primos, NULL);

    // Lê o arquivo
    FILE *arquivo = fopen("in.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo!");
        exit(EXIT_FAILURE);
    }

    int valor;
    while (fscanf(arquivo, "%d", &valor) != EOF)
    {
        adiciona_no(&lista, valor);
    }
    fclose(arquivo);

    // Adiciona o sentinela
    adiciona_no(&lista, -1);

    // Espera todas as threads terminarem
    pthread_join(thread_impares, NULL);
    pthread_join(thread_primos, NULL);
    pthread_join(thread_imprime, NULL);

    libera_memoria(&lista);

    printf("FIM\n");
    return 0;
}