/**
    * @file main.c
    * @brief Programa para exibir processos em formato de árvore e executar comandos no terminal.
    *
    * Este programa lê os dados de processos do diretório /proc e exibe os processos em formato de árvore.
    * Também permite a execução de comandos no terminal, incluindo a execução em segundo plano.
    *
    * @author Sizenando S. França
    * @date 16-04-2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

struct Process {    // Estrutura para armazenar os dados do processo
    int pid;    // PID do processo
    int ppid;   // PID do processo pai
    char estado;    // Estado do processo
    char nome[256]; // Nome do processo
};

/**
    * @brief Lê os dados do processo a partir do arquivo /proc/[pid]/stat.
    *
    * Esta função lê os dados do processo a partir do arquivo /proc/[pid]/stat
    * e armazena as informações em uma estrutura de processos.
    *
    * @param pid PID do processo a ser lido.
    * @param processos Array de estruturas Process para armazenar os dados dos processos.
    * @param nProcessos Ponteiro para o número total de processos lidos.
    * @return void
*/
void le_processos(int pid, struct Process *processos, int *nProcessos){ // Lê os dados do processo a partir do /proc/[pid]/stat
    char path[256]; // Buffer para guardar o caminho do arquivo
    snprintf(path, sizeof(path), "/proc/%d/stat", pid); // Cria o caminho do arquivo
    FILE *fp = fopen(path, "r");    // Abre o arquivo para leitura
    if(!fp){    //  Verifica se o arquivo foi aberto corretamente
        perror("Erro: O PID fornecido não existe.");
        return;
    }

    struct Process p;   // Cria uma variável do tipo Process
    fscanf(fp, "%d %s %c %d", &p.pid, p.nome, &p.estado, &p.ppid);  // Lê os dados do processo do arquivo
    // printf("PID: %d, Nome: %s, Estado: %c, PPID: %d\n", p.pid, p.nome, p.estado, p.ppid); // Exibe os dados do processo

    processos[*nProcessos] = p; //  Armazena os dados do processo na estrutura
    (*nProcessos)++;    //  Incrementa o número de processos lidos
    fclose(fp); // Fecha o arquivo
}

/**
    * @brief Inicializa a estrutura de processos.
    *
    * Esta função lê os dados de todos os processos do diretório /proc e armazena
    * as informações em uma estrutura de processos.
    *
    * @param processos Array de estruturas Process para armazenar os dados dos processos.
    * @param nProcessos Ponteiro para o número total de processos lidos.
    * @return void  
    * @note Se algum processo não aparecer na árvore, então o processo não existe na pasta /proc
*/
void inicializa_processos(struct Process *processos, int *nProcessos){
    DIR *dir = opendir("/proc"); // Abre o diretório /proc

    if(dir == NULL){ // Verifica se o diretório foi aberto corretamente
        perror("Erro: Não foi possível abrir o diretório /proc.");
        return;
    }

    struct dirent *diretorio; // Estrutura para guardar os dados do diretório
    while((diretorio = readdir(dir)) != NULL){ // Lê os dados do diretório
        if(diretorio->d_type == DT_DIR){ // Verifica se o arquivo é um diretório
            int pid = atoi(diretorio->d_name); // Converte o nome do diretório para inteiro
            if(pid > 0){ // Verifica se o PID é válido
                le_processos(pid, processos, nProcessos); // Lê os dados do processo
            }
        }
    }
    closedir(dir); // Fecha o diretório
}

/**
    * @brief Exibe os processos em formato de árvore.
    *
    * Esta função exibe os processos em formato de árvore, começando pelo PID fornecido.
    * A função é chamada recursivamente para exibir os processos filhos.
    *
    * @param ppid PID do processo pai.
    * @param nivel Nível atual na árvore de processos.
    * @param processos Array de estruturas Process contendo os dados dos processos.
    * @param nProcessos Número total de processos lidos.
    * @return void
*/
void exibir_processos(int ppid, int nivel, struct Process *processos, int nProcessos){
    int nivelAtual = nivel * 4; // Nível atual na árvore de processos
    for(int i = 0; i < nProcessos; i++){   // Percorre todos os processos lidos
        if(ppid == processos[i].pid){   // Se o PID do processo for igual ao PID fornecido
            printf("%*s", nivelAtual, ""); // Indenta a saída
            printf("----- %s %d %c %d\n", processos[i].nome, processos[i].pid, processos[i].estado, processos[i].ppid); // Exibe os dados do processo
            break;
        }
    }
    
    for(int i = 0; i < nProcessos; i++){    // Percorre todos os processos lidos
        if(ppid == processos[i].ppid){  // Se o PID do processo pai for igual ao PID fornecido
            printf("%*s", nivelAtual + 4, ""); // Indenta a saída
            printf("|\n");  // Exibe a linha de conexão entre o processo pai e o filho
            exibir_processos(processos[i].pid, nivel + 1, processos, nProcessos); // Chama a função recursivamente para exibir os processos filhos
        }
    }   
}


/**
 * @brief Executa o comando "tree" para exibir os processos em formato de árvore.
 *
 * Esta função inicializa a estrutura de processos, verifica se um PID válido foi fornecido
 * e exibe os processos em formato de árvore a partir do PID fornecido.
 *
 * @param token Ponteiro para o token que contém o comando e os argumentos.
 * @return void
 */
void execucao_tree(char *token){
    int nProcessos = 0; // Número de processos lidos
    struct Process processos[5000]; // Array de processos (Aumentar se necessário)
    inicializa_processos(processos, &nProcessos); // Inicializa a estrutura de processos
    
    token = strtok(NULL, " "); // Lê o próximo token
    if(token == NULL){ // Se não houver nenhum token, continua o loop
        printf("[ #<- ] ERRO: Nenhum PID fornecido.\n");
        return;
    }

    int pid = atoi(token); // Converte o token para inteiro

    if(pid <= 0){ // Verifica se o PID é válido
        printf("[ #<- ] ERRO: PID inválido.\n");
        return;
    }

    exibir_processos(pid, 1, processos, nProcessos);    // Exibe os processos em árvore
}

/**
    * @brief Executa um comando no terminal.
    *
    * Esta função executa um comando no terminal, criando um processo filho caso desejado
    * para executar o comando e aguardando sua conclusão.
    *
    * @param arguments Array de argumentos do comando a ser executado.
    * @param token Token que contém o comando a ser executado.
    * @return void
*/
void terminal(char *argumentos[7], char *token){
    int i = 0;  // Inicializa o índice para os argumentos
    int ehFork = 0; // Variável para verificar se o comando deve ser executado em segundo plano

    argumentos[i] = token;   // Armazena o primeiro token (comando) no vetor de argumentos
    i++;    // Incrementa o índice
    token = strtok(NULL, " ");  // Divide a linha em tokens usando espaço como delimitador

    while(token != NULL){   // Continua a dividir a linha em tokens
        if(i == 6){ // Verifica se o número máximo de argumentos foi atingido
            printf("[ #<- ] ERRO: Número máximo de argumentos excedido.\n");
            break;
        }

        if (token[0] == '&') {  // Verifica se o fork será criado
            ehFork = 1;
            break;
        }
        
        argumentos[i] = token; // Armazena o token no vetor de argumentos
        i++;
        token = strtok(NULL, " "); // Continua a dividir a linha em tokens
    }
    argumentos[i] = NULL; // Adiciona NULL ao final do vetor de argumentos, necessário para o execvp
    

    pid_t pid = fork(); // Cria um novo processo
    if(pid == -1){  // Verifica se o fork falhou
        perror("[ -># ] ERRO: Não foi possível criar o processo filho");
        return;
    }
    
    if(pid == 0){ // O que o processo filho deve fazer
        execvp(argumentos[0], argumentos); // Executa o comando com os argumentos fornecidos, processo filho morre mas o pai continua rodando
        perror("[ -># ] ERRO: Não foi possível executar o comando");
        exit(EXIT_FAILURE); // Se o execvp falhar, o processo filho termina com erro 
    } else {
        if(!ehFork){  // O que o processo pai deve fazer se não for um fork
            waitpid(pid, NULL, 0);  // Espera o processo filho terminar
        }
    }

}

/**
    * @brief Função principal do programa.
    *
    * Esta função lê os comandos do usuário, executa os comandos apropriados
    * e exibe os processos em formato de árvore quando solicitado.
    *
    * @return 0 se o programa for executado com sucesso.
*/
int main(){
    char input[200]; // Buffer para guardar a linha de comando
    char *token; // Ponteiro para guardar os tokens 
    char *argumentos[7]; // Ponteiro para guardar os argumentos

    system("clear");
    printf("-------------------------------------------------------------------------------------\n");
    printf("[ ! ] Informações de Uso:\n");
    printf("  - Digite 'exit' para sair do programa.\n");
    printf("  - Digite 'tree PID' para exibir os processos em formato de árvore do PID desejado.\n");
    printf("  - Para executar um comando em segundo plano, adicione '&' como argumento final.\n");
    printf("  - O terminal suporta 5 argumentos, qualquer coisa além disso será ignorado.\n\n");

    printf("[ ! ] Informações de Execução:\n");
    printf("  - [ #<- ] : Erro de entrada do usuário.\n");
    printf("  - [ -># ] : Erro interno de execução.\n");
    printf("  - [ <- ]  : Entrada do usuário.\n");
    printf("  - [ -> ]  : Saída para o usuário.\n");
    printf("-------------------------------------------------------------------------------------\n");


    while(1){
        printf("[ <- ] ");
        fgets(input, sizeof(input), stdin); // Lê a linha de comando do usuário
        input[strcspn(input, "\n")] = '\0'; // Remove o caractere de nova linha

        token = strtok(input, " "); // Divide a linha em tokens usando espaço como delimitador
        if(token == NULL){  // Se não houver nenhum token, continua o loop
            printf("[ #<- ] ERRO: Nenhum comando fornecido.\n");
            continue;
        }

        if(strcmp(token, "tree") == 0){
            execucao_tree(token);   // Executa a função para exibir os processos em árvore
            printf("-------------------------------------------------------------------------------------\n");
        } else if (strcmp(token, "exit") == 0){
            printf("[ -> ] Saindo do programa...\n");
            break;
        } else {
            terminal(argumentos, token); // Executa o comando fornecido
        }
    }
    return 0;
}