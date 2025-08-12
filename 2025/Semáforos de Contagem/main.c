#include <stdio.h>      // Entrada, saída, leitura de arquivo
#include <semaphore.h>  // Semáforos
#include <pthread.h>    // Threads

// Estrutura do nó
typedef struct{
    int valor;
    No* proximo;
} No;

// Estrutura da lista
typedef struct {
    No* inicio;
    No* fim;
} Lista;

// Listas que serão usadas
Lista L1, L2, L3;
// Semáforos para controlar acesso a lista encadeada
sem_t mutex1, mutex2, mute3;
// Semáforos para contar quantos valores estão disponíveis
sem_t cont1, cont2, cont3;

/**
 * @brief Adiciona novo nó numa lista
 * 
 * Adiciona um novo nó com o valor desejado na lista simplesmente encadeada
 * 
 * @param lista Ponteiro para início da lista
 * @param valor Valor do nó
 */
void adiciona_no(Lista **lista, int valor){
    // Aloca novo nó e inicializa valores
    No *novoNo = (No*) malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->proximo = NULL;

    // Adiciona no ínicio se não houver nós
    if((*lista)->inicio == NULL){
        (*lista)->inicio = novoNo;
        (*lista)->fim = novoNo;

    // Se já existir nó na (*lista)
    } else {
        (*lista)->fim->proximo = novoNo;
        (*lista)->fim = novoNo;
    }
}

/**
 * @brief Remove primeiro nó da lista
 * 
 * Realiza a remoção e retorno do primeiro nó da lista
 * 
 * @param lista Ponteiro para inicio da lista
 * 
 * @return Valor removido
 */
int remove_no(Lista **lista){
    // Verifica se lista está vazia
    if((*lista)->inicio == NULL){
        return -1;
    } else {
        int valor;
        No *temp = (*lista)->inicio;

        // Pega valor e passa e avança para próxmimo nó
        valor = temp->valor;
        (*lista)->inicio = (*lista)->inicio->proximo;

        // Se lista for vazia, define final como nulo
        if((*lista)->inicio == NULL){
            (*lista)->fim = NULL;
        }

        // Libera nó lido
        free(temp);

        // Retorna valor lido do nó cabeça
        return valor;
    }
}

/**
 * @brief Inicializa lista
 * 
 * Insere valor nulo no início e fim da lista
 * 
 * @param lista Ponteiro para lista
 */
void inicializa_lista(Lista **lista){
    (*lista)->inicio = NULL;
    (*lista)->fim = NULL;
}


int main(){

    return 0;
}