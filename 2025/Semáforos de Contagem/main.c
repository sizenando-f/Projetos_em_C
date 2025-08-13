#include <stdio.h>     // Entrada, saída, leitura de arquivo
#include <semaphore.h> // Semáforos
#include <pthread.h>   // Threads
#include <string.h>    // Nome do arquivo
#include <stdlib.h>    // Para alocação

// Estrutura do nó
typedef struct No
{
    int valor;
    struct No *proximo;
} No;

// Estrutura da lista
typedef struct
{
    No *inicio;
    No *fim;
} Lista;

// Listas que serão usadas
Lista L1, L2, L3;
// Semáforos para controlar acesso a lista encadeada
sem_t *mutex1, *mutex2, *mutex3;
// Semáforos para contar quantos valores estão disponíveis
sem_t *cont1, *cont2, *cont3;

char nome_arquivo[50] = "";

/**
 * @brief Adiciona novo nó numa lista
 *
 * Adiciona um novo nó com o valor desejado na lista simplesmente encadeada
 *
 * @param lista Ponteiro para início da lista
 * @param valor Valor do nó
 */
void adiciona_no(Lista *lista, int valor)
{
    // Aloca novo nó e inicializa valores
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->proximo = NULL;

    // Adiciona no ínicio se não houver nós
    if (lista->inicio == NULL)
    {
        lista->inicio = novoNo;
        lista->fim = novoNo;

        // Se já existir nó na (*lista)
    }
    else
    {
        lista->fim->proximo = novoNo;
        lista->fim = novoNo;
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
int remove_no(Lista *lista)
{
    // Verifica se lista está vazia
    if (lista->inicio == NULL)
    {
        return -1;
    }
    else
    {
        int valor;
        No *temp = lista->inicio;

        // Pega valor e passa e avança para próxmimo nó
        valor = temp->valor;
        lista->inicio = lista->inicio->proximo;

        // Se lista for vazia, define final como nulo
        if (lista->inicio == NULL)
        {
            lista->fim = NULL;
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
void inicializa_lista(Lista *lista)
{
    lista->inicio = NULL;
    lista->fim = NULL;
}

/**
 * @brief Lê valores de um arquivo
 *
 * Thread que lê os valores de um arquivo desejado e insere numa
 * lista simplesmente encadeada
 *
 */
void *thread_insere_L1(void *arg)
{
    // Abre arquivo e verifica se foi aberto corretamente
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo!");
        pthread_exit(NULL);
    }

    // Lê valor por valor
    int valor;
    while (fscanf(arquivo, "%d", &valor) != EOF)
    {
        // Bloqueia o acesso à lista 1
        sem_wait(mutex1);
        // Adiciona o valor na lista
        adiciona_no(&L1, valor);
        // Libera o acesso à lista 1
        sem_post(mutex1);
        // Sinaliza que lista 1 possui valor
        sem_post(cont1);
    }
    // Fecha arquivo
    fclose(arquivo);

    // Bloqueia acesso à lista 1
    sem_wait(mutex1);
    // Adiciona flag -1 para indicar final da lista
    adiciona_no(&L1, -1);
    // Libera acesso à lista 1
    sem_post(mutex1);
    // Sinaliza que lista 1 possui mais um valor
    sem_post(cont1);

    // Encerra a thread
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    strcpy(nome_arquivo, argv[1]);
    return 0;
}