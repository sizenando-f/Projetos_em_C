// Projeto: Máquina de Estados
// Descrição: Simula o funcionamento de uma máquina de estados
// Versão: 1.2.1

// Autor: Sizenando S. França
// Data de criação: 19/05/24
// Última modificação: 21/05/24

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

// Verifica existênicia de um estado no conjunto de estados
int verificaEstado(char estados[20], char estado, int numEstados){
  for(int i = 0; i < numEstados; i++){
    if(estados[i] == estado){
      return 1;
    }
  }
  return 0;
}

// Verifica se todas letras da palavra estão no alfabeto pré-determinado
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

// Recebe o alfabeto do usuario
void getAlfabeto(char alfabeto[2]){
  printf("[ <- ] Insira a primeira letra do alfabeto > ");
  scanf(" %c", &alfabeto[0]);
  printf("[ <- ] Insira a segunda letra do alfabeto > ");
  scanf(" %c", &alfabeto[1]);
  alfabeto[2] = '\0';
  getchar();
  printf("-----------------------------------------------------------------------\n");
}

// Retorna indice de um estado
int encontraEstado(char estados[20], char estadoAtual, int numEstados){
  for(int i = 0; i < numEstados; i++){
    if(estados[i] == estadoAtual){
      return i;
    }
  }
  return -1;
}

// Retorna indice de uma letra do alfabeto
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

// Recebe os estados da maquina
void getEstados(char estados[20], int *numEstados){
  char temp[30] = "";
  printf("[ <- ] Insira os estados da maquina (separe por virgulas) > ");
  fgets(temp, sizeof(temp), stdin);

  char *pont = strtok(temp, ",");
  int indice = 0;
  while(pont != NULL){
    estados[indice] = *pont;
    pont = strtok(NULL, ",");
    indice++;
    *numEstados = *numEstados + 1;
  }
  printf("-----------------------------------------------------------------------\n");
}

// Recebe o estado inicial da maquina
void getEstadoInicial(char estados[20], char *estadoInicial, int numEstados){
  int check = 1;
  while(check){
    printf("[ <- ] Insira o estado inicial da maquina > ");
    fgets(estadoInicial, sizeof(estadoInicial), stdin);
    if(verificaEstado(estados, *estadoInicial, numEstados)){
      check = 0;
    } else {
      printf("\n[ -># ERRO ] Estado nao existe na maquina, tente novamente\n");
    }
  }
  printf("-----------------------------------------------------------------------\n");
}

// Recebe os estados de aceitacao da maquina
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
      estadosDeAceitacao[indice] = *pont;
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

// Recebe a função de funcionamento da máquina
void getFuncao(char funcao[20][2], char estados[20], int numEstados, char alfabeto[2]){
  char temp;
  for(int i = 0; i < numEstados; i++){
    for(int j = 0; j < 2; j++){
      printf("[ <- ] Delta(%c, %c) > ", estados[i], alfabeto[j]);
      scanf(" %c", &temp);
      if(!verificaEstado(estados, temp, numEstados) && temp != '-'){
        printf("[ -># ERRO ] Utilize '-' para representar estado parcial\n");
        j--;
      } else {
        funcao[i][j] = temp;
      }
    }
  }
  getchar();
  printf("-----------------------------------------------------------------------\n");
}

// Recebe palavra para ser executada na máquina
void getPalavra(char palavra[100], char alfabeto[2]){
  char temp[100] = "";
  int check = 1;
  while(check){
    printf("[ <- ] Insira a palavra > ");
    fgets(temp, sizeof(temp), stdin);
    if(!verificaPalavra(temp, alfabeto, strlen(temp)-1)){
      printf("[ -># ERRO ] Alguma letra inserida nao pertence a maquina, tente novamente\n");
    } else {
      strcpy(palavra, temp);
      check = 0;
    }
  }
  
  printf("-----------------------------------------------------------------------\n");
}

// Executa a máquina por completo
int executaMaquina(char estadoInicial, char estados[20], char alfabeto[], char estadosDeAceitacao[20], char funcao[20][2], char palavra[100]){
  int tamPalavra = strlen(palavra);
  limpa_tela();
  printf("Palavra %s", palavra);
  printf("< Estados de aceitacao ");
  for(size_t i = 0; i < strlen(estadosDeAceitacao); i++){
    printf("[%c] ", estadosDeAceitacao[i]);
  }
  printf("> \n");
  for(int i = 0; i < tamPalavra; i++){
    printf(" -> [%c]", estadoInicial);
    int indiceEstado = encontraEstado(estados, estadoInicial, strlen(estados));
    if(indiceEstado == -1){
      printf("\n[ -># ERRO ] A maquina travou\n");
      return 0;
    }
    int indiceLetra = encontraLetra(alfabeto, palavra[i]);
    estadoInicial = funcao[indiceEstado][indiceLetra];
  }
  printf("\n-----------------------------------------------------------------------\n");

  for(size_t i = 0; i < strlen(estadosDeAceitacao); i++){
    if(estadoInicial == estadosDeAceitacao[i]){
      return 1;
    }
  }
  return 0;
}

// Usuário escolhe ou não continuar com os dados entrados
void exibeVisaoGeral(char estadoInicial, char estados[20], char alfabeto[], char estadosDeAceitacao[20], char funcao[20][2], int numEstados){
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
  printf("|   | %c    %c |\n", alfabeto[0], alfabeto[1]);
  printf("--------------\n");
  for(int i = 0; i < numEstados; i++){
    printf("| %c ", estados[i]);
    for(int j = 0; j < 2; j++){
      printf("| %c |", funcao[i][j]);
    }
    printf("\n");
    printf("--------------\n");
  }
}

// Realiza todos os passos de montagem da máquina
void inicializa(char estados[20], char alfabeto[2], char estadosDeAceitacao[20], char *estadoInicial, char funcao[20][2], int *numEstados){
  int numEstadosTemp = 0;
  char estadoInicialTemp;
  getEstados(estados, &numEstadosTemp);
  *numEstados = numEstadosTemp;
  getAlfabeto(alfabeto);
  getEstadosDeAceitacao(estados, estadosDeAceitacao, *numEstados);
  getEstadoInicial(estados, &estadoInicialTemp, *numEstados);
  *estadoInicial = estadoInicialTemp;
  getFuncao(funcao, estados, *numEstados, alfabeto);
}

int main(){
  char estadoInicial, alfabeto[2] = "", estados[20] = "", estadosDeAceitacao[20] = "", funcao[20][2], palavra[100] = "", esc;
  int check = 1;

  // Processo de inserção de dados na máquina e verificação
  do{
    int numEstados = 0;
    limpa_tela();
    printf("            ==== MAQUINA DE ESTADOS ====\n");
    inicializa(estados, alfabeto, estadosDeAceitacao, &estadoInicial, funcao, &numEstados);
    limpa_tela();
    exibeVisaoGeral(estadoInicial, estados, alfabeto, estadosDeAceitacao, funcao, numEstados);
    printf("[ <- ] Deseja continuar? [S/n] > ");
    scanf(" %c", &esc);
    getchar();
    if(esc == 'S' || esc == 's'){
      check = 0;
    }
  } while(check);

  check = 1;

  // Processo de inserção de palavras
  do{
    limpa_tela();
    getPalavra(palavra, alfabeto);
    if(executaMaquina(estadoInicial, estados, alfabeto, estadosDeAceitacao, funcao, palavra)){
      printf("\n[ -> ] A maquina terminou em um estado aceitacao, a palavra foi aceita");
    } else {
      printf("\n[ -> ] A maquina terminou em um estado incorreto, a palavra foi recusada");
    }
    printf("\n[ <- ] Deseja tentar outra palavra? [S/n] > ");
    scanf(" %c", &esc);
    getchar();
    if(esc == 'N' || esc == 'n'){
      check = 0;
    }
  }while(check);

  printf("[ -> ] PROGRAMA ENCERRADO");
  return 0;
}