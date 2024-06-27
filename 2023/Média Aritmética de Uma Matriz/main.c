// Faça uma função que receba uma matriz 12x12 e retorne a média aritmética dos elementos abaixo da 
// diagonal principal

#include <stdio.h>
#include <stdlib.h>

int somaAritmetica(int matriz[12][12]){
  int soma = 0;

  for(int i = 0; i < 11; i++){
    soma += matriz[i+1][i];
  }

  return soma/11;

}

int main(){
  int matriz[12][12];
  for(int i = 0; i < 12; i++){
    for(int j = 0; j < 12; j++){
      printf("Posicao [%d][%d] \n", i, j);
      scanf("%d", &matriz[i][j]);
    }
  }

  for(int i = 0; i < 12; i++){
    for(int j = 0; j < 12; j++){
      printf("%3d", matriz[i][j]);
    }
    printf("\n");
  }

  printf("A media aritmetica eh: %d", somaAritmetica(matriz));
  return 0;
}