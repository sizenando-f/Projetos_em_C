#include <stdio.h>
#include <string.h>


void pegaEstado(char estados[]){
  char temp[30];
  printf("Insira o estado da máquina: ");
  fgets(temp, sizeof(temp), stdin);

  char *pont = strtok(temp, ",");
  int indice = 0;
  while(pont != NULL){
    strcat(estados, pont);
    pont = strtok(NULL, ",");
    indice++;
  }
}

int main(){
  char estados[20];
  pegaEstado(estados);


  return 0;
}