/**
 * @file Escalonamento de CPU
 * 
 * Trabalho 3 referente ao curso de Ciência da Computação.
 * Simulação de cinco algoritmos de escalonamento: FCFS, SJF, SRTF, Prioridade Preemptivo, Round-Robin
 * 
 * @author Sizenando S. França | RGM: 50575
 */

#include <stdio.h>  // Para entrada e saída
#include <stdlib.h> // Para alocação de memória
#include <string.h> // Para manipulação de strings
#include <limits.h> // Para escolha de menor tempo em algoritmos de preempção

typedef struct{
    char nome[10];
    int prioridade, instante_submissao, indice_tempo, proximo_tempo, tempo_restante;
    unsigned int *tempos, qtd_tempos, tam_tempos;

    // Esses aqui são para os cálculos de CPU e etc
    int termino, turnaround, espera;
} Processo;

/**
 * @brief Calcula turnaround
 * 
 * Procedimento que calcula turnaround de cada processo e a média de todos os turnarounds
 * Salva o resultado no arquivo
 * 
 * @param processos Vetor de processos
 * @param qtd_processos Quantidade de processos no vetor
 * @param fp Arquivo para escrita do resultado
 * 
 */
void calcula_turnaround(Processo processos[], unsigned short int qtd_processos, FILE *fp){
    int soma = 0;
    char temp[50];

    fputs("------------------------\n", fp);
    fputs("TURNAROUND:\n", fp);
    for(int i = 0; i < qtd_processos; i++){
        snprintf(temp, sizeof(temp), "%s: %dms\n", processos[i].nome, processos[i].turnaround);
        fputs(temp, fp);
        soma += processos[i].turnaround;
    }
    float media =  (float) soma / qtd_processos;

    snprintf(temp, sizeof(temp), "MEDIO: %.2fms\n", media);
    fputs(temp, fp);
    fputs("------------------------\n", fp);
}

/**
 * @brief Calcula tempo de espera
 * 
 * Procedimento que calcula o tempo de espera de cada processo e a média de todos os turnarounds
 * Salva o resultado no arquivo
 * 
 * @param processos Vetor de processos
 * @param qtd_processos Quantidade de processos no vetor
 * @param fp Arquivo para escrita do resultado
 * 
 */
void calcula_tempo_espera(Processo processos[], unsigned short int qtd_processos, FILE *fp){
    char temp[50];

    fputs("TEMPO DE ESPERA:\n", fp);
    int soma = 0;
    for(int i = 0; i < qtd_processos; i++){
        int picos = 0;
        for(int j = 0; j < processos[i].qtd_tempos; j += 2){
            picos += processos[i].tempos[j];
        }
        processos[i].espera = processos[i].turnaround - picos > 0 ? processos[i].turnaround - picos : 0;
        soma += processos[i].espera;
        snprintf(temp, sizeof(temp), "%s: %dms\n", processos[i].nome, processos[i].espera);
        fputs(temp, fp);
    }
    float media =  (float) soma / qtd_processos;

    snprintf(temp, sizeof(temp), "MEDIO: %.2fms\n", media);
    fputs(temp, fp);
    fputs("------------------------\n\n", fp);
}

/**
 * @brief Algoritmo de escalonamento Round-Robin
 * 
 * Esse algoritmo realiza a preempção entre processos a cada intervalo
 * (quantam) escolhidos pelo usuário, calculando uso de CPU e throughput no final
 * 
 * @param processos Vetor com os processos do arquivo
 * @param qtd_processos Quantidade de processos no vetor
 * @param seq 1: Sequencial, 0: Paralelo
 * @param quantum Quantum para preempção
 * @param fp Arquivo para escrita
 */
char* algRoundRobin(Processo processos[], unsigned short int qtd_processos, int seq, int quantum, FILE *fp){
    // ALocação das filas
    Processo **processos_futuros = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_prontos = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_espera = (Processo**) malloc(qtd_processos * sizeof(Processo*));

    // Variáveis para métricas
    int tempo_total_cpu = 0;
    int tempo_final = 0;

    // Variáveis de controle
    int tam_processos_futuros = 0, tam_fila_prontos = 0, tam_fila_espera = 0;
    // Variável de controle para tempo I/O
    int liberacao_io = 0;
    
    // Insere na fila de processo futuros ordenado por tempo de submissão
    for(int i = 0; i < qtd_processos; i++){
        if(!i){
            processos_futuros[0] = &processos[0];
            tam_processos_futuros++;
        } else {
            int j = i - 1;

            while(j >= 0 && processos_futuros[j]->instante_submissao > processos[i].instante_submissao){
                processos_futuros[j+1] = processos_futuros[j];
                j--;
            }

            processos_futuros[j+1] = &processos[i];
            tam_processos_futuros++;
        }

        processos[i].indice_tempo = 0;
        processos[i].tempo_restante = processos[i].tempos[0];
    }

    // Variáveis para armazenar a saída
    char saida[512], saida_temp[20];
    // Atual processo em execução
    Processo *executando = NULL;
    // Tempo atual iniciando com o primeiro instante de submissão na fila de processos futuros
    int tempo_atual = processos_futuros[0]->instante_submissao, quantum_final = 0;

    // Cria ínicio da saída
    snprintf(saida, sizeof(saida), "ROUND-ROBIN(q=%dms): %d[", quantum, tempo_atual);

    // Enquanto houver processos em alguma das filas
    while(tam_processos_futuros || tam_fila_espera || tam_fila_prontos || executando){
        // Se houver processo em espera e seu tempo atingiu o ms atual
        while(tam_fila_espera && fila_de_espera[0]->proximo_tempo <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = fila_de_espera[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_fila_espera - 1; i++){
                fila_de_espera[i] = fila_de_espera[i + 1];
            }
            tam_fila_espera--;

        }

        // Se houver processos futuros e seu instante de chegada na CPU atingiu o tempo ms atual
        while(tam_processos_futuros && processos_futuros[0]->instante_submissao <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = processos_futuros[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_processos_futuros - 1; i++){
                processos_futuros[i] = processos_futuros[i + 1];
            }
            tam_processos_futuros--;
        }

        // Se não tiver processo executando e houver processos na fila de prontos
        if(executando == NULL && tam_fila_prontos > 0){
            // O primeiro processo começa a executar
            executando = fila_de_prontos[0];

            for(int i = 0; i < tam_fila_prontos - 1; i++){
                fila_de_prontos[i] = fila_de_prontos[i + 1];
            }
            tam_fila_prontos--;

            // Realiza o cálculo do tempo do quantum final
            quantum_final = tempo_atual + quantum;
        }

        if(executando == NULL){
            // Se existir processos futuros, pega o instante de submissão do primeiro
            int proximo_evento_futuros = tam_processos_futuros > 0 ? processos_futuros[0]->instante_submissao : INT_MAX;
            // Se existir processos na fila de espera, pega o o tempo onde o ocorrerá a próxima execução
            int proximo_evento_espera = tam_fila_espera > 0 ? fila_de_espera[0]->proximo_tempo : INT_MAX;
            // Verifica qual vem primeiro
            int proximo_tempo_evento = proximo_evento_espera < proximo_evento_futuros ? proximo_evento_espera : proximo_evento_futuros;

            // Se nenhum for menor
            if(proximo_tempo_evento == INT_MAX) break;

            // Armazena na saída
            snprintf(saida_temp, sizeof(saida_temp), "*** %d|",proximo_tempo_evento);
            strcat(saida, saida_temp);

            // Atualiza o tempo atual ms para o tempo do processo mais próximo, seja na fila de espera ou na fila de processos futuros caso exista
            tempo_atual = proximo_tempo_evento;
            continue;
        }

        // Se houver processo futuro, pega o instante de submissão do primeiro
        int proximo_evento = (tam_processos_futuros > 0) ? processos_futuros[0]->instante_submissao : INT_MAX;
        // Cálculo de término
        int tempo_termino = tempo_atual + executando->tempo_restante;
        // Pega o menor tempo
        int proximo_tempo = (proximo_evento < tempo_termino) ? proximo_evento : tempo_termino;
        // Filtra o menor entre os três
        proximo_tempo = (quantum_final < proximo_tempo) ? quantum_final : proximo_tempo;

        int diferenca = proximo_tempo - tempo_atual;

        // Se a diferença for um valor válido, decrementa no tempo restante do processo em execução
        if(diferenca > 0){
            executando->tempo_restante -= diferenca;

            tempo_total_cpu += diferenca;
        }

        // O tempo atual ms é escolhido
        tempo_atual = proximo_tempo;

        // Se o tempo restante do processo em execução chegou ao fim
        if(executando->tempo_restante <= 0){
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", executando->nome, tempo_atual);
            strcat(saida, saida_temp);

            // Passa para o próximo tempo
            executando->indice_tempo++;

            // Se não houver mais tempos para ser analisado
            if(executando->indice_tempo >= executando->qtd_tempos){
                // Calcula turnaround e libera CPU
                executando->termino = tempo_atual;
                executando->turnaround = executando->termino - executando->instante_submissao;
                executando = NULL;
            } else {
                // Seleciona o tempo de espera de entrada/saída
                int io = executando->tempos[executando->indice_tempo];
                // Passa para o próximo tempo
                executando->indice_tempo++;

                // Se for sequencial
                if(seq){
                    // Armazena o maior entre o tempo atual e o tempo da próxima liberação de entrada/sa´dia
                    int inicio_io = (tempo_atual > liberacao_io) ? tempo_atual : liberacao_io;
                    // Informa quando o processo voltará a executar
                    executando->proximo_tempo = inicio_io + io;
                    // O tempo para a próxima liberação é o próprio tempo do processo em execução
                    liberacao_io = executando->proximo_tempo;
                } else {
                    // Apenas armazena o próximo tempo sem influênciar a variável de controle da próxima liberação da I/O
                    executando->proximo_tempo = tempo_atual + io;
                }

                // Se ainda houver tempos para ser analisados
                if(executando->indice_tempo < executando->qtd_tempos){
                    executando->tempo_restante = executando->tempos[executando->indice_tempo];
                }

                // Se não houver processo na fila de espera
                 if(!tam_fila_espera){
                    // Coloca o processo em execução na fila
                    fila_de_espera[0] = executando;
                } else {
                    // Insere na fila ordenando pelo próximo tempo de espera
                    int k = tam_fila_espera-1;
                    while(k >= 0 && fila_de_espera[k]->proximo_tempo > executando->proximo_tempo){
                        fila_de_espera[k+1] = fila_de_espera[k];
                        k--;
                    }
                    fila_de_espera[k+1] = executando;
                }
                tam_fila_espera++;
                executando = NULL;
            }
        // Se o tempo atual em ms alcançou o quantum atual acumulado
        } else if (tempo_atual >= quantum_final){
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", executando->nome, tempo_atual);
            strcat(saida, saida_temp);

            // Passa o processo em execução para o final da fila
            fila_de_prontos[tam_fila_prontos] = executando;
            tam_fila_prontos++;
            executando = NULL;
        }
    }

    // Finaliza saída
    saida[strlen(saida)-1] = ']';

    tempo_final = tempo_atual;
    float uso_cpu = ((float)tempo_total_cpu / tempo_final) * 100;
    char temp[50];
    snprintf(temp, sizeof(temp), "UTILIZACAO CPU: %f\n", uso_cpu);
    fputs(temp, fp);
    snprintf(temp, sizeof(temp), "THROUGHPUT: %f\n", (float)qtd_processos / tempo_final);
    fputs(temp, fp);


    // Libera os vetores alocados
    free(processos_futuros);
    free(fila_de_prontos);
    free(fila_de_espera);

    char *final = (char*) malloc(sizeof(saida) + 1);
    strcpy(final, saida);
    strcat(final, "\n");
    return final;
}

/**
 * @brief Algoritmo de escalonamento por prioridade com preempção
 * 
 * O algoritmo é exatamento mesmo que a SRTF, a única diferença é que na escolha do algoritmo a qual será preemptado, no SRTF
 * comparava-se o tempo restante e nesse, se compara a prioridade,calculando uso de CPU e throughput no final
 * 
 * @param processos Vetor com os processos do arquivo
 * @param qtd_processos Quantidade de processos no vetor
 * @param seq 1: Sequencial, 0: Paralelo
 * @param fp Arquivo para escrita
 */
char* algPrioridadePreemptivo(Processo processos[], unsigned short int qtd_processos, int seq, FILE *fp){
    // ALocação das filas
    Processo **processos_futuros = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_prontos = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_espera = (Processo**) malloc(qtd_processos * sizeof(Processo*));

    // Variáveis para métricas
    int tempo_total_cpu = 0;
    int tempo_final = 0;

    // Variáveis de controle
    int tam_processos_futuros = 0, tam_fila_prontos = 0, tam_fila_espera = 0;
    // Variável de controle para tempo I/O
    int liberacao_io = 0;
    
    // Insere na fila de processo futuros ordenado por tempo de submissão
    for(int i = 0; i < qtd_processos; i++){
        if(!i){
            processos_futuros[0] = &processos[0];
            tam_processos_futuros++;
        } else {
            int j = i - 1;

            while(j >= 0 && processos_futuros[j]->instante_submissao > processos[i].instante_submissao){
                processos_futuros[j+1] = processos_futuros[j];
                j--;
            }

            processos_futuros[j+1] = &processos[i];
            tam_processos_futuros++;
        }

        processos[i].indice_tempo = 0;
        processos[i].tempo_restante = processos[i].tempos[0];
    }

    // Variáveis para armazenar a saída
    char saida[512], saida_temp[20];
    Processo *executando = NULL;
    // Tempo atual iniciando com o primeiro instante de submissão na fila de processos futuros
    int tempo_atual = processos_futuros[0]->instante_submissao;

    // Cria ínicio da saída
    snprintf(saida, sizeof(saida), "PRIORIDADE PREEMPTIVO: %d[",tempo_atual);

    // Enquanto houver processos em alguma das filas
    while(tam_processos_futuros || tam_fila_espera || tam_fila_prontos || executando){
        // Se houver processo em espera e seu tempo atingiu o ms atual
        while(tam_fila_espera && fila_de_espera[0]->proximo_tempo <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = fila_de_espera[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_fila_espera - 1; i++){
                fila_de_espera[i] = fila_de_espera[i + 1];
            }
            tam_fila_espera--;

        }

        // Se houver processos futuros e seu instante de chegada na CPU atingiu o tempo ms atual
        while(tam_processos_futuros && processos_futuros[0]->instante_submissao <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = processos_futuros[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_processos_futuros - 1; i++){
                processos_futuros[i] = processos_futuros[i + 1];
            }
            tam_processos_futuros--;
        }

        Processo *anterior = executando;

        if(executando != NULL){
            fila_de_prontos[tam_fila_prontos] = executando;
            tam_fila_prontos++;
        }

        if(tam_fila_prontos){
            int idx_menor = 0;
            for(int i = 1; i < tam_fila_prontos; i++){
                if(fila_de_prontos[i]->prioridade > fila_de_prontos[idx_menor]->prioridade){
                    idx_menor = i;
                }
            }

            executando =  fila_de_prontos[idx_menor];

            for(int i = idx_menor; i < tam_fila_prontos - 1; i++){
                fila_de_prontos[i] = fila_de_prontos[i + 1];
            }
            tam_fila_prontos--;
        } else {
            executando = NULL;
        }

        if(anterior != NULL && executando != anterior){
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", anterior->nome, tempo_atual);
            strcat(saida, saida_temp);
        }

        if(executando == NULL){
            // Se existir processos futuros, pega o instante de submissão do primeiro
            int proximo_evento_futuros = tam_processos_futuros > 0 ? processos_futuros[0]->instante_submissao : INT_MAX;
            // Se existir processos na fila de espera, pega o o tempo onde o ocorrerá a próxima execução
            int proximo_evento_espera = tam_fila_espera > 0 ? fila_de_espera[0]->proximo_tempo : INT_MAX;
            // Verifica qual vem primeiro
            int proximo_tempo_evento = proximo_evento_espera < proximo_evento_futuros ? proximo_evento_espera : proximo_evento_futuros;

            if(proximo_tempo_evento == INT_MAX) break;

            // Armazena na saída
            snprintf(saida_temp, sizeof(saida_temp), "*** %d|",proximo_tempo_evento);
            strcat(saida, saida_temp);

            // Atualiza o tempo atual ms para o tempo do processo mais próximo, seja na fila de espera ou na fila de processos futuros caso exista
            tempo_atual = proximo_tempo_evento;
            continue;
        }

        int proximo_evento = (tam_processos_futuros > 0) ? processos_futuros[0]->instante_submissao : INT_MAX;
        int tempo_termino = tempo_atual + executando->tempo_restante;

        int proximo_tempo = (proximo_evento < tempo_termino) ? proximo_evento : tempo_termino;
    
        int diferenca = proximo_tempo - tempo_atual;

        if(diferenca){
            executando->tempo_restante -= diferenca;

            tempo_total_cpu += diferenca;
        }

        tempo_atual = proximo_tempo;

        if(executando->tempo_restante <= 0){
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", executando->nome, tempo_atual);
            strcat(saida, saida_temp);

            executando->indice_tempo++;

            if(executando->indice_tempo >= executando->qtd_tempos){
                executando->termino = tempo_atual;
                executando->turnaround = executando->termino - executando->instante_submissao;
                executando = NULL;
            } else {
                int io = executando->tempos[executando->indice_tempo];
                executando->indice_tempo++;

                if(seq){
                    int inicio_io = (tempo_atual > liberacao_io) ? tempo_atual : liberacao_io;
                    executando->proximo_tempo = inicio_io + io;
                    liberacao_io = executando->proximo_tempo;
                } else {
                    executando->proximo_tempo = tempo_atual + io;
                }

                if(executando->indice_tempo < executando->qtd_tempos){
                    executando->tempo_restante = executando->tempos[executando->indice_tempo];
                }

                 if(!tam_fila_espera){
                    fila_de_espera[0] = executando;
                } else {
                    int k = tam_fila_espera-1;
                    while(k >= 0 && fila_de_espera[k]->proximo_tempo > executando->proximo_tempo){
                        fila_de_espera[k+1] = fila_de_espera[k];
                        k--;
                    }
                    fila_de_espera[k+1] = executando;
                }
                tam_fila_espera++;
                executando = NULL;
            }
        }
    }

    // Finaliza saída
    saida[strlen(saida)-1] = ']';

    tempo_final = tempo_atual;
    float uso_cpu = ((float)tempo_total_cpu / tempo_final) * 100;
    char temp[50];
    snprintf(temp, sizeof(temp), "UTILIZACAO CPU: %f\n", uso_cpu);
    fputs(temp, fp);
    snprintf(temp, sizeof(temp), "THROUGHPUT: %f\n", (float)qtd_processos / tempo_final);
    fputs(temp, fp);


    free(processos_futuros);
    free(fila_de_prontos);
    free(fila_de_espera);

    char *final = (char*) malloc(sizeof(saida) + 1);
    strcpy(final, saida);
    strcat(final, "\n");
    return final;
}


/**
 * @brief Algoritmo de escalonamento SJF
 * 
 * Algoritmo funciona semelhante ao FCFS porém antes de executar um determinado processo, ele procura por aquele
 * processo com o menor pico de CPU na fila de prontos para ser executado primeiro, calculando uso de CPU e throughput no final
 * 
 * @param processos Vetor com os processos do arquivo
 * @param qtd_processos Quantidade de processos no vetor
 * @param seq 1: Sequencial, 0: Paralelo
 * @param fp Arquivo para escrita
 */
char* algSJF(Processo processos[], unsigned short int qtd_processos, int seq, FILE *fp){
    // ALocação das filas
    Processo **processos_futuros = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_prontos = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_espera = (Processo**) malloc(qtd_processos * sizeof(Processo*));

    // Variáveis para métricas
    int tempo_total_cpu = 0;
    int tempo_final = 0;

    // Variáveis de controle
    int tam_processos_futuros = 0, tam_fila_prontos = 0, tam_fila_espera = 0;
    // Variável de controle para tempo I/O
    int liberacao_io = 0;

    // Insere na fila de processo futuros ordenado por tempo de submissão
    for(int i = 0; i < qtd_processos; i++){
        if(!i){
            processos_futuros[0] = &processos[0];
            processos_futuros[0]->tempo_restante = processos_futuros[0]->tempos[0];
            tam_processos_futuros++;
        } else {
            int j = i - 1;

            while(j >= 0 && processos_futuros[j]->instante_submissao > processos[i].instante_submissao){
                processos_futuros[j+1] = processos_futuros[j];
                j--;
            }

            processos_futuros[j+1] = &processos[i];
            tam_processos_futuros++;
        }
    }

    // Variáveis para armazenar a saída
    char saida[512], saida_temp[20];
    // Variável para indicar se CPU está livre
    int cpu_livre = 1;
    // Tempo atual iniciando com o primeiro instante de submissão na fila de processos futuros
    int tempo_atual = processos_futuros[0]->instante_submissao;

    // Cria ínicio da saída
    snprintf(saida, sizeof(saida), "SJF: %d[",tempo_atual);

    // Enquanto houver processos em alguma das filas
    while(tam_processos_futuros || tam_fila_espera || tam_fila_prontos){
        // Se houver processo em espera e seu tempo atingiu o ms atual
        while(tam_fila_espera && fila_de_espera[0]->proximo_tempo <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = fila_de_espera[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_fila_espera - 1; i++){
                fila_de_espera[i] = fila_de_espera[i + 1];
            }
            tam_fila_espera--;

        }

        // Se houver processos futuros e seu instante de chegada na CPU atingiu o tempo ms atual
        while(tam_processos_futuros && processos_futuros[0]->instante_submissao <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = processos_futuros[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_processos_futuros - 1; i++){
                processos_futuros[i] = processos_futuros[i + 1];
            }
            tam_processos_futuros--;
        }

        // Se a CPU estiver livre e houver processos na fila de prontos
        if(cpu_livre && tam_fila_prontos){
            // 1. Encontrar o índice do processo com o menor burst de CPU
            int idx_menor = 0;
            // Pega o burst do primeiro processo como referência inicial
            int menor_pico = fila_de_prontos[0]->tempos[fila_de_prontos[0]->indice_tempo];

            tempo_total_cpu += menor_pico;

            for (int i = 1; i < tam_fila_prontos; i++) {
                // Pega o próximo burst do processo 'i'
                int pico_atual = fila_de_prontos[i]->tempos[fila_de_prontos[i]->indice_tempo];
                
                // CORREÇÃO: Compara os bursts de CPU, não o 'proximo_tempo'
                if (pico_atual < menor_pico) {
                    menor_pico = pico_atual;
                    idx_menor = i;
                }
            }

            // 2. O processo atual é aquele no índice do menor pico encontrado
            Processo *processo_atual = fila_de_prontos[idx_menor];

            // 3. Remover o processo selecionado da fila de prontos
            for (int j = idx_menor; j < tam_fila_prontos - 1; j++) {
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
                processo_atual->termino = tempo_atual;
                processo_atual->turnaround = processo_atual->termino - processo_atual->instante_submissao;
                processo_atual = NULL;
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
    saida[strlen(saida)-1] = ']';

    tempo_final = tempo_atual;
    float uso_cpu = ((float)tempo_total_cpu / tempo_final) * 100;
    char temp[50];
    snprintf(temp, sizeof(temp), "UTILIZACAO CPU: %f\n", uso_cpu);
    fputs(temp, fp);
    snprintf(temp, sizeof(temp), "THROUGHPUT: %f\n", (float)qtd_processos / tempo_final);
    fputs(temp, fp);

    free(processos_futuros);
    free(fila_de_prontos);
    free(fila_de_espera);

    char *final = (char*) malloc(sizeof(saida) + 1);
    strcpy(final, saida);
    strcat(final, "\n");
    return final;
}


/**
 * @brief Algoritmo de escalonamento SRTF
 * 
 * Esse algoritmo escolhe o processo com o menor tempo restante, calculando uso de CPU e throughput no final
 * 
 * @param processos Vetor com os processos do arquivo
 * @param qtd_processos Quantidade de processos no vetor
 * @param seq 1: Sequencial, 0: Paralelo
 * @param fp Arquivo para escrita
 */
char* algSRTF(Processo processos[], unsigned short int qtd_processos, int seq, FILE *fp){
    // ALocação das filas
    Processo **processos_futuros = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_prontos = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_espera = (Processo**) malloc(qtd_processos * sizeof(Processo*));

    // Variáveis para métricas
    int tempo_total_cpu = 0;
    int tempo_final = 0;

    // Variáveis de controle
    int tam_processos_futuros = 0, tam_fila_prontos = 0, tam_fila_espera = 0;
    // Variável de controle para tempo I/O
    int liberacao_io = 0;
    
    // Insere na fila de processo futuros ordenado por tempo de submissão
    for(int i = 0; i < qtd_processos; i++){
        if(!i){
            processos_futuros[0] = &processos[0];
            tam_processos_futuros++;
        } else {
            int j = i - 1;

            while(j >= 0 && processos_futuros[j]->instante_submissao > processos[i].instante_submissao){
                processos_futuros[j+1] = processos_futuros[j];
                j--;
            }

            processos_futuros[j+1] = &processos[i];
            tam_processos_futuros++;
        }

        processos[i].indice_tempo = 0;
        processos[i].tempo_restante = processos[i].tempos[0];
    }

    // Variáveis para armazenar a saída
    char saida[512], saida_temp[50];
    Processo *executando = NULL;
    // Tempo atual iniciando com o primeiro instante de submissão na fila de processos futuros
    int tempo_atual = processos_futuros[0]->instante_submissao;

    // Cria ínicio da saída
    snprintf(saida, sizeof(saida), "SRTF: %d[",tempo_atual);

    // Enquanto houver processos em alguma das filas
    while(tam_processos_futuros || tam_fila_espera || tam_fila_prontos || executando){
        // Se houver processo em espera e seu tempo atingiu o ms atual
        while(tam_fila_espera && fila_de_espera[0]->proximo_tempo <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = fila_de_espera[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_fila_espera - 1; i++){
                fila_de_espera[i] = fila_de_espera[i + 1];
            }
            tam_fila_espera--;

        }

        // Se houver processos futuros e seu instante de chegada na CPU atingiu o tempo ms atual
        while(tam_processos_futuros && processos_futuros[0]->instante_submissao <= tempo_atual){
            fila_de_prontos[tam_fila_prontos] = processos_futuros[0];
            tam_fila_prontos++;

            for(int i = 0; i < tam_processos_futuros - 1; i++){
                processos_futuros[i] = processos_futuros[i + 1];
            }
            tam_processos_futuros--;
        }

        Processo *anterior = executando;

        if(executando != NULL){
            fila_de_prontos[tam_fila_prontos] = executando;
            tam_fila_prontos++;
        }

        if(tam_fila_prontos){
            int idx_menor = 0;
            for(int i = 1; i < tam_fila_prontos; i++){
                if(fila_de_prontos[i]->tempo_restante < fila_de_prontos[idx_menor]->tempo_restante){
                    idx_menor = i;
                }
            }

            executando =  fila_de_prontos[idx_menor];

            for(int i = idx_menor; i < tam_fila_prontos - 1; i++){
                fila_de_prontos[i] = fila_de_prontos[i + 1];
            }
            tam_fila_prontos--;
        } else {
            executando = NULL;
        }

        if(anterior != NULL && executando != anterior){
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", anterior->nome, tempo_atual);
            strcat(saida, saida_temp);
        }

        if(executando == NULL){
            // Se existir processos futuros, pega o instante de submissão do primeiro
            int proximo_evento_futuros = tam_processos_futuros > 0 ? processos_futuros[0]->instante_submissao : INT_MAX;
            // Se existir processos na fila de espera, pega o o tempo onde o ocorrerá a próxima execução
            int proximo_evento_espera = tam_fila_espera > 0 ? fila_de_espera[0]->proximo_tempo : INT_MAX;
            // Verifica qual vem primeiro
            int proximo_tempo_evento = proximo_evento_espera < proximo_evento_futuros ? proximo_evento_espera : proximo_evento_futuros;

            if(proximo_tempo_evento == INT_MAX) break;

            // Armazena na saída
            snprintf(saida_temp, sizeof(saida_temp), "*** %d|",proximo_tempo_evento);
            strcat(saida, saida_temp);

            // Atualiza o tempo atual ms para o tempo do processo mais próximo, seja na fila de espera ou na fila de processos futuros caso exista
            tempo_atual = proximo_tempo_evento;
            continue;
        }

        int proximo_evento = (tam_processos_futuros > 0) ? processos_futuros[0]->instante_submissao : INT_MAX;

        int proximo_espera = (tam_fila_espera > 0) ? fila_de_espera[0]->proximo_tempo : INT_MAX;

        int tempo_termino = (executando != NULL) ? (tempo_atual + executando->tempo_restante) : INT_MAX;

        int proximo_tempo = proximo_evento;

        if(proximo_espera < proximo_tempo){
            proximo_tempo = proximo_espera;
        }
        if(tempo_termino < proximo_tempo){
            proximo_tempo = tempo_termino;
        }

        if(proximo_tempo == INT_MAX) break;
    
        int diferenca = proximo_tempo - tempo_atual;

        if(executando){
            if(diferenca){
                executando->tempo_restante -= diferenca;
                tempo_total_cpu += diferenca;
            }
        } else {
            snprintf(saida_temp, sizeof(saida_temp), "*** %d|", proximo_tempo);
            strcat(saida, saida_temp);
        }
        

        tempo_atual = proximo_tempo;

        if(executando->tempo_restante <= 0){
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", executando->nome, tempo_atual);
            strcat(saida, saida_temp);

            executando->indice_tempo++;

            if(executando->indice_tempo >= executando->qtd_tempos){
                executando->termino = tempo_atual;
                executando->turnaround = executando->termino - executando->instante_submissao;
                executando = NULL;
            } else {
                int io = executando->tempos[executando->indice_tempo];
                executando->indice_tempo++;

                if(seq){
                    int inicio_io = (tempo_atual > liberacao_io) ? tempo_atual : liberacao_io;
                    executando->proximo_tempo = inicio_io + io;
                    liberacao_io = executando->proximo_tempo;
                } else {
                    executando->proximo_tempo = tempo_atual + io;
                }

                if(executando->indice_tempo < executando->qtd_tempos){
                    executando->tempo_restante = executando->tempos[executando->indice_tempo];
                }

                 if(!tam_fila_espera){
                    fila_de_espera[0] = executando;
                } else {
                    int k = tam_fila_espera-1;
                    while(k >= 0 && fila_de_espera[k]->proximo_tempo > executando->proximo_tempo){
                        fila_de_espera[k+1] = fila_de_espera[k];
                        k--;
                    }
                    fila_de_espera[k+1] = executando;
                }
                tam_fila_espera++;
                executando = NULL;
            }
        }
    }

    // Finaliza saída
    saida[strlen(saida)-1] = ']';

    tempo_final = tempo_atual;
    float uso_cpu = ((float)tempo_total_cpu / tempo_final) * 100;
    char temp[50];
    snprintf(temp, sizeof(temp), "UTILIZACAO CPU: %f\n", uso_cpu);
    fputs(temp, fp);
    snprintf(temp, sizeof(temp), "THROUGHPUT: %f\n", (float)qtd_processos / tempo_final);
    fputs(temp, fp);

    free(processos_futuros);
    free(fila_de_prontos);
    free(fila_de_espera);

    char *final = (char*) malloc(sizeof(saida) + 1);
    strcpy(final, saida);
    strcat(final, "\n");
    return final;
}

/**
 * @brief Algoritmo FCFS
 * 
 * O algoritmo FCFS executa processos na sua ordem de chegada, existindo a possibilidade de executar paralelamente ou sequencial,
 * calculando uso de CPU e throughput no final
 * 
 * @param processos Vetor com os processos existentes
 * @param qtd_processos Quantidadde de processos no vetor de processos
 * @param seq 1: Sequencial. 0: Paralelo
 * @param fp Arquivo para escrita
 */
char* algFCFS(Processo processos[], unsigned short int qtd_processos, int seq, FILE *fp){
    // Alocação da fila de processos futuros, fila de porntos e fila de espera seguindo a ideia do diagrama proposto em aula
    Processo **processos_futuros = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_prontos = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    Processo **fila_de_espera = (Processo**) malloc(qtd_processos * sizeof(Processo*));
    
    // Variáveis para métricas
    int tempo_total_cpu = 0;
    int tempo_final = 0;
    
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
            
            tempo_total_cpu += tempo_pico;
            
            // Calcula em qual ms vai encerrar o processo
            int tempo_ms_final = tempo_atual + tempo_pico;
            
            // Adiciona na saída
            snprintf(saida_temp, sizeof(saida_temp), "%s %d|", processo_atual->nome, tempo_ms_final);
            strcat(saida, saida_temp);
            
            // Avança no índice do vetor de tempos indo para um tempo I/O
            processo_atual->indice_tempo++;

            // Se for maior que a quantidade de tempos no vetor, indica que o processo terminou 
            if(processo_atual->indice_tempo >= processo_atual->qtd_tempos){
                // Calcula turnaround e libera CPU
                processo_atual->termino = tempo_ms_final;
                processo_atual->turnaround = processo_atual->termino - processo_atual->instante_submissao;
                processo_atual = NULL;
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
    saida[strlen(saida)-1] = ']';

    tempo_final = tempo_atual;
    float uso_cpu = ((float)tempo_total_cpu / tempo_final) * 100;
    char temp[50];
    snprintf(temp, sizeof(temp), "UTILIZACAO CPU: %f\n", uso_cpu);
    fputs(temp, fp);
    snprintf(temp, sizeof(temp), "THROUGHPUT: %f\n", (float)qtd_processos / tempo_final);
    fputs(temp, fp);

    // Desaloca vetores de fila
    free(processos_futuros);
    free(fila_de_prontos);
    free(fila_de_espera);

    char *final = (char*) malloc(sizeof(saida) + 1);
    strcpy(final, saida);
    strcat(final, "\n");
    return final;
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
 
/**
 * @brief Inicilizar processos
 * 
 * Procedimento que inicialzia todos os processos existentes e garante
 * que exista consistência entre as chamadas dos algoritmos sem um
 * altear o outro
 * 
 * @param processos Vetor de processos
 * @param qtd_processos Quantidade de processos no vetor
 */
void inicializar_processos(Processo processos[], unsigned short int qtd_processos){
    for(int i = 0; i < qtd_processos; i++){
        processos[i].indice_tempo = 0;
        processos[i].proximo_tempo = 0;
        processos[i].turnaround = 0;
        processos[i].termino = 0;
        processos[i].espera = 0;

        if(processos[i].qtd_tempos > 0){
            processos[i].tempo_restante = processos[i].tempos[0];
        } else {
            processos[i].tempo_restante = 0;
        }
    }
}

void escrever_arquivo(FILE *fp){

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
    unsigned int quantum = atoi(argv[2]);
    char saida[512], *temp = NULL;
    Processo *processos = (Processo*) malloc(qtd_processos * sizeof(Processo));
    snprintf(saida, sizeof(saida), "%s.out", argv[1]);
    FILE *fp = fopen(saida, "w");
    if(!fp){
        puts("[ -># ] Erro ao abrir o arquivo de escrita!");
        exit(EXIT_FAILURE);
    }

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
    
    puts("---------------------------------------------------------");
    puts("[ -> ] Executando algoritmo FCFS...                     |");
    inicializar_processos(processos, processos_existentes);
    puts("[ -> ] Execucao concluida!...                           |");
    temp = algFCFS(processos, processos_existentes, seq, fp);
    puts("[ -> ] Escrevendo resultado...                          |");
    fputs(temp, fp);
    calcula_turnaround(processos, processos_existentes, fp);
    calcula_tempo_espera(processos, processos_existentes, fp);
    free(temp);

    puts("---------------------------------------------------------");
    puts("[ -> ] Executando algoritmo SJF...                      |");
    inicializar_processos(processos, processos_existentes);
    temp = algSJF(processos, processos_existentes, seq, fp);
    puts("[ -> ] Execucao concluida!...                           |");
    puts("[ -> ] Escrevendo resultado...                          |");
    fputs(temp, fp);
    calcula_turnaround(processos, processos_existentes, fp);
    calcula_tempo_espera(processos, processos_existentes, fp);
    free(temp);

    puts("---------------------------------------------------------");
    puts("[ -> ] Executando algoritmo SRTF...                     |");
    inicializar_processos(processos, processos_existentes);
    temp = algSRTF(processos, processos_existentes, seq, fp);
    puts("[ -> ] Execucao concluida!...                           |");
    puts("[ -> ] Escrevendo resultado...                          |");
    fputs(temp, fp);
    calcula_turnaround(processos, processos_existentes, fp);
    calcula_tempo_espera(processos, processos_existentes, fp);
    free(temp);

    puts("---------------------------------------------------------");
    puts("[ -> ] Executando algoritmo de Prioridade Preemptivo... |");
    inicializar_processos(processos, processos_existentes);
    temp = algPrioridadePreemptivo(processos, processos_existentes, seq, fp);
    puts("[ -> ] Execucao concluida!...                           |");
    puts("[ -> ] Escrevendo resultado...                          |");
    fputs(temp, fp);
    calcula_turnaround(processos, processos_existentes, fp);
    calcula_tempo_espera(processos, processos_existentes, fp);
    free(temp);

    puts("---------------------------------------------------------");
    puts("[ -> ] Executando algoritmo Round-Robin...              |");
    inicializar_processos(processos, processos_existentes);
    temp = algRoundRobin(processos, processos_existentes, seq, quantum, fp);
    puts("[ -> ] Execucao concluida!...                           |");
    puts("[ -> ] Escrevendo resultado...                          |");
    fputs(temp, fp);
    calcula_turnaround(processos, processos_existentes, fp);
    calcula_tempo_espera(processos, processos_existentes, fp);
    puts("---------------------------------------------------------");
    free(temp);

    puts("[ -> ] Gravacao concluida");

    fclose(fp);

    // Libera todas as memórias
    for(unsigned short i = 0; i < processos_existentes; i++){
        free(processos[i].tempos);

    }
    free(processos);
    puts("[ -> ] Programa finalizado com sucesso!");
    return 0;
}