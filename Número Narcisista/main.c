#include <stdio.h>
#include <stdlib.h>

int numero_narcisista(int num, int algarismos){

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
  int fim, indice = 0;
  printf("Insira um valor: ");
  scanf("%d", &fim);
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