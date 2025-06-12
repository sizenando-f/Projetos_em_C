#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int prioridade, instante_submissao;
    unsigned int *tempos, qtd_tempos, tam_tempos;
} Processo;

/**
 * @brief Expande a capacidade de armazenamento de processos
 * 
 * O procedimento expande a capacidade do vetor de processos em 4
 * 
 * @param processos Vetor de processos a ser expandido
 * @param qtd_processos Quantidade de processos alocados 
 */
void expandir_processos(Processo **processos, unsigned short int *qtd_processos){
    // Aloca mais 4 espaços
    *qtd_processos += 4;
    Processo *temp = realloc(*processos, *qtd_processos * sizeof(Processo));
    // Se der errado encerra
    if(temp == NULL){
        puts("[ -># ] Erro ao expandir processos!");
        exit(EXIT_FAILURE);
    }

    // Atualiza para vetor original
    *processos = temp;
}

/**
 * @brief Lê dados de um arquivo
 * 
 * O procedimento lê os dados em cada linha do arquivo e monta structs preenchendo com suas informações
 * realiza verificações de consistência nas informações e aloca mais espaço para processos caso necessário.
 * 
 * @param nome_arquivo Nome do arquivo a ser lido
 * @param processos Vetor onde será armazenado os processos
 * @param qtd_processos Quantidade alocada para armazenar os processos 
 * @param processos_existentes Quantidade de processos existentes no vetor
 */
void ler_arquivo(char nome_arquivo[], Processo **processos, unsigned short int *qtd_processos, unsigned short *processos_existentes){
    // Abre arquivo
    FILE *fp = fopen(nome_arquivo, "r");
    if(fp == NULL){
        puts("[ #<- ] O arquivo nao existe!");
        exit(EXIT_FAILURE);
    }

    // Variáveis de buffer e token para manipulação de string
    char linha[100], temp_linha[100], *token;
    // Contador de linha para caso ocorra erro
    unsigned short int cont_linha = 1;

    // Leitura de cada linha
    while(fgets(linha, sizeof(linha), fp) != NULL){
        // Se vetor estiver cheito, expande
        if(*qtd_processos == *processos_existentes){
            expandir_processos(processos, qtd_processos);
        }
        // Define o tamanho inicial do vetor de tempos
        (*processos)[*processos_existentes].tam_tempos = 3;
        // Aloca o vetor de tempos
        (*processos)[*processos_existentes].tempos = (unsigned*) malloc((*processos)[*processos_existentes].tam_tempos * sizeof(unsigned));

        // Inicializa o processo com nenhum tempo
        (*processos)[*processos_existentes].qtd_tempos = 0;

        // Para saber qual divisão da linha está lendo
        unsigned short int parte = 0;
        // Para armazenar o valor lido da linha
        int temp;

        // Copia para buffer temporário e não afetar a linha original
        strncpy(temp_linha, linha, sizeof(linha));
        token = strtok(temp_linha, " ");
        
        // Separação de cada valor da linha
        while(token != NULL){
            // Conversão do token para inteiro
            temp = atoi(token);
           
            switch (parte){
            // Prioridade
            case 0:
                // Caso a prioridade estiver fora do intervalo permitido
                if(temp < 1 || temp > 10){
                    printf("[ #<- ] A prioridade inserida na linha %d precisa estar entre 1 - 10\n", cont_linha);
                    exit(EXIT_FAILURE);
                }
                // Armazena a prioridade no processo
                (*processos)[*processos_existentes].prioridade = temp;
                break;
            // Instante de submissão
            case 1:
                // Armazena o instante de submissão no processo
                (*processos)[*processos_existentes].instante_submissao = temp;
                break;
            default:
                // Verifica se o vetor de tempos está cheio
                if((*processos)[*processos_existentes].tam_tempos <= (*processos)[*processos_existentes].qtd_tempos){
                    // Expande o vetor em um valor ímpar
                    (*processos)[*processos_existentes].tam_tempos += 3;
                    // Realoca o vetor de tempos
                    (*processos)[*processos_existentes].tempos = realloc((*processos)[*processos_existentes].tempos, (*processos)[*processos_existentes].tam_tempos * sizeof(int));
                    // Se der errado, encerra programa
                    if((*processos)[*processos_existentes].tempos == NULL){
                        puts("[ -># ] Erro ao expandir tempos do processo!");
                        exit(EXIT_FAILURE);
                    }
                }
                
                // Armazena o tempo no vetor de tempos do processo, de 0 até a quantidade total de tempos
                (*processos)[*processos_existentes].tempos[parte-2] = (unsigned) temp;

                // Incrementa o contador de quantidade de tempos que o processo possui
                (*processos)[*processos_existentes].qtd_tempos++;
                break;
            }
            // Passa para a próxima parte da linha
            parte++;
            // Avança o token
            token = strtok(NULL, " ");
        }

        // Incrementa o contador de linha e a quantidade de processos existentes
        cont_linha++, (*processos_existentes)++;
    }

    fclose(fp);
}
 
int main(int argc, char *argv[]){
    // Verifica entradas
    if(argc == 1){
        printf("[ ? ] %s <nome_arquivo> <tamanho_quantum> <-seq (opcional)>\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if(argc <= 2 || argc > 4){
        puts("[ #<- ] Numero de argumentos invalido!");
        exit(EXIT_FAILURE);
    }

    // Aloca vetor de processos inicialmente 4
    unsigned short int qtd_processos = 5, processos_existentes = 0;
    Processo *processos = (Processo*) malloc(qtd_processos * sizeof(Processo));

    // Faz leitura do arquivo passado
    ler_arquivo(argv[1], &processos, &qtd_processos, &processos_existentes);

    // Libera todas as memórias
    for(unsigned short i = 0; i < processos_existentes; i++){
        free(processos[i].tempos);

    }
    free(processos);
    return 0;
}