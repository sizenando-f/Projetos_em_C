#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Limpa a tela de acordo com o sistema operacional
void limpa_tela(){
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// Verifica a existência de um estado no conjunto de estados
int verificaEstado(char estados[20], char estado, int numEstados){
  for(int i = 0; i < numEstados; i++){
    if(estados[i] == estado){
      return 1;
    }
  }
  return 0;
}

// Verifica se todas as letras da palavra estão no alfabeto pré-determinado
int verificaPalavra(char palavra[100], char alfabeto[2], int tamPalavra){
  for(int i = 0; i < tamPalavra; i++){
    int check = 0;
    for(int j = 0; j < 2; j++){
      if(palavra[i] == alfabeto[j]){
        check = 1;
      }
    }
    if(!check){
      return 0;
    }
  }
  return 1;
}

// Recebe o alfabeto do usuário
void getAlfabeto(char alfabeto[2]){
  printf("[ <- ] Insira a primeira letra do alfabeto > ");
  scanf(" %c", &alfabeto[0]);
  printf("[ <- ] Insira a segunda letra do alfabeto > ");
  scanf(" %c", &alfabeto[1]);
  alfabeto[2] = '\0'; 
  getchar();
  printf("-----------------------------------------------------------------------\n");
}

// Retorna o índice de um estado
int encontraEstado(char estados[20], char estadoAtual, int numEstados){
  for(int i = 0; i < numEstados; i++){
    if(estados[i] == estadoAtual){
      return i;
    }
  }
  return -1;
}

// Retorna o índice de uma letra do alfabeto
int encontraLetra(char alfabeto[2], char letra){
  for(int i = 0; i < 2; i++){
    if(alfabeto[i] == letra){
      return i;
    }
  }
  return -1;
}

// Verifica se os estados de aceitação são válidos
int verificaEstados(char estados[20], char palavra[20], int numEstados, int numEstadosAceitacao){
  for(int i = 0; i < numEstadosAceitacao; i++){
    int check = 0;
    for(int j = 0; j < numEstados; j++){
      if(palavra[i] == estados[j]){
        check = 1;
      }
    }
    if(!check){
      return 0;
    }
  }
  return 1;
}

// Recebe os estados da máquina
void getEstados(char estados[20], int *numEstados){
  char temp[30] = "";
  printf("[ <- ] Insira os estados da maquina (separe por virgulas) > ");
  fgets(temp, sizeof(temp), stdin);

  char *pont = strtok(temp, ",");
  int indice = 0;
  while(pont != NULL){
    estados[indice] = pont[0]; 
    pont = strtok(NULL, ",");
    indice++;
    *numEstados = *numEstados + 1;
  }
  printf("-----------------------------------------------------------------------\n");
}

// Recebe o estado inicial da máquina
void getEstadoInicial(char estados[20], char *estadoInicial, int numEstados){
  int check = 1;
  while(check){
    printf("[ <- ] Insira o estado inicial da maquina > ");
    scanf(" %c", estadoInicial);
    getchar(); 
    if(verificaEstado(estados, *estadoInicial, numEstados)){
      check = 0;
    } else {
      printf("\n[ -># ERRO ] Estado nao existe na maquina, tente novamente\n");
    }
  }
  printf("-----------------------------------------------------------------------\n");
}

// Recebe os estados de aceitação da máquina
void getEstadosDeAceitacao(char estados[20], char estadosDeAceitacao[20], int numEstados){
  char temp[30] = "";
  int check = 1;
  while(check){
    int numEstadosAceitacao = 0;
    printf("[ <- ] Insira os estados de aceitacao da maquina (separe por virgulas) > ");
    fgets(temp, sizeof(temp), stdin);
    char *pont = strtok(temp, ",");
    int indice = 0;
    while(pont != NULL){
      estadosDeAceitacao[indice] = pont[0]; 
      pont = strtok(NULL, ",");
      indice++;
      numEstadosAceitacao++;
    }
    if(verificaEstados(estados, estadosDeAceitacao, numEstados, numEstadosAceitacao)){
      check = 0;
    } else {
      printf("[ -># ERRO ] Algum estado inserido nao pertence a maquina, tente novamente\n");
    }
  }
  printf("-----------------------------------------------------------------------\n");
}

// Recebe a função de transição da máquina
void getFuncao(char funcao[20][3][20], char estados[20], int numEstados, char alfabeto[2]){
  char temp[20];
  for(int i = 0; i < numEstados; i++){
    for(int j = 0; j < 3; j++){
      if(j < 2){
        printf("[ <- ] Delta(%c, %c) > ", estados[i], alfabeto[j]);
      } else {
        printf("[ <- ] Delta(%c, e) > ", estados[i]);
      }
      fgets(temp, sizeof(temp), stdin);
      temp[strcspn(temp, "\n")] = '\0';
      if(strcmp(temp, "-") == 0){
        strcpy(funcao[i][j], "");
      } else {
        strcpy(funcao[i][j], temp);
      }
    }
  }
  printf("-----------------------------------------------------------------------\n");
}

// Executa a máquina de estados finitos não-determinística (AFN)
int executaAFN(char estadoAtual, char estados[20], char alfabeto[], char estadosDeAceitacao[20], char funcao[20][3][20], char palavra[100], int tamPalavra, int posicao){
  // Verifica se o estado atual é um estado de aceitação
  for(size_t i = 0; i < strlen(estadosDeAceitacao); i++){
    if(estadoAtual == estadosDeAceitacao[i]){
      if(posicao == tamPalavra) return 1; // Aceita a palavra se a posição atual for o final da palavra
    }
  }

  if(posicao < tamPalavra){
    // Transições normais (baseadas no alfabeto)
    int indiceEstado = encontraEstado(estados, estadoAtual, strlen(estados));
    int indiceLetra = encontraLetra(alfabeto, palavra[posicao]);
    if(strlen(funcao[indiceEstado][indiceLetra]) > 0){
      for(size_t i = 0; i < strlen(funcao[indiceEstado][indiceLetra]); i++){
        if(executaAFN(funcao[indiceEstado][indiceLetra][i], estados, alfabeto, estadosDeAceitacao, funcao, palavra, tamPalavra, posicao + 1)){
          return 1;
        }
      }
    }
  }

  // Transições epsilon
  int indiceEstado = encontraEstado(estados, estadoAtual, strlen(estados));
  if(strlen(funcao[indiceEstado][2]) > 0){
    for(size_t i = 0; i < strlen(funcao[indiceEstado][2]); i++){
      if(executaAFN(funcao[indiceEstado][2][i], estados, alfabeto, estadosDeAceitacao, funcao, palavra, tamPalavra, posicao)){
        return 1;
      }
    }
  }

  return 0;
}

// Usuário escolhe ou não continuar com os dados entrados
void exibeVisaoGeral(char estadoInicial, char estados[20], char alfabeto[], char estadosDeAceitacao[20], char funcao[20][3][20], int numEstados){
  printf("            [# VISAO GERAL #]\n");
  printf("[ @ ] Estados da maquina   -> ");
  for(int i = 0; i < numEstados; i++){
    printf("[%c] ", estados[i]);
  }
  printf("\n[ @ ] Alfabeto             -> [%c] [%c]", alfabeto[0], alfabeto[1]);
  printf("\n[ @ ] Estado inicial       -> [%c]", estadoInicial);
  printf("\n[ @ ] Estados de aceitacao -> ");
  for(size_t i = 0; i < strlen(estadosDeAceitacao); i++){
    printf("[%c] ", estadosDeAceitacao[i]);
  }
  printf("\n\n  { FUNCAO }\n");
  printf("--------------\n");
  printf("|   | %c    %c    e |\n", alfabeto[0], alfabeto[1]);
  printf("--------------\n");
  for(int i = 0; i < numEstados; i++){
    printf("| %c | %s    %s    %s |\n", estados[i], funcao[i][0], funcao[i][1], funcao[i][2]);
    printf("--------------\n");
  }
  printf("\n");
}

int main() {
  char estados[20] = "";                 // Conjunto de estados
  char alfabeto[3] = "";                 // Alfabeto
  char estadosDeAceitacao[20] = "";      // Estados de aceitação
  char funcao[20][3][20];                // Função de transição
  char estadoInicial;                    // Estado inicial
  char palavra[100];                     // Palavra de entrada
  int numEstados = 0;                    // Número de estados

  // Recebe o alfabeto
  getAlfabeto(alfabeto);

  // Recebe os estados da máquina
  getEstados(estados, &numEstados);

  // Recebe o estado inicial
  getEstadoInicial(estados, &estadoInicial, numEstados);

  // Recebe os estados de aceitação
  getEstadosDeAceitacao(estados, estadosDeAceitacao, numEstados);

  // Recebe a função de transição
  getFuncao(funcao, estados, numEstados, alfabeto);

  // Exibe uma visão geral da máquina
  exibeVisaoGeral(estadoInicial, estados, alfabeto, estadosDeAceitacao, funcao, numEstados);

  while (1) {
    printf("[ <- ] Insira a palavra para ser testada (ou \"sair\" para sair) > ");
    fgets(palavra, sizeof(palavra), stdin);
    palavra[strcspn(palavra, "\n")] = '\0'; // Remove newline character

    // Verifica se o usuário deseja sair
    if (strcmp(palavra, "sair") == 0) {
      break;
    }

    // Verifica se a palavra é válida
    if (verificaPalavra(palavra, alfabeto, strlen(palavra))) {
      if (executaAFN(estadoInicial, estados, alfabeto, estadosDeAceitacao, funcao, palavra, strlen(palavra), 0)) {
        printf("[# RESULTADO #] A palavra foi aceita pela maquina!\n");
      } else {
        printf("[# RESULTADO #] A palavra foi rejeitada pela maquina.\n");
      }
    } else {
      printf("[ -># ERRO ] A palavra inserida contem simbolos que nao pertencem ao alfabeto.\n");
    }

    printf("-----------------------------------------------------------------------\n");
  }

  printf("Programa encerrado.\n");
  return 0;
}
