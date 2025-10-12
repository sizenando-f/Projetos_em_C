/**
 * @file main.c
 * 
 * Esse trabalho é voltado para o uso de semáforos binários onde cada nó precisa ter um semáforo
 * A lógica se baseou totalmente em tratar estados como é possível ver logo abaixo
 * Problemas surgiram como threads terminar antes das outras, não saber quando tudo foi lido, travamentos por velocidade que precisou ser mediado com usleep
 * 
 * @author Sizenando S. França | RGM: 50575
 * @date 25/09/2025
 */

#include <stdio.h>     // Entrada, saída, leitura de arquivo
#include <semaphore.h> // Semáforos
#include <pthread.h>   // Threads
#include <stdlib.h>    // Para alocação
#include <unistd.h>     // Para o usleep

typedef enum {
    CRIADO,          // Indica que nó acabou de ser criado
    REMOVIDO_PAR,    // Foi removido por ser par
    CHECK_PAR,       // Passou no teste de paridade
    REMOVIDO_PRIMO,  // Foi removido por não ser primo
    CHECK_PRIMO,     // É primo
    IMPRESSO         // Já foi impresso na tela
} Estado;

typedef struct No {
    int valor;          // Valor do nó
    struct No* proximo; // Ponteiro para próximo nó
    sem_t trava_no;     // Semáforo do nó
    Estado estado;      // Estado em que o nó está
} No;

typedef struct Lista {
    No *inicio;         // Ponteiro para inicio da lista
    No *fim;            // Ponteiro para fim da lista
    sem_t trava_lista;  // Semáforo da lista
    int arquivo_completo;  // Flag para indicar se o arquivo foi totalmente lido ou não
} Lista;

Lista lista;

/**
 * @brief Inicializa a lista
 * 
 * Indica início e fim da lista como NULL.
 * Declara que não foi lido por completo e incializa seu semáforo com 1
 *  
 * @param lista Ponteiro para lista
 */
void inicializa_lista(Lista *lista){
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->arquivo_completo = 0;
    sem_init(&lista->trava_lista, 0, 1);
}

/**
 * @brief Adiciona nó na lista
 * 
 *  Adiciona nó na lista com valores padrões e com o estado CRIADO. Tomando devidos cuidados para inserir ao travar a lista
 * 
 * @param lista Ponteiro para lista
 * @param valor Valor do novo nó
 */
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

/**
 * @brief Verifica se valor é primo
 *
 * Função que verifica se valor passado é primo. Melhorado em relação ao código do trabalho passado sobre Semáforo de contagem, permitindo um cálculo ainda mais rápido
 *
 * @param valor Valor para ser verificado
 *
 * @return 1 se for primo e 0 se não for primo
 */
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

/**
 * @brief Verifica se tem trabalho a fazer
 * 
 * Essa função evita que as threads terminem antes quando ainda existe trabalho pra ser feito,
 * verificando se ainda existe nós em um estado específico que cada thread precisa processar
 * 
 * @param estado_procurado Estado do nó
 * 
 * @return 1 se tiver trabalho a ser feito, 0 caso contrário
 */
int tem_trabalho_pendente(Estado estado_procurado) {
    No *atual = lista.inicio;
    int tem_trabalho = 0;
    
    // Enquanto não for NULL
    while (atual != NULL) {
        sem_wait(&atual->trava_no);
        // Enquanto não for terminador e estado for o procurado
        if (atual->valor != -1 && atual->estado == estado_procurado) {
            // Indica que ainda tem coisa pra ser feita
            tem_trabalho = 1;
            sem_post(&atual->trava_no);
            break;
        }
        // Libera e avança
        sem_post(&atual->trava_no);
        atual = atual->proximo;
    }
    
    return tem_trabalho;
}

/**
 * @brief Remove pares da lista
 *
 * Essa função remove qualquer par e maior que 2 da lista
 * 
 */
void *remove_pares(void *arg) {
    // Para saber se chegou ao final da lista
    int sentinela_encontrado = 0;
    
    // Enquanto não for final da lista
    while (!sentinela_encontrado) {
        // Pega inicio da lista
        sem_wait(&lista.trava_lista);
        No *atual = lista.inicio;
        sem_post(&lista.trava_lista);
        
        // Para saber se algo foi feito
        int processou_algo = 0;
        
        // Enquanto não for NULL e não for final da lista
        while (atual != NULL && !sentinela_encontrado) {
            sem_wait(&atual->trava_no);
            
            // Encontra o terminador
            if (atual->valor == -1) {
                sentinela_encontrado = 1;
                sem_post(&atual->trava_no);
                break;
            }
            
            // Se o estado anterior foi concluido
            if (atual->estado == CRIADO) {
                if ((atual->valor % 2 == 0) && (atual->valor > 2)) {
                    // Marca como removido
                    atual->estado = REMOVIDO_PAR;
                } else {
                    // Marca como par
                    atual->estado = CHECK_PAR;
                }
                // Diz que algo foi feito
                processou_algo = 1;
            }
            
            // Avança
            No *proximo = atual->proximo;
            sem_post(&atual->trava_no);
            atual = proximo;
        }
        
        // Se não encontrou sentinela e não processou nada, verifica se ainda tem trabalho
        if (!sentinela_encontrado) {
            if (!processou_algo && lista.arquivo_completo) {
                // Verifica várias vezes se realmente não há mais trabalho
                int tentativas = 0;
                int sem_trabalho = 0;
                while (tentativas < 3 && !sem_trabalho) {
                    usleep(5000); // 5ms
                    if (!tem_trabalho_pendente(CRIADO)) {
                        sem_trabalho = 1;
                    }
                    tentativas++;
                }
                if (sem_trabalho) break;
            }
            usleep(1000);
        }
    }
    return NULL;
}

/**
 * @brief Remove não primos
 * 
 * Essa função remove qualquer valor que não seja primo usando a função auxiliar `verifica_primo`
 * 
 */
void *filtra_primos(void *arg){
    // Para saber se final da lista foi chego
    int sentinela_encontrado = 0;
    
    // Enquanto não for final da lista
    while (!sentinela_encontrado){
        // Pega inicio da lista
        sem_wait(&lista.trava_lista);
        No *atual = lista.inicio;
        sem_post(&lista.trava_lista);
        
        // Para saber se algo foi feito
        int processou_algo = 0;
        
        // Enquanto não for NULL e não for final da lista
        while(atual != NULL && !sentinela_encontrado){
            sem_wait(&atual->trava_no);
            
            // Se o terminador foi encontrado
            if (atual->valor == -1) {
                sentinela_encontrado = 1;
                sem_post(&atual->trava_no);
                break;
            }
            
            // Se o estado anterior foi conluido
            if(atual->estado == CHECK_PAR){
                if(verifica_primo(atual->valor)){
                    // Marca como primo
                    atual->estado = CHECK_PRIMO;
                } else {
                    // Marca que foi removido
                    atual->estado = REMOVIDO_PRIMO;
                }
                // Diz que algo foi feito
                processou_algo = 1;
            }
            
            // Avança
            No *proximo = atual->proximo;
            sem_post(&atual->trava_no);
            atual = proximo;
        }
        
        // Se não encontrou sentinela e não processou nada, verifica se ainda tem trabalho
        if (!sentinela_encontrado) {
            if (!processou_algo && lista.arquivo_completo) {
                // Verifica várias vezes se realmente não há mais trabalho
                int tentativas = 0;
                int sem_trabalho = 0;
                // Tenta 3 vezes
                while (tentativas < 3 && !sem_trabalho) {
                    usleep(5000); // 5ms
                    if (!tem_trabalho_pendente(CHECK_PAR)) {
                        sem_trabalho = 1;
                    }
                    tentativas++;
                }
                if (sem_trabalho) break;
            }
            usleep(1000);
        }
    }
    return NULL;
}

/**
 * @brief Imprime resultados
 * 
 * Essa função imprime os resultados obtidos, os nós cujo seu estado é CHECK_PRIMO
 */
void *imprime_primos(void *arg){
    // Para saber se chegou ao final da lista
    int sentinela_encontrado = 0;
    
    // Enquanto não chegou no final
    while (!sentinela_encontrado){
        // Pega o inicio da lista
        sem_wait(&lista.trava_lista);
        No *atual = lista.inicio;
        sem_post(&lista.trava_lista);
        
        // Para saber se houve alguma alteração
        int processou_algo = 0;
        
        // Enquanto não for NULL e nem final da lista
        while(atual != NULL && !sentinela_encontrado){
            sem_wait(&atual->trava_no);
            
            // Verifica se encontrou terminador
            if (atual->valor == -1) {
                sentinela_encontrado = 1;
                sem_post(&atual->trava_no);
                break;
            }
            
            // Se estado anterior foi conclúido, printa valor
            if(atual->estado == CHECK_PRIMO){
                printf("%d ", atual->valor);
                fflush(stdout);
                atual->estado = IMPRESSO;
                processou_algo = 1;
            }
            
            // Avança para próximo
            No *proximo = atual->proximo;
            sem_post(&atual->trava_no);
            atual = proximo;
        }
        
        // Se não encontrou sentinela e não processou nada, verifica se ainda tem trabalho pra ser feito
        if (!sentinela_encontrado) {
            // Se não processou e o arquivo foi lido por completo
            if (!processou_algo && lista.arquivo_completo) {
                // Verifica váerias vezes se realmente não há mais trabalho
                int tentativas = 0;
                int sem_trabalho = 0;
                // Verifica 3 vezes se não houver trabalho
                while (tentativas < 3 && !sem_trabalho) {
                    usleep(5000); // 5ms
                    if (!tem_trabalho_pendente(CHECK_PRIMO)) {
                        sem_trabalho = 1;
                    }
                    tentativas++;
                }
                if (sem_trabalho) break;
            }
            usleep(1000);
        }
    }
    
    return NULL;
}

/**
 * @brief Libera memória
 * 
 * Essa função limpa toda a lista setando inicio e fim como NULL e liberando todos os semáforos de cada nó
 * 
 * @param lista Ponteiro para a lista
 */
void libera_memoria(Lista *lista){
    No *atual;

    // Trava a lista e seta tudo como NULL
    sem_wait(&lista->trava_lista);
    atual = lista->inicio;
    lista->inicio = NULL;
    lista->fim = NULL;
    sem_post(&lista->trava_lista);

    // Vai liberado todos os semáforos
    while(atual != NULL){
        No *proximo = atual->proximo;
        sem_destroy(&atual->trava_no);
        free(atual);
        atual = proximo;
    }

    sem_destroy(&lista->trava_lista);
}

int main(){
    // Threads para execução
    pthread_t thread_impares, thread_primos, thread_imprime;

    // Inicializa a lista com NULL
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

    // Adiciona os valores lidos na lista
    int valor;
    while (fscanf(arquivo, "%d", &valor) != EOF)
    {
        adiciona_no(&lista, valor);
    }
    fclose(arquivo);

    // Diz que o arquivo foi lido por completo
    sem_wait(&lista.trava_lista);
    lista.arquivo_completo = 1;
    sem_post(&lista.trava_lista);

    // Espera até que todo o processamento seja concluído
    int processamento_completo = 0;
    while (!processamento_completo) {
        usleep(10000); // 10ms entre verificações
        
        // Verifica se todos os nós foram lidos e processados por completo
        sem_wait(&lista.trava_lista);
        No *atual = lista.inicio;
        int tem_nao_processados = 0;
        int tem_primos_nao_impressos = 0;
        
        // Fica verificando os estados
        while (atual != NULL) {
            sem_wait(&atual->trava_no);
            if (atual->estado == CRIADO || atual->estado == CHECK_PAR) {
                tem_nao_processados = 1;
            }
            if (atual->estado == CHECK_PRIMO) {
                tem_primos_nao_impressos = 1;
            }
            sem_post(&atual->trava_no);
            // Avança
            atual = atual->proximo;
        }
        sem_post(&lista.trava_lista);
        
        // Se tudo for processado e impresso então termina
        if (!tem_nao_processados && !tem_primos_nao_impressos) {
            processamento_completo = 1;
        }
    }

    // Adiciona o sentinela pra dizer que é o final da lista
    adiciona_no(&lista, -1);

    // Espera todas as threads terminarem
    pthread_join(thread_impares, NULL);
    pthread_join(thread_primos, NULL);
    pthread_join(thread_imprime, NULL);

    // Finaliza programa liberado toda a memória
    libera_memoria(&lista);

    return 0;
}