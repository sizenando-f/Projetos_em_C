#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct{
    char nome[10];
    int prioridade, instante_submissao, indice_tempo, proximo_tempo;
    unsigned int *tempos, qtd_tempos, tam_tempos;
} Processo;

/**
 * @brief Algoritmo FCFS
 * 
 * O algoritmo FCFS executa processos na sua ordem de chegada, existindo a possibilidade de executar paralelamente ou sequencial
 * 
 * @param processos Vetor com os processos existentes
 * @param qtd_processos Quantidadde de processos no vetor de processos
 * @param seq 1: Sequencial. 2: Paralelo
 */
void algFCFS(Processo processos[], unsigned short int qtd_processos, int seq){
    // Alocação da fila de processos futuros, fila de porntos e fila de espera seguindo a ideia do diagrama proposto em aula
    Processo **processos_futuros = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_prontos = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_espera = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    // Variáveis para armazenar a quantidade de elementos em cada fila
    int tam_processos_futuros = 0, tam_fila_prontos = 0, tam_fila_espera = 0;

    // Variável para armazenar o tempo do dispositivo I/O
    int liberacao_io = 0;

    // Insere os processos na ordem de chegada
    for(int i = 0; i < qtd_processos; i++){
        // Se a fila de processos futuros estiver vazia
        if(!i){
            processos_futuros[0] = &processos[0];
            tam_processos_futuros++;
        } else {
            // Insere o processo na fila
            int j = i-1;

            while(j >= 0 && processos_futuros[j]->instante_submissao > processos[i].instante_submissao){
                processos_futuros[j+1] = processos_futuros[j];
                j--;
            }

            processos_futuros[j+1] = &processos[i]; 
            tam_processos_futuros++;
        }
    }

    // Variáveis para armazenar o resultado do algoritmo
    char saida[512], saida_temp[20];
    // Variável de controle para saber se a CPU está sendo usada
    int cpu_livre = 1;
    // O tempo ms do primeiro processo a ser lido
    int tempo_atual = processos_futuros[0]->instante_submissao;
    
    // Armazena inicio da saída
    snprintf(saida, sizeof(saida), "FCFS: %d[", processos_futuros[0]->instante_submissao);

    // Loop para executar enquanto houver processos em uma das filas existentes
    while(tam_processos_futuros || tam_fila_prontos || tam_fila_espera){

        // Insere na fila de porntos caso exista processo na fila de espera e esteja dentro do tempo ms passado
        while(tam_fila_espera > 0 && fila_de_espera[0]->proximo_tempo <= tempo_atual){
            // Adiciona no final da fila de prontos
            fila_de_prontos[tam_fila_prontos] = fila_de_espera[0];
            tam_fila_prontos++;

            // Avança na fila de espera
            for(int j = 0; j < tam_fila_espera - 1; j++){
                fila_de_espera[j] = fila_de_espera[j + 1];
            }
            tam_fila_espera--;
        }

        // Insere na fila de prontos processos na fila de processos futuros caso exista e estiver dentro do tempo em ms
        while(tam_processos_futuros > 0 && processos_futuros[0]->instante_submissao <= tempo_atual){
            // Insere no final da fila
            fila_de_prontos[tam_fila_prontos] = processos_futuros[0];
            tam_fila_prontos++;

            // Avança na fila de processos futuros
            for(int j = 0; j < tam_processos_futuros-1; j++){
                processos_futuros[j] = processos_futuros[j + 1];
            }
            tam_processos_futuros--;
        }


        // Executa caso a CPU esteja livre e exista processos na fila de prontos
        if(cpu_livre && tam_fila_prontos){
            // Seleciona o processo atual
            Processo *processo_atual = fila_de_prontos[0];
            // Avança na fila de prontos removendo o processo atual
            for(int j = 0; j < tam_fila_prontos-1; j++){
                fila_de_prontos[j] = fila_de_prontos[j + 1];
            }
            tam_fila_prontos--;
            // Informa que a CPU está em uso
            cpu_livre = 0;

            // Pega o tempo de pico do processo atual
            int tempo_pico = processo_atual->tempos[processo_atual->indice_tempo];
            // Calcula em qual ms vai encerrar o processo
            int tempo_ms_final = tempo_atual + tempo_pico;
            
            // Adiciona na saída
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", processo_atual->nome, tempo_ms_final);
            strcat(saida, saida_temp);
            
            // Avança no índice do vetor de tempos indo para um tempo I/O
            processo_atual->indice_tempo++;

            // Se for maior que a quantidade de tempos no vetor, indica que o processo terminou 
            if(processo_atual->indice_tempo >= processo_atual->qtd_tempos){
            } else {
                // Armazena o tempo de I/O atual
                int tempo_io = processo_atual->tempos[processo_atual->indice_tempo];
                // Avança para o proximo tempo (pico de CPU)
                processo_atual->indice_tempo++;

                // Se for pra executar de forma sequencial
                if(seq){
                    // O inicio da I/O precisa ser o maior entre o tempo ms atual e o tempo de liberação da I/O
                    int inicio_io = (tempo_ms_final  > liberacao_io) ? tempo_ms_final : liberacao_io;

                    // Armazena quando o processo poderá ser executado novamente
                    processo_atual->proximo_tempo  = inicio_io + tempo_io;

                    // Trava a I/O até o tempo do processo atual ser alcançado
                    liberacao_io = processo_atual->proximo_tempo;
                } else {
                    // Não trava I/O, apenas armazena no processo quando ele poderá voltar
                    processo_atual->proximo_tempo = tempo_ms_final + tempo_io;
                }
                
                // Se a fila de espera estiver vazia, insere o processo atual na primeira posição
                if(!tam_fila_espera){
                    fila_de_espera[0] = processo_atual;
                } else {
                    // Insere na fila com um insertion sort
                    int k = tam_fila_espera-1;

                    while(k >= 0 && fila_de_espera[k]->proximo_tempo > processo_atual->proximo_tempo){
                        fila_de_espera[k+1] = fila_de_espera[k];
                        k--;
                    }

                    fila_de_espera[k+1] = processo_atual; 
                }

                tam_fila_espera++;
            }

            // Atualiza o tempo ms
            tempo_atual = tempo_ms_final;
            // Indica que a CPU está livre para uso novamente
            cpu_livre = 1;
        } else {
            // Se existir processos futuros, pega o instante de submissão do primeiro
            int proximo_evento_futuros = tam_processos_futuros ? processos_futuros[0]->instante_submissao : INT_MAX;
            // Se existir processos na fila de espera, pega o o tempo onde o ocorrerá a próxima execução
            int proximo_evento_espera = tam_fila_espera ? fila_de_espera[0]->proximo_tempo : INT_MAX;
            
            // Verifica qual vem primeiro
            int proximo_tempo_evento = proximo_evento_espera < proximo_evento_futuros ? proximo_evento_espera : proximo_evento_futuros;

            // Armazena na saída
            snprintf(saida_temp, sizeof(saida_temp), "*** %d|",proximo_tempo_evento);
            strcat(saida, saida_temp);

            // Atualiza o tempo atual ms para o tempo do processo mais próximo, seja na fila de espera ou na fila de processos futuros caso exista
            tempo_atual = proximo_tempo_evento;
        }

    }

    // Finaliza saída
    snprintf(saida_temp, sizeof(saida_temp), "]\n");
    strcat(saida, saida_temp);

    printf("%s", saida);

    // Desaloca vetores de fila
    free(processos_futuros);
    free(fila_de_prontos);
    free(fila_de_espera);

}

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
        if (strspn(linha, " \t\r\n") == strlen(linha)) {
            continue; // Pula para a próxima linha do arquivo
        }

        // Se vetor estiver cheito, expande
        if(*qtd_processos == *processos_existentes){
            expandir_processos(processos, qtd_processos);
        }
        // Define o tamanho inicial do vetor de tempos
        (*processos)[*processos_existentes].tam_tempos = 1;
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
                    (*processos)[*processos_existentes].tam_tempos += 2;
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

        // Verifica se o processo possui uma quantidade ímpar de tempos de pico
        if(parte % 2 == 0){
            printf("[ #<- ] O processo na linha %d possui uma quantidade par de tempos, corrija e tente novamente!\n", cont_linha);
            exit(EXIT_FAILURE);
        }

        (*processos)[*processos_existentes].indice_tempo = 0;
        (*processos)[*processos_existentes].proximo_tempo = 0;
        char nome[10];
        snprintf(nome, sizeof(nome), "P%d", cont_linha);
        strcpy((*processos)[*processos_existentes].nome, nome);

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

    int seq = 0;

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-seq") == 0){
            seq = 1;
            break;
        }
    }

    // Faz leitura do arquivo passado
    puts("[ -> ] Lendo arquivo...");
    ler_arquivo(argv[1], &processos, &qtd_processos, &processos_existentes);
    puts("[ -> ] Arquivo lido com sucesso!");

    algFCFS(processos, processos_existentes, seq);

    // Libera todas as memórias
    for(unsigned short i = 0; i < processos_existentes; i++){
        free(processos[i].tempos);

    }
    free(processos);
    return 0;
}