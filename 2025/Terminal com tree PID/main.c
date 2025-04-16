#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

struct Process {    // Estrutura para guardar os dados do processo
    int pid;
    int ppid;
    char estado;
    char nome[256];
};

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

void inicializa_processos(struct Process *processos, int *nProcessos){ // Inicializa a estrutura de processos
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

void exibir_processos(int ppid, int nivel, struct Process *processos, int nProcessos){
    for(int i = 0; i < nProcessos; i++){
        if(ppid == processos[i].pid){
            printf("%*s", nivel * 4, ""); // Indenta a saída
            printf("--- PID: %d nome: %s estado:%c\n", processos[i].pid, processos[i].nome, processos[i].estado); // Exibe os dados do processo
            break;
        }
    }
    
    for(int i = 0; i < nProcessos; i++){
        if(ppid == processos[i].ppid){
            exibir_processos(processos[i].pid, nivel + 1, processos, nProcessos); // Chama a função recursivamente para exibir os processos filhos
        }
    }
    
}

int main(){
    char input[200]; // Buffer para guardar a linha de comando
    char *token; // Ponteiro para guardar os tokens 
    char *arguments[7]; // Ponteiro para guardar os argumentos

    while(1){
        printf("> ");
        fgets(input, sizeof(input), stdin); // Lê a linha de comando do usuário
        input[strcspn(input, "\n")] = '\0'; // Remove o caractere de nova linha

        token = strtok(input, " "); // Divide a linha em tokens usando espaço como delimitador
        if(token == NULL){  // Se não houver nenhum token, continua o loop
            printf("Erro: Nenhum comando fornecido.\n");
            continue;
        }

        if(strcmp(token, "tree") == 0){ // Comando para listar os processos em árvore
            int nProcessos = 0; // Número de processos lidos
            struct Process processos[5000]; // Array de processos
            inicializa_processos(processos, &nProcessos); // Inicializa a estrutura de processos
            
            token = strtok(NULL, " "); // Lê o próximo token
            if(token == NULL){ // Se não houver nenhum token, continua o loop
                printf("Erro: Nenhum PID fornecido.\n");
                continue;
            }

            int pid = atoi(token); // Converte o token para inteiro

            if(pid <= 0){ // Verifica se o PID é válido
                printf("Erro: PID inválido.\n");
                continue;
            }

            exibir_processos(pid, 1, processos, nProcessos);

            
        } else if (strcmp(token, "exit") == 0){
            break;
        } else {
            int i = 0;
            int isFork = 0;

            arguments[i] = token;
            i++;
            token = strtok(NULL, " ");
            while(token != NULL){
                if(i == 6){
                    printf("Erro: Número máximo de argumentos excedido.\n");
                    break;
                }

                if (token[0] == '&') {
                    isFork = 1;
                    break;
                }
                
                arguments[i] = token; // Armazena o token no vetor de argumentos
                i++;
                token = strtok(NULL, " "); // Continua a dividir a linha em tokens
            }
            arguments[i] = NULL; // Adiciona NULL ao final do vetor de argumentos
            

            pid_t pid = fork();
            if(pid == -1){
                perror("Erro ao criar o processo filho");
                continue;
            }
            
            if(pid == 0){ // O que o processo filho deve fazer
                execvp(arguments[0], arguments); // Executa o comando com os argumentos fornecidos, processo filho morre mas o pai continua rodando
                perror("Erro ao executar o comando");
                exit(EXIT_FAILURE); // Se o execvp falhar, o processo filho termina com erro 
            } else {
                if(!isFork){  // O que o processo pai deve fazer
                    waitpid(pid, NULL, 0);
                }
            }

        }

        
    }
    return 0;
}