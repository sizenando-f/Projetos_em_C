/**
 * @file main.c
 * 
 * Esse arquivo contém o trabalho de SO implementando sobre substituição
 * de páginas na memória. Os algoritmos implementados foram:
 * FIFO
 * LRU
 * OPT
 * 
 * O pseudocódigo desenvolvido para implementar o código se encontra no final
 * 
 * @author Sizenando S. França | RGM: 50575
 * @date 17-10-2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Descobre se página está em quadro: Auxilixar para todos
 * 
 * Itera por todos os índices do quadro verificando a existência de uma página
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

/**
 * @brief Desaloca matriz: Auxiliar para OPT
 * 
 * Essa função desaloca por completo uma matriz qualquer
 * 
 * @param seq_futuras Matriz a ser desalocada
 * @param maior_pag Tamanho da matriz
 */
void libera_matriz(int **seq_futuras, int maior_pag) {
    if (seq_futuras == NULL) return;
    for (int i = 0; i < maior_pag + 1; i++) {
        free(seq_futuras[i]);
    }
    free(seq_futuras);
}

/**
 * @brief Encontra melhor vítima para ser substituída: Auxiliar para OPT
 * 
 * Realiza o calculo de distância buscando aquela página que nunca
 * mais será usada ou aquela que vai demorar mais para ser usada
 * 
 * @param quadros Vetor de quadros da memória
 * @param seq_fut Vetor de sequências futuras
 * @param pont_leitura Vetor que indica onde ler em cada página
 * @param quant_quadros Tamanho de vetor de quadros
 * @param indice_atual Indice da página a ser procurado
 * 
 * @return Melhor vítima
 */
int encontra_vitima(int quadros[], int **seq_fut, int pont_leitura[], int quant_quadros, int indice_atual){
    int maior_distancia = -1;
    int vitima = 0;

    for(int i = 0; i < quant_quadros; i++){
        int pag_no_quadro = quadros[i];

        // Descobre quando será o próximo uso da página
        int prox_uso = seq_fut[pag_no_quadro][pont_leitura[pag_no_quadro]];

        // Se ela nunca mais aparecer
        if(prox_uso == 0){
            return i;
        }

        // Se não foi a vítima perfeita, calcula a distância
        int distancia_futura = prox_uso - (indice_atual + 1);
        
        if(maior_distancia < distancia_futura){
            maior_distancia = distancia_futura;
            vitima = i;
        }
    }
    return vitima;
}

/**
 * @brief Cria a tabela hash
 * 
 * Cria a tabela hash necessária para realizar a pesquisa da
 * melhor vítima de forma rápida
 * 
 * @param seq_futuras Ponteiro para vetor de ponteiros
 * @param seq_ref Vetor de páginas
 * @param num_lidos Tamanho do vetor de páginas
 * @param maior_pag Maior página existente
 * 
 * @return -1 se ocorreu algum erro, 0 se tudo deu certo
 */
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
        // Aloca coluna com memória zerada
        (*seq_futuras)[i] = (int *) calloc(num_lidos + 1, sizeof(int));
        if((*seq_futuras)[i] == NULL){
            printf("(aloca_indices_futuros) Erro ao alocar coluna de seq_futuras...\n");
            // Libera matriz
            libera_matriz(*seq_futuras, i - 1);
            *seq_futuras = NULL;
            return -1;
        }
    }
    // Término da criação da matriz

    // Para saber qual índice livre para colocar índices
    int *conta_indice = (int*) calloc(tam_real, sizeof(int));
    if (conta_indice == NULL) {
        printf("(aloca_indice_futuro) Erro na alocacao de conta_indice...\n");
        libera_matriz(*seq_futuras, maior_pag);
        return -1; 
    }

    for(int i = 0; i < num_lidos; i++){
        // Pega página que está sendo lida
        int pag_atual = seq_ref[i];
        int pos_livre = conta_indice[pag_atual];

        // Adiciona na tabela hash na posição livre
        (*seq_futuras)[pag_atual][pos_livre] = i + 1;
        // Incrementa próxima posição livre
        conta_indice[pag_atual]++;
    }

    free(conta_indice);
    return 0;
}

/**
 * @brief  Aplica algoritmo OPT
 * 
 * Realiza a substituição em páginas que não serão mais usadas ou que
 * demorarão muito para serem usadas novamente
 * 
 * @param seq_ref Vetor de páginas
 * @param num_lidos Tamanho do vetor de páginas
 * @param quant_quadros Tamanho do vetor de quadros
 * @param enderecos Vetor de endereços acessados
 * @param maior_pag Maior página existente
 * @param erros_out Arquivo para escrever os erros
 * 
 * @return Número de falta de páginas
 */
int opt_alg(int seq_ref[], int num_lidos, int quant_quadros, int enderecos[], int maior_pag, FILE *erros_out){
    int **seq_futuras = NULL;
    if(aloca_indice_futuros(&seq_futuras, seq_ref, num_lidos, maior_pag) != 0){
        printf("(opt_alg) Erro ao alocar matriz...\n");
        return -1;
    }

    int *pont_leitura = (int*) calloc(maior_pag + 1, sizeof(int));
    int *quadros = (int*) malloc(quant_quadros * sizeof(int));

    // Armazena os erros
    int *enderecos_falha = (int*) malloc(num_lidos * sizeof(int));
    int *pag_falha = (int*) malloc(num_lidos * sizeof(int));

    if(pont_leitura == NULL || quadros == NULL || enderecos_falha == NULL || pag_falha == NULL){
        printf("(opt_alg) Erro na alocação de memória...\n");
        libera_matriz(seq_futuras, maior_pag);
        free(pont_leitura);
        free(quadros);
        free(enderecos_falha);
        free(pag_falha);
        return -1;
    }

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

    // Escreve os erros no arquivo
    for(int i = 0; i < falta_de_pag; i++){
        fprintf(erros_out, "erro de pagina endereço :%d pagina: %d\n", enderecos_falha[i], pag_falha[i]);
    }

    // Desaloca antes de sair
    free(quadros);
    libera_matriz(seq_futuras, maior_pag);
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
 * @param erros_out Arquivo para escrever os erros
 * 
 * @return Número de falta de páginas
 */
int lru_alg(int seq_ref[], int num_lidos, int quant_quadros, int enderecos[], FILE *erros_out){
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

    // Escreve os erros no arquivo
    for(int i = 0; i < falta_de_pag; i++){
        fprintf(erros_out, "erro de pagina endereço :%d pagina: %d\n", enderecos_falha[i], pag_falha[i]);
    }

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
 * @param erros_out Arquivo para escrever os erros
 * 
 * @return Número de erros
 */
int fifo_alg(int seq_ref[], int num_lidos, int quant_quadros, int enderecos[], FILE *erros_out){
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

    // Escreve os erros no arquivo
    for(int i = 0; i < falta_de_pag; i++){
        fprintf(erros_out, "erro de pagina endereço :%d pagina: %d\n", enderecos_falha[i], pag_falha[i]);
    }

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

    // Aloca os vetores dinâmicos com um tamanho inicial
    int capacidade = 20;
    int *enderecos = (int*) malloc(capacidade * sizeof(int));
    int *seq_ref = (int*) malloc(capacidade * sizeof(int));

    // Verifica se algum não foi alocado corretamente
    if (enderecos == NULL || seq_ref == NULL) {
        printf("(main) Erro na alocacao inicial dos vetores dinamicos...\n");
        fclose(arquivo);
        free(enderecos);
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
                free(enderecos);
                free(seq_ref);
                fclose(arquivo);
                return 1;
            }
            enderecos = temp_enderecos;

            int *temp_seq_ref = (int*) realloc(seq_ref, capacidade * sizeof(int));
            if (temp_seq_ref == NULL) {
                printf("(main) Erro na realocacao do vetor de sequencia de referencia...\n");
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
    
    // Fecha arquivo de entrada
    fclose(arquivo);

    // Abre arquivo de saída
    FILE *erros_out = fopen("erros.out", "w");
    if(erros_out == NULL){
        printf("(main) Erro ao criar arquivo de saida 'erros.out'...\n");
        free(enderecos);
        free(seq_ref);
        return 1;
    }

    // Executa os algoritmos e escreve no arquivo
    fprintf(erros_out, "FIFO:\n");
    int falta_fifo = fifo_alg(seq_ref, num_lidos, quant_quadros, enderecos, erros_out);
    
    fprintf(erros_out, "\nOPT:\n");
    int falta_opt = opt_alg(seq_ref, num_lidos, quant_quadros, enderecos, maior_pag, erros_out);
    
    fprintf(erros_out, "\nLRU:\n");
    int falta_lru = lru_alg(seq_ref, num_lidos, quant_quadros, enderecos, erros_out);

    // Fecha arquivo de saída
    fclose(erros_out);

    // Exibe os resultados na tela
    printf("FIFO: %d page faults - %.2f%% de taxa de falha.\n", falta_fifo, ((float)falta_fifo / num_lidos) * 100.0);
    printf("OPT: %d page faults - %.2f%% de taxa de falha.\n", falta_opt, ((float)falta_opt / num_lidos) * 100.0);
    printf("LRU: %d page faults - %.2f%% de taxa de falha.\n", falta_lru, ((float)falta_lru / num_lidos) * 100.0);

    // Limpeza final
    free(enderecos);
    free(seq_ref);
    return 0;
}

/*
=== PSEUDOCÓDIGO ===
Obs: A única coisa não presente é a escrita do arquivo se saida erros_out
A escolha da implementação da tabela hash foi necessário porque a procura bruta índice por índice estava deixando a execução extremamente lenta
Em casos extremos ao lidar com 1 milhão de valores, o tempo de execução caiu de 1 minutos e 40 segundos para 1 milissegundo

# Função auxiliar para todas as outras
função esta_em_quadro(pagina, quadros, quantidade_de_quadros, indice -> referência): inteiro
	# Verifica no vetor de quadros se página já existe
	para i de 1 ate quantidade_de_quadros faca
		se quadros[i] = pagina então
			indice <- i
			retorne 1
		fimse
	fimpara
	retorne 0
fimfuncao

funcao encontra_vitima(quadros, sequencias_futuras, ponteiros_de_leitura, quantidade_de_quadros, indice_atual)
	maior_distancia <- -1
	vitima <- 1
	para i de 1 ate quantidade_de_quadros faca
		distancia_futura <- sequencias_futuras[quadros[i]][ponteiros_de_leitura[quadros[i]]] - indice_atual
		se distancia_futura <= 0 entao
			retorne i
		senao se maior_distancia < distancia_futura entao
			maior_distancia <- distancia_futura
			vitima <- i
		fimse
	fimpara
	retorne vitima
fimfuncao

funcao aloca_indice_futuros(sequencia_referencia, numeros_lidos, maior_pagina): matriz
	# Aloca a a tabela hash com o tamanho da maior página para não ter vazamento de memmória
	sequencias_futuras[maior_pagina][numeros_lidos]
	# Para contar qual proximo indice livre pra colocar os indices, incializa todos com 1
	conta_indice[maior_pagina] <- 1
	para i de 1 ate numeros_lidos faca
		# Pega a página que está sendo lida
		pagina_atual <- sequencia_referencia[i]
		# Adiciona na tabela hash na posição livre
		sequencias_futuras[pagina_atual][conta_indice[pagina_atual]] <- i
		# Incrementa a próxima posição livre referente a página lida
		conta_indice[pagina_atual] <- conta_indice[pagina_atual] + 1
	fimpara

	retorna sequencias_futuras
fimfuncao

funcao opt_alg(sequencia_referencia, numeros_lidos, quantidade_de_quadros, enderecos, maior_pagina): inteiro
	sequencias_futuras <- aloca_indice_futuros(sequencias_futuras, sequencia_referencia. numeros_lidos, maior_pagina)

	ponteiros_de_leitura[maior_pagina] <- 0 # Inicializa todos com 0, indica que é o primeiro

	# Inicializa o vetor de quadros com -1 (vazio)
	quadros[quantidade_de_quadros] <- -1
	# Para saber quantos quadros foram preenchidos
	quadros_preenchidos <- 0
	# Para saber quantos erros ocorreram
	falta_de_pagina <- 0

	para i de 1 ate numeros_lidos faca
		# Escolhe o elemento atual
		elemento <- sequencia_referencia[i]
		# Para ler na busca da vítima
		ponteiros_de_leitura[elemento] <- ponteiros_de_leitura[elemento] + 1
		# Se estiver no quadro apenas ignora
		se esta_em_quadro(elemento, quadros, quantidade_de_quadros, LAMBDA) então
			continue
		senão
			# Incrementa um erro
			falta_de_pagina <- falta_de_pagina + 1
			# Insere endereço e página que deram erro
			endereços_falha.insere(enderecos[i])
			paginas_falha.insere(elemento)
			# Se houver espaço no vetor de quadros
			se quadros_preenchidos < quantidade_de_quadros então
				# quadros_preenchidos + 1 pois começa em 0
				quadros[quadros_preenchidos + 1] <- elemento
				# Incrementa o contador de quadros
				quadros_preenchidos <- quadros_preenchidos + 1
			senao
				# Encontra a melhor vítima para ser substituida
				vitima <- encontra_vitima(sequencias_futuras, quadros, quantidade_de_quadros, i, numeros_lidos)
				# Substitui na memória colocando a nova página
				quadros[vitima] <- elemento
			fimse
		fimse
		
	fimpara
fimfuncao

funcao menor_tempo(uso_recente, quantidade_de_quadros): inteiro
	# Define o primeiro elemento do vetor como o menor
	menor <- uso_recente[1]
	indice_menor <- 1

	para i de 2 ate quantidade_de_quadros faca
		se uso_recente[i] < menor entao
			menor <- uso_recente[i]
			indice_menor <- i
		fimse
	fimpara

	retorne indice_menor
fimfuncao

função lru_alg(sequencia_referencia, numeros_lidos, quantidade_de_quadros, enderecos): inteiro
	# Inicializa o vetor de quadros com -1 (vazio)
	quadros[quantidade_de_quadros] <- -1
	# Para saber quando cada quadro foi utilizado
	uso_recente[quantidade_de_quadros] <- 0
	# Para saber quantos quadros foram preenchidos
	quadros_preenchidos <- 0
	# Para saber quanto tempo se passou
	tempo <- 0
	# Para saber quantos erros ocorreram
	falta_de_pagina <- 0

	# Vetores para armazenar erros
	enderecos_falha[numeros_lidos]
	paginas_falha[numeros_lidos]

	para i de 1 ate numeros_lidos faca
		# Escolhe o elemento atual
		elemento <- sequencia_referencia[i]
		# Passa o tempo
		tempo <- tempo + 1
		# Se estiver no quadro apenas ignora
		se esta_em_quadro(elemento, quadros, quantidade_de_quadros, indice) então
			uso_recente[indice] <- tempo
			continue
		senão
			# Incrementa um erro
			falta_de_pagina <- falta_de_pagina + 1
			# Insere endereço e página que deram erro
			endereços_falha.insere(enderecos[i])
			paginas_falha.insere(elemento)
			# Se houver espaço no vetor de quadros
			se quadros_preenchidos < quantidade_de_quadros então
				# quadros_preenchidos + 1 pois começa em 0
				quadros[quadros_preenchidos + 1] <- elemento
				# Atualiza o tempo de uso da página
				uso_recente[quadros_preenchidos + 1] <- tempo
				# Incrementa o contador de quadros
				quadros_preenchidos <- quadros_preenchidos + 1
			senao	
				# Descobre o menos recentemente usado
				menor <- menor_tempo(uso_recente, quantidade_de_quadros)
				# Substitui na memória colocando a nova página
				quadros[menor] <- elemento
				# Atualiza o tempo correspondente
				uso_recente[menor] <- tempo
			fimse
		fimse
	fimpara

	retorne falta_de_pagina
fimfuncao

função fifo_alg(sequencia_referencia, numeros_lidos, quantidade_de_quadros, enderecos): inteiro
	# Inicializa o vetor de quadros com -1 (vazio)
	quadros[quantidade_de_quadros] <- -1
	# Indica qual sofrerá substituição, iniciando com 1 (primeira posição em pseudoalg)
	proximo_quadro_substituir <- 1
	# Para saber quantas faltas de páginas ocorreram
	falta_de_pagina <- 0
	# Para indicar quantos quadros já foram preenchidos
	quadros_preenchidos <- 0

	# Vetores para armazenar erros
	enderecos_falha[numeros_lidos]
	paginas_falha[numeros_lidos]

	para i de 1 ate numeros_lidos faca
		# Escolhe o elemento atual
		elemento <- sequencia_referencia[i]
		# Se estiver no quadro apenas ignora
		se esta_em_quadro(elemento, quadros, quantidade_de_quadros, LAMBDA) então
			continue
		senão
			# Incrementa um erro
			falta_de_pagina <- falta_de_pagina + 1
			# Insere endereço e página que deram erro
			endereços_falha.insere(enderecos[i])
			paginas_falha.insere(elemento)
			# Se houver espaço no vetor de quadros
			se quadros_preenchidos < quantidade_de_quadros então
				# quadros_preenchidos + 1 pois começa em 0
				quadros[quadros_preenchidos + 1] <- elemento
				# Incrementa o contador de quadros
				quadros_preenchidos <- quadros_preenchidos + 1
			senao				
				# Substitui na memória colocando a nova página
				quadros[proximo_quadro_substituir] <- elemento
				# Incrementa o contador de quadro para substiuição
				proximo_quadro_substituir <- (proximo_quadro_substituir + 1) % quantidade_de_quadros
			fimse
		fimse
	fimpara
	
	# Retorna o número de erros
	retorne falta_de_pagina
fimfuncao

algoritmo
	se argc != 4 então
		escreve("Quantidade inválida de parâmetros")
		retorne
	senão
		tamanho_da_pagina <- inteiro(argumento[1])
		tamanho_da_memoria <- inteiro(argumento[2])
		
		quantidade_de_quadros <- chão(tamanho_da_memoria/tamanho_da_pagina)
		
		memoria_física[quantidade_de_quadros] <- -1
		enderecos[]
		maior_pagina <- -1

		arquivo <- abrirArquivo(argumento[3])
		numeros_lidos <- 0
			
		enquanto(valor <- existirElemento(arquivo)) faca
			endereços.insere(valor)
			numero_da_pagina <- chão(valor/tamanho_da_página)
			sequencia_referencia.insere(numero_da_pagina)
			numeros_lidos <- numeros_lidos + 1
			se numero_da_pagina > maior_pagina entao
				maior_pagina <- numero_da_pagina
			fimse
		fimenquanto
		
		fechaArquivo(arquivo)

		falta_fifo <- fifo_alg(sequencia_referencia, numeros_lidos, quantidade_de_quadros, enderecos)
		falta_opt <- opt_alg(sequencia_referencia, numeros_lidos, quantidade_de_quadros, enderecos, maior_pagina)
		falta_lru <- lru_alg(sequencia_referencia, numeros_lidos, quantidade_de_quadros, enderecos)

		escreve(falta_fifo, (falta_fifo/numeros_lidos) * 100)
		escreve(falta_opt, (falta_opt/numeros_lidos) * 100)
		escreve(falta_lru, (falta_lru/numeros_lidos) * 100)
	fimse
fimalgoritmo
*/