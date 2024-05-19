#include <stdio.h>
#include <string.h>

// Verifica existênicia de um estado apenas
int verificaEstado(char estados[20], char estado, int numEstados){
  for(int i = 0; i < numEstados; i++){
    if(estados[i] == estado){
      return 1;
    }
  }
  return 0;
}

// Verifica a existência de uma cadeia de estados
int verificaEstados(char estados[20], char palavra[20], int numEstados, int numEstadosAceitacao){
  printf("Numero de estados: %d | Numero de estados de aceitacao: %d\n", numEstados, numEstadosAceitacao);
  for(int i = 0; i < numEstadosAceitacao; i++){
    int check = 0;
    for(int j = 0; j < numEstados; j++){
      if(palavra[i] == estados[j]){
        check = 1;
      }
    }
    if(!check){
      printf("\nAlgum estado nao esta presente na maquina, tente novamente\n");
      return 0;
    }
  }
  return 1;
}

// Recebe os estados da maquina
void getEstado(char estados[20], int *numEstados){
  char temp[30] = "";
  printf("Insira o estado da maquina (separe por virgulas): ");
  fgets(temp, sizeof(temp), stdin);

  char *pont = strtok(temp, ",");
  int indice = 0;
  while(pont != NULL){
    estados[indice] = *pont;
    pont = strtok(NULL, ",");
    indice++;
    *numEstados = *numEstados + 1;
  }
}

// Recebe o estado inicial da maquina
void getEstadoInicial(char estados[20], char *estadoInicial, int numEstados){
  int check = 1;
  while(check){
    printf("Insira o estado inicial da maquina: ");
    fgets(estadoInicial, sizeof(estadoInicial), stdin);
    if(verificaEstado(estados, *estadoInicial, numEstados)){
      check = 0;
    } else {
      printf("\nEstado nao existe na maquina, tente novamente\n");
    }
  }
}

// Recebe os estados de aceitacao da maquina
void getEstadosDeAceitacao(char estados[20], char estadosDeAceitacao[20], int numEstados){
  char temp[30] = "";
  int check = 1;
  while(check){
    int numEstadosAceitacao = 0;
    printf("Insira os estados de aceitacao da maquina (separe por virgulas): ");
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
    }
  }
  
}

int main(){
  char estadoInicial, estados[20] = "", estadosDeAceitacao[20];
  int numEstados = 0;
  getEstado(estados, &numEstados);
  getEstadosDeAceitacao(estados, estadosDeAceitacao, numEstados);
  getEstadoInicial(estados, &estadoInicial, numEstados);

  printf("\n\n %s", estados);
  printf("\n A maquina possui %d estados: ", numEstados);
  return 0;
}