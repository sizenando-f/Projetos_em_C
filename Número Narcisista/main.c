#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h> // unistd.h - sleep() ou a usleep()

int numero_narcisista(int num, int algarismos){
  int soma = 0;

  for(int i = 0; i < algarismos; i++){
    soma += pow((num%10), algarismos);
    num /= 10;
  }

  return soma;
}

int descobre_algarismos(int num){
  int temp = num;
  int cont = 0;
  while(temp != 0){
    if(temp != 0){
      cont++;
    }
    temp /= 10;
  }
  return cont;
}

int main(){
  int fim, indice = 0, esc;
  do{
    printf("-----# NUM3R0 N4RC1S1ST4 #-----\n");
    printf(" 1. ARMAZENAR NUMEROS\n");
    printf(" 2. EXCLUIR NUMERO\n");
    printf(" 3. MOSTRAR NUMEROS NARCISISTAS\n");
    printf(" 4. SAIR\n");
    printf("-------------------------------\n");
    printf("ESCOLHA ~# ");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        printf("INSIRA UM VALOR LIMITE: ");
        scanf("%d", &fim);
      default:
        break;
    }
  }while(esc != 4);

  
  int * numeros = malloc(fim * sizeof(int));

  for(int i = 10; i <= fim; i++){
    int potencia = descobre_algarismos(i);
    int num = numero_narcisista(i, potencia);
    if(num == i) numeros[indice++] = i;
  }

  for(int i = 0; i < indice; i++){
    printf("%d ", numeros[i]);
  }

  free(numeros);

  return 0;
}