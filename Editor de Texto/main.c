#include <stdio.h>
#include <string.h>

void pegaTexto(char frase[], char texto[][82]){
  char * palavra = strtok(frase, " ");
  strcpy(texto[0], palavra);
  strcat(texto[0], " ");

  palavra = strtok(NULL, " ");

  for (size_t i = 0; i < 10; i++){
    int check = 0;
    while(check == 0 && palavra != NULL){
      strcat(texto[i], palavra);
      
      if(strlen(texto[i]) > 80){
        check = 1;
        memmove(texto[i] + strlen(texto[i]) - strlen(palavra), " ", strlen(palavra));
        strcpy(texto[i + 1], palavra);
      }

      if(check){
        strcat(texto[i+1], " ");
      } else {
        strcat(texto[i], " ");
      }

      palavra = strtok(NULL, " ");
    }
  }
  }

int main(){
  char frase[500];
  char texto[10][82];

  printf("Insira o texto: \n");
  fgets(frase, 500, stdin);
  printf("\n");

  pegaTexto(frase, texto);
  
  for (size_t i = 0; i < strlen(frase); i++)
  {
    printf("%s\n", texto[i]);
  }

  return 0;
}