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
 * O procedimento expande a capacidade do vetor de processos em 3
 * 
 * @param processos Vetor de processos a ser expandido
 * @param qtd_processos Quantidade de processos alocados 
 */
void expandir_processos(Processo processos[], unsigned short int *qtd_processos){
    // Aloca mais 3 espaços
    *qtd_processos += 4;
    Processo *temp = realloc(processos, *qtd_processos * sizeof(Processo));
    // Se der errado encerra
    if(temp == NULL){
        puts("[ -># ] Erro ao expandir processos!");
        exit(EXIT_FAILURE);
    }

    // Atualiza para vetor original
    processos = temp;
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
void ler_arquivo(char nome_arquivo[], Processo processos[], unsigned short int *qtd_processos, unsigned short *processos_existentes){
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
        // Aloca a quantidade inicial de 3 tempos para esse processo
        processos[*processos_existentes].tam_tempos = 3;
        processos[*processos_existentes].tempos = (unsigned*) malloc(processos[*processos_existentes].tam_tempos * sizeof(unsigned));

        // Parte para saber qual divisão da linha está lendo
        // Temp para armazenar o valor lido da linha
        unsigned short int parte = 1;
        int temp;

        // Copia para buffer temporário e não afetar a linha original
        strncpy(temp_linha, linha, sizeof(linha));
        token = strtok(temp_linha, " ");
        
        // Separação de cada valor da linha
        while(token != NULL){
            temp = atoi(token);

            switch (parte){
            // Prioridade
            case 1:
                if(temp < 1 || temp > 10){
                    printf("[ #<- ] A prioridade inserida na linha %d precisa estar entre 1 - 10\n", cont_linha);
                    exit(EXIT_FAILURE);
                }
                processos[*processos_existentes].prioridade = temp;
                break;
            // Instante de submissão
            case 2:
                processos[*processos_existentes].instante_submissao = temp;
                break;
            default:
                if(processos[*processos_existentes].tam_tempos < processos[*processos_existentes].qtd_tempos){
                    processos[*processos_existentes].tam_tempos += 3;
                    processos[*processos_existentes].tempos = realloc(processos[*processos_existentes].tempos, processos[*processos_existentes].tam_tempos * sizeof(int));
                    if(processos[*processos_existentes].tempos == NULL){
                        puts("[ -># ] Erro ao expandir tempos do processo!");
                        exit(EXIT_FAILURE);
                    }
                }

                processos[*processos_existentes].qtd_tempos++;
                break;
            }
            parte++;
            token = strtok(NULL, " ");
        }

        cont_linha++, processos_existentes++;
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
    ler_arquivo(argv[1], processos, &qtd_processos, &processos_existentes);

    // Libera todas as memórias
    for(unsigned short i = 0; i <= processos_existentes; i++){
        free(processos[i].tempos);
    }
    free(processos);
    return 0;
}

// // Verifica se possui quantidade mínima de argumentos
//     if(argc < 3){
//         puts("[ #<- ] Eh esperado pelo menos 3 argumentos!");
//         exit(EXIT_FAILURE);
//     }

//     unsigned short int prioridade = atoi(args[2]);

//     // Verifica se prioridade está dentro do intervalo 1 - 10
//     if(prioridade < 1 || prioridade > 10){
//         puts("[ #<- ] A prioridade precisa estar entre 1 e 10!");
//         exit(EXIT_FAILURE);
//     }

//     // Verifica se possui quantidade impar de tempos de picos de CPU
//     if(argc % 2){
//         puts("[ #<- ] Sequencia impar de picos de CPU esperada!");
//         exit(EXIT_FAILURE);
//     }