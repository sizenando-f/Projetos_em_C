#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int fifo_alg(int* seq_ref, int num_lidos, int quant_quadros, int* enderecos);
// int opt_alg(int* seq_ref, int num_lidos, int quant_quadros, int* enderecos, int maior_pag);
// int lru_alg(int* seq_ref, int num_lidos, int quant_quadros, int* enderecos);

/**
 * @brief Descobre se página está em quadro: Auxilixar para todos
 * 
 * Itera por todos os índices de quandro verificando a existência de uma página
 * 
 * @param pag Página a ser procurada
 * @param quadros Vetor de quadros
 * @param quant_quadros Tamanho do vetor de quadros
 * @param indice Para indicar qual índice está a página
 * 
 * @return 0 se não encontrado,
 * 1 se encontrado com indice da página
 */
int esta_em_quadro(int pag, int quadros[], int quant_quadros, int *indice){
    for(int i = 0; i < quant_quadros; i++){
        if(quadros[i] == pag){
            if(indice) *indice = i;
            return 1;
        }
    }
    return 0;
}

void libera_matriz(int **seq_futuras, int maior_pag) {
    if (seq_futuras == NULL) return;
    for (int i = 0; i < maior_pag; i++) {
        free(seq_futuras[i]);
    }
    free(seq_futuras);
}

int encontra_vitima(int quadros[], int **seq_fut, int pont_leitura[], int quant_quadros, int indice_atual){
    int maior_distancia = -1;
    int vitima = 0;

    for(int i = 0; i < quant_quadros; i++){
        int pag_no_quadro = quadros[i];
        int prox_uso = seq_fut[pag_no_quadro][pont_leitura[pag_no_quadro]] - indice_atual;

        if(prox_uso == 0){
            return i;
        }

        int distancia_futura = prox_uso - (indice_atual + 1);
        
        if(maior_distancia < distancia_futura){
            maior_distancia = distancia_futura;
            vitima = i;
        }
    }
    return vitima;
}

int aloca_indice_futuros(int ***seq_futuras, int seq_ref[], int num_lidos, int maior_pag){
    // Criação da tabela hash (matriz)
    int tam_real = maior_pag + 1;
    // Aloca vetor de ponteiros
    *seq_futuras = (int **) malloc(tam_real * sizeof(int *));
    // Verifica se alocou corretamente
    if(*seq_futuras == NULL){
        printf("(aloca_indices_futuros) Erro ao alocar seq_futuras...\n");
        return -1;
    } 

    // Aloca todas as colunas
    for(int i = 0; i < tam_real; i++){
        // Aloca coluna
        (*seq_futuras)[i] = (int *) malloc(num_lidos * sizeof(int));
        if((*seq_futuras)[i] == NULL){
            printf("(aloca_indices_futuros) Erro ao alocar coluna de seq_futuras...\n");
            for(int k = 0; k < i; k++){
                free((*seq_futuras)[k]);
            }
            free(*seq_futuras);
            *seq_futuras = NULL;
            return -1;
        }
    }
    // Término da criação da matriz

    // Para saber qual índice livre para colocar índices
    int *conta_indice = (int*) malloc(tam_real * sizeof(int));
    if (conta_indice == NULL) {
        printf("(aloca_indice_futuro) Erro na alocacao de conta_indice...\n");
        return -1; 
    }
    // Inicializa tudo com 0 (próximo indice livre)
    memset(conta_indice, 0, tam_real * sizeof(int));

    for(int i = 0; i < num_lidos; i++){
        // Pega página que está sendo lida
        int pag_atual = seq_ref[i];
        int pos_livre = conta_indice[pag_atual];

        // Adiciona na tabela hash na posição livre
        (*seq_futuras)[pag_atual][pos_livre] = i + 1;
        // Incrementa próxima posição livre
        conta_indice[pag_atual] = conta_indice[pag_atual] + 1;
    }

    free(conta_indice);
    return 0;
}

int opt_alg(int seq_ref[], int num_lidos, int quant_quadros, int enderecos[], int maior_pag){
    int **seq_futuras = NULL;
    if(aloca_indice_futuros(&seq_futuras, seq_ref, num_lidos, maior_pag+1) == -1){
        printf("(opt_alg) Erro ao alocar matriz...\n");
        return -1;
    }

    int *pont_leitura = (int*) malloc((maior_pag+1) * sizeof(int));
    int *quadros = (int*) malloc(quant_quadros * sizeof(int));

    // Armazena os erros
    int *enderecos_falha = (int*) malloc(num_lidos * sizeof(int));
    int *pag_falha = (int*) malloc(num_lidos * sizeof(int));

    if(pont_leitura == NULL || quadros == NULL || enderecos_falha == NULL || pag_falha == NULL){
        printf("(opt_alg) Erro na alocação de pont_leitura ou quadros ou endereços de falha ou páginas de falha...\n");
        libera_matriz(seq_futuras, maior_pag + 1);
        return -1;
    }

    memset(pont_leitura, 0, (maior_pag+1) * sizeof(int));
    memset(quadros, -1, quant_quadros * sizeof(int));

    int quadros_preenchidos = 0;
    int falta_de_pag = 0;

    for(int i = 0; i < num_lidos; i++){
        int elem = seq_ref[i];

        pont_leitura[elem] = pont_leitura[elem] + 1;

        if(esta_em_quadro(elem, quadros, quant_quadros, NULL)) continue;

        // Salva endereço e página que deram erro
        enderecos_falha[falta_de_pag] = enderecos[i];
        pag_falha[falta_de_pag++] = elem;

        if(quadros_preenchidos < quant_quadros){
            quadros[quadros_preenchidos++] = elem;
        } else {
            int vitima = encontra_vitima(quadros, seq_futuras, pont_leitura, quant_quadros, i);

            quadros[vitima] = elem;
        }
    }

    /**
     * TODO: Fazer a inserção dos resultados em out.txt com um arquivo
     * recebido por referência da main
     */

    // Desaloca antes de sair
    free(quadros);
    libera_matriz(seq_futuras, maior_pag + 1);
    free(enderecos_falha);
    free(pag_falha);
    free(pont_leitura);

    // Retorna o número de erros
    return falta_de_pag;
}

/**
 * @brief Procura página com menor tempo: Auxiliar para LRU
 * 
 * Itera pelo vetor de tempos de cada página procurando pelo menor entre eles
 * 
 * @param uso_recente Vetor de tempos de cada página
 * @param quant_quadros Tamanho do vetor de tempos
 * 
 * @return Índice da página com o menor tempo
 */
int menor_tempo(int uso_recente[], int quant_quadros){
    // Primeiro elemento com o menor por padrão
    int menor = uso_recente[0];
    int indice_menor = 0;

    // Procura outro menor ainda
    for(int i = 1; i < quant_quadros; i++){
        if(uso_recente[i] < menor){
            menor = uso_recente[i];
            indice_menor = i;
        }
    }

    return indice_menor;
}

/**
 * @brief Aplica algoritmo LRU substituindo a página menos recentemente usada
 * 
 * @param seq_ref Vetor de páginas
 * @param num_lidos Tamanho do vetor de páginas
 * @param quant_quadros Tamanho do vetor de quadros
 * @param enderecos Vetor de endereços acessados
 * 
 * @return Número de falta de páginas
 */
int lru_alg(int seq_ref[], int num_lidos, int quant_quadros, int enderecos[]){
    // Vetor para simbolizar memória física
    int *quadros = (int*) malloc(quant_quadros * sizeof(int));
    // Para saber quando cada quadro foi utilizado
    int *uso_recente = (int*) malloc(quant_quadros * sizeof(int));

    // Se alocação deu errado
    if(quadros == NULL || uso_recente == NULL){
        printf("(lru_alg) Erro ao alocar o vetor de quadros ou uso recente...\n");
        // Retorna -1 para indicar erro
        return -1;
    }

    // Inicializa a memória com -1 pra representar vazio
    memset(quadros, -1, quant_quadros * sizeof(int));
    // Inicializa o vetor de tempos
    memset(uso_recente, 0, quant_quadros * sizeof(int));

    // Quantos quadros já foram preenchidos
    int quadros_preenchidos = 0;
    // Para saber quanto tempo se passou
    int tempo = 0;
    // Quantas faltas de página ocorreram
    int falta_de_pag = 0;

    // Armazena os erros
    int *enderecos_falha = (int*) malloc(num_lidos * sizeof(int));
    int *pag_falha = (int*) malloc(num_lidos * sizeof(int));

    if(enderecos_falha == NULL || pag_falha == NULL){
        printf("(lru_alg) Erro na alocação dos vetores de armazenamento...\n");
        // Desaloca antes de sair
        free(quadros);
        free(uso_recente);
        // Retorna -1 para indicar erro
        return -1;
    }

    // Para saber qual índice a página foi encontrada
    int indice;
    for(int i = 0; i < num_lidos; i++){
        // Pega o elemento atual
        int elem = seq_ref[i];
        // Passa o tempo
        tempo++;
        // Verifica se já está no quadro
        if(esta_em_quadro(elem, quadros, quant_quadros, &indice)){
            // Armazena tempo do índice da página encontrada
            uso_recente[indice] = tempo;
            continue;
        }
        
        // Salva endereço e página que deram erro
        enderecos_falha[falta_de_pag] = enderecos[i];
        pag_falha[falta_de_pag++] = elem;

        // Se houver espaço na memória
        if(quadros_preenchidos < quant_quadros){
            // Insere elemento no espaço vazio
            quadros[quadros_preenchidos] = elem;
            // Salva em conjunto o tempo do elemento respectivo
            uso_recente[quadros_preenchidos++] = tempo;
        } else {
            // Descobre o menos recentemente usado
            int menor = menor_tempo(uso_recente, quant_quadros);
            // Substitui na memória colocando elemento no índice descoberto
            quadros[menor] = elem;
            // Armazena o tempo respectivo
            uso_recente[menor] = tempo;
        }
    }

    /**
     * TODO: Fazer a inserção dos resultados em out.txt com um arquivo
     * recebido por referência da main
     */

    // Desaloca antes de sair
    free(quadros);
    free(uso_recente);
    free(enderecos_falha);
    free(pag_falha);

    // Retorna o número de erros
    return falta_de_pag;
}

/**
 * @brief Aplica algoritmo FIFO
 * 
 * Itera pelas páginas inserindo na memória e substituindo o primeiro da fila quando necessário
 * 
 * @param seq_ref Vetor de páginas
 * @param num_lidos Tamanho do vetor de páginas
 * @param quant_quadros Tamanho do vetor de quadros
 * @param enderecos Vetor com endereços acessados
 * 
 * @return Número de erros
 */
int fifo_alg(int seq_ref[], int num_lidos, int quant_quadros, int enderecos[]){
    // Vetor para simbolizar memória física
    int *quadros = (int*) malloc(quant_quadros * sizeof(int));
    // Se alocação deu errado
    if(quadros == NULL){
        printf("(fifo_alg) Erro ao alocar o vetor de quadros...\n");
        // Retorna -1 para indicar erro
        return -1;
    }
    // Inicializa o vetor com -1 pra representar vazio
    memset(quadros, -1, quant_quadros * sizeof(int));
    // Indica qual sofrerá substituição
    int prox_quadro_subs = 0;
    // Quantas faltas de página ocorreram
    int falta_de_pag = 0;
    // Quantos quadros já foram preenchidos
    int quadros_preenchidos = 0;

    // Armazena os erros
    int *enderecos_falha = (int*) malloc(num_lidos * sizeof(int));
    int *pag_falha = (int*) malloc(num_lidos * sizeof(int));

    if(enderecos_falha == NULL || pag_falha == NULL){
        printf("(fifo_alg) Erro na alocação dos vetores de armazenamento...\n");
        // Desaloca antes de sair
        free(quadros);
        // Retorna -1 para indicar erro
        return -1;
    }

    for(int i = 0; i < num_lidos; i++){
        // Pega o elemento atual
        int elem = seq_ref[i];
        // Verifica se já está no quadro
        if(esta_em_quadro(elem, quadros, quant_quadros, NULL)){
            continue;
        }

        // Salva endereço e página que deram erro
        enderecos_falha[falta_de_pag] = enderecos[i];
        pag_falha[falta_de_pag++] = elem;
        // Incrementa um erro

        // Verifica se tem espaço no vetor de quadros
        if(quadros_preenchidos < quant_quadros){
            // Insere elemento no espaço vazioS
            quadros[quadros_preenchidos++] = elem;
        } else {
            // Substitui na memória colocando a nova página
            quadros[prox_quadro_subs] = elem;
            // Incrementa o contador de substituição
            prox_quadro_subs = (prox_quadro_subs + 1) % quant_quadros;
        }
    }

    /**
     * TODO: Fazer a inserção dos resultados em out.txt com um arquivo
     * recebido por referência da main
     */

    // Desaloca antes de sair
    free(quadros);
    free(enderecos_falha);
    free(pag_falha);

    // Retorna o número de erros
    return falta_de_pag;
}

int main(int argc, char *argv[]) {
    // Verifica a entrada de parâmetros
    if (argc != 4) {
        printf("Uso: %s <tamanho da pagina> <tamanho da memoria> <nome do arquivo de entrada>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo de entrada
    FILE *arquivo = fopen(argv[3], "r");
    if (arquivo == NULL) {
        printf("(main) Erro ao abrir o arquivo '%s'...\n", argv[3]);
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
        printf("(main) Erro na alocacao de memoria fisica...\n");
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
        printf("(main) Erro na alocacao inicial dos vetores dinamicos...\n");
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
                printf("(main) Erro na realocacao do vetor de enderecos...\n");
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
                printf("(main) Erro na realocacao do vetor de sequencia de referencia...\n");
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