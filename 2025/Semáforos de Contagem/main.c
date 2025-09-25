/**
 * @file main.c
 *
 * Esse arquivo contém a implementação do trabalho de S.O. sobre
 * o uso de semáforos para contagem. De forma concorrente é realizo:
 * 1. Leitura de um arquivo "in.txt"
 * 2. Filtro por valores ímpares e menor/igual a 2
 * 3. Filtro de números primos
 * 4. Impressão do resultado
 *
 * @author Sizenando S. França | RGM: 50575
 * @date 29-08-2025
 */

#include <stdio.h>     // Entrada, saída, leitura de arquivo
#include <semaphore.h> // Semáforos
#include <pthread.h>   // Threads
#include <string.h>    // Nome do arquivo
#include <stdlib.h>    // Para alocação
#include <fcntl.h>     // Para O_CREAT

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

// Definicão dos nomes dos semáforos
const char *MUTEX1_NOME = "/sem_mutex1";
const char *MUTEX2_NOME = "/sem_mutex2";
const char *MUTEX3_NOME = "/sem_mutex3";
const char *CONT1_NOME = "/sem_cont1";
const char *CONT2_NOME = "/sem_cont2";
const char *CONT3_NOME = "/sem_cont3";

// Nome do arquivo a ser lido
char nome_arquivo[] = "in.txt";

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
 * @brief Cria lista 2 com impares
 *
 * Thread que cria uma lista encadeada apenas com os números ímpares ou
 * valores <= 2
 *
 */
void *remove_pares(void *arg)
{
    // Variável para receber  o valor da lista
    int valor;
    while (1)
    {
            sem_wait(cont1);
            // Espera pela lista 1 ter algum elemento novo
            // Assume o controle da lista
            sem_wait(mutex1);
            // Lê e remove o valor do nó cabeça
            valor = remove_no(&L1);
            // Devolve o acesso à lista
            sem_post(mutex1);

            // Se for impar oi menor, valor é inserido na lista 2
            if (valor == -1)
            {
                break;
            }
            else if ((valor % 2 == 1) || valor <= 2)
            {
                // Toma acesso da lista 2
                sem_wait(mutex2);
                // Adiciona valor na nova lista
                adiciona_no(&L2, valor);
                // Devolve acesso à lista
                sem_post(mutex2);
                // Sinaliza que lista contém novo elemento
                sem_post(cont2);
            }
        }


    
    // Toma acesso da lista 2
    sem_wait(mutex2);
    // Adiciona flag de terminação na nova lista
    adiciona_no(&L2, -1);
    // Devolve acesso à lista
    sem_post(mutex2);
    // Sinaliza que lista contém novo nó
    sem_post(cont2);

    return NULL;
}

/**
 * @brief Verifica se valor é primo
 *
 * Função que verifica se valor passado é primo
 *
 * @param valor Valor para ser verificado
 *
 * @return 1 se for primo e 0 se não for primo
 */
int verifica_primo(int valor)
{
    if (valor <= 1)
    {
        return 0;
    }
    else
    {
        for (int i = 2; i < valor; i++)
        {
            if (valor % i == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief Cria lista 3 com primos
 *
 * Cria uma lista simplesmente encadeada contendo apenas valores primos
 * da lista 2
 */
void *filtra_primos(void *arg)
{
    int valor;
    while (1)
    {
        sem_wait(cont2);

        // Recebe acesso a lista 2
        sem_wait(mutex2);
        // Pega o valor do nó cabeça e remove ele
        valor = remove_no(&L2);
        // Devolve o acesso da lista 2
        sem_post(mutex2);

        // Se não for valor terminal
        if (valor == -1)
        {
            break;
        }
        else
        {
            // Se for primo
            if (verifica_primo(valor))
            {
                // Ganha o acesso a lista 3
                sem_wait(mutex3);
                // Adiciona o valor primo na lista
                adiciona_no(&L3, valor);
                // Devolve acesso
                sem_post(mutex3);
                // Sinaliza que lista contém novo nó
                sem_post(cont3);
            }
        }
        
    }

    // Toma acesso da lista 3
    sem_wait(mutex3);
    // Adiciona flag de terminação na nova lista
    adiciona_no(&L3, -1);
    // Devolve acesso à lista
    sem_post(mutex3);
    // Sinaliza que lista contém novo nó
    sem_post(cont3);

    return NULL;
}

/**
 * @brief Imprime valores de L3
 *
 * Imprime todos valores existentes na lista 3
 */
void *imprime_primos(void *arg)
{
    int valor;
    printf("[ -> ] Valores primos encontrados: \n");
    while (1)
    {
        // Verifica se lista 2 possui novo nó
        sem_wait(cont3);

        // Recebe acesso a lista 2
        sem_wait(mutex3);
        // Pega o valor do nó cabeça e remove ele
        valor = remove_no(&L3);
        // Devolve o acesso da lista 2
        sem_post(mutex3);

        // Se não for valor terminal, imprime na tela
        if (valor == -1)
        {
            break;
        }
        else
        {
            printf("%d ", valor);
        }
    }

    return NULL;
}

/**
 * @brief Iniciliza semáforos
 *
 * Realiza o unlink de possíveis semáforos abertos e abre novos com
 * seus respectivos valores iniciais
 */
void inicializa_semaforos()
{
    // Realiza o unlink de possíveis semáforos abertos anteriormente
    sem_unlink(CONT1_NOME);
    sem_unlink(CONT2_NOME);
    sem_unlink(CONT3_NOME);
    sem_unlink(MUTEX1_NOME);
    sem_unlink(MUTEX2_NOME);
    sem_unlink(MUTEX3_NOME);

    // Realiza a abertura de cada semáforo
    cont1 = sem_open(CONT1_NOME, O_CREAT, 0660, 0);
    if (cont1 == SEM_FAILED)
    {
        perror("Erro ao criar cont1");
        exit(EXIT_FAILURE);
    }

    cont2 = sem_open(CONT2_NOME, O_CREAT, 0660, 0);
    if (cont2 == SEM_FAILED)
    {
        perror("Erro ao criar cont2");
        exit(EXIT_FAILURE);
    }

    cont3 = sem_open(CONT3_NOME, O_CREAT, 0660, 0);
    if (cont3 == SEM_FAILED)
    {
        perror("Erro ao criar cont3");
        exit(EXIT_FAILURE);
    }

    mutex1 = sem_open(MUTEX1_NOME, O_CREAT, 0660, 1);
    if (mutex1 == SEM_FAILED)
    {
        perror("Erro ao criar mutex1");
        exit(EXIT_FAILURE);
    }

    mutex2 = sem_open(MUTEX2_NOME, O_CREAT, 0660, 1);
    if (mutex2 == SEM_FAILED)
    {
        perror("Erro ao criar mutex2");
        exit(EXIT_FAILURE);
    }

    mutex3 = sem_open(MUTEX3_NOME, O_CREAT, 0660, 1);
    if (mutex3 == SEM_FAILED)
    {
        perror("Erro ao criar mutex3");
        exit(EXIT_FAILURE);
    }
}

// "Thread principal"
int main(int argc, char **argv)
{
    // Define as threads
    pthread_t thread_impares, thread_primos, thread_imprime;

    // Incializa cada lista com NULL
    inicializa_lista(&L1);
    inicializa_lista(&L2);
    inicializa_lista(&L3);

    // Inicializa todos mutex e contadores
    inicializa_semaforos();

    // Cria todas as threads
    // pthread_create(&thread_le, NULL, principal, NULL);
    pthread_create(&thread_impares, NULL, remove_pares, NULL);
    pthread_create(&thread_primos, NULL, filtra_primos, NULL);
    pthread_create(&thread_imprime, NULL, imprime_primos, NULL);

    // Abre arquivo e verifica se foi aberto corretamente
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo!");
        exit(EXIT_FAILURE);
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

    // Define cada uma para esperar
    pthread_join(thread_impares, NULL);
    pthread_join(thread_primos, NULL);
    pthread_join(thread_imprime, NULL);

    // Encerra todos os semáforos
    sem_close(cont1);
    sem_close(cont2);
    sem_close(cont3);
    sem_close(mutex1);
    sem_close(mutex2);
    sem_close(mutex3);

    // Apaga o link com os semáforos salvos
    sem_unlink(CONT1_NOME);
    sem_unlink(CONT2_NOME);
    sem_unlink(CONT3_NOME);
    sem_unlink(MUTEX1_NOME);
    sem_unlink(MUTEX2_NOME);
    sem_unlink(MUTEX3_NOME);

    return 0;
}