#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int fifo_alg(int* seq_ref, int num_lidos, int quant_quadros, int* enderecos);
// int opt_alg(int* seq_ref, int num_lidos, int quant_quadros, int* enderecos, int maior_pag);
// int lru_alg(int* seq_ref, int num_lidos, int quant_quadros, int* enderecos);

int main(int argc, char *argv[]) {
    // Verifica a entrada de parâmetros
    if (argc != 4) {
        printf("Uso: %s <tamanho da pagina> <tamanho da memoria> <nome do arquivo de entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo de entrada
    FILE *arquivo = fopen(argv[3], "r");
    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo '%s'\n", argv[3]);
        return 1;
    }

    // Converte os valores lidos
    int tam_pag = atoi(argv[1]);
    int tam_mem = atoi(argv[2]);
    // Realiza o cálculo de quantos quadros estão disponíveis
    int quant_quadros = tam_mem / tam_pag;

    // Vetor para representar a memória física
    int *mem_fisica = (int*) malloc(quant_quadros * sizeof(int));
    if (mem_fisica == NULL) {
        printf("Alocacao de memoria fisica falhou...\n");
         // Fecha o arquivo antes de sair
        fclose(arquivo);
        return 1;
    }
    // Inicializa o vetor com -1 pra representar vazio
    memset(mem_fisica, -1, quant_quadros * sizeof(int));

    // Aloca os vetores dinâmicos com um tamanho inicial
    int capacidade = 20;
    int *enderecos = (int*) malloc(capacidade * sizeof(int));
    int *seq_ref = (int*) malloc(capacidade * sizeof(int));

    // Verifica se algum não foi alocado corretamente
    if (enderecos == NULL || seq_ref == NULL) {
        printf("Falha na alocacao inicial dos vetores dinamicos...\n");
        fclose(arquivo);
        free(mem_fisica);
        free(enderecos); // free(NULL) é seguro, não há problema em chamar
        free(seq_ref);
        return 1;
    }

    // Para saber quantos números foram lidos
    int num_lidos = 0;
    // Para criar a tabela hash
    int maior_pag = -1;
    // Valor atual lido
    int valor;

    while (fscanf(arquivo, "%d", &valor) != EOF) {
        // Se a capacidade estourou, realoca os vetores
        if (num_lidos == capacidade) {
            // Aumenta a capacidade
            capacidade += 20; 

            int *temp_enderecos = (int*) realloc(enderecos, capacidade * sizeof(int));
            if (temp_enderecos == NULL) {
                printf("Falha na realocacao do vetor de enderecos...\n");
                // Libera tudo antes de sair
                free(mem_fisica);
                free(enderecos);
                free(seq_ref);
                fclose(arquivo);
                return 1;
            }
            enderecos = temp_enderecos;

            int *temp_seq_ref = (int*) realloc(seq_ref, capacidade * sizeof(int));
            if (temp_seq_ref == NULL) {
                printf("Falha na realocacao do vetor de sequencia de referencia...\n");
                free(mem_fisica);
                free(enderecos);
                free(seq_ref);
                fclose(arquivo);
                return 1;
            }
            seq_ref = temp_seq_ref;
        }

        // Realiza o cálaculo do número da página
        int num_pagina = valor / tam_pag;
        enderecos[num_lidos] = valor;
        seq_ref[num_lidos] = num_pagina;
        num_lidos++;
        // Substitui a maior página se necessário
        if (num_pagina > maior_pag) {
            maior_pag = num_pagina;
        }
    }
    
    /* 
    int falta_fifo = fifo_alg(seq_ref, num_lidos, quant_quadros, enderecos);
    int falta_opt = opt_alg(seq_ref, num_lidos, quant_quadros, enderecos, maior_pag);
    int falta_lru = lru_alg(seq_ref, num_lidos, quant_quadros, enderecos);

    printf("FIFO: %d page faults - %.2f%% de taxa de falha.\n", falta_fifo, ((float)falta_fifo / num_lidos) * 100.0);
    printf("OPT: %d page faults - %.2f%% de taxa de falha.\n", falta_opt, ((float)falta_opt / num_lidos) * 100.0);
    printf("LRU: %d page faults - %.2f%% de taxa de falha.\n", falta_lru, ((float)falta_lru / num_lidos) * 100.0);
    */

    // Pra saber se tá funcionando
    printf("Arquivo lido com sucesso!\n");
    printf("Total de enderecos lidos: %d\n", num_lidos);
    printf("Total de quadros de memoria: %d\n", quant_quadros);
    printf("Maior pagina referenciada: %d\n", maior_pag);


    // Limpeza final
    fclose(arquivo);
    free(mem_fisica);
    free(enderecos);
    free(seq_ref);
    return 0;
}