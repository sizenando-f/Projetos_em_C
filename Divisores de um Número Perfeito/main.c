// Faça um procedimento que determine os N primeiros números perfeitos. Sabe-se que um número é 
// perfeito quando é igual a soma de seus divisores (exceto ele mesmo). Por exemplo, os divisores de 
// 6 são 1, 2 e 3 e 1+2+3=6, logo 6 é perfeito. O valor de N será passado para o procedimento e os 
// números perfeitos encontrados devem ser armazenados em um vetor global e informados na rotina 
// principal

#include <stdio.h>
#include <stdlib.h>

int nPerfeitos(int n, int vetor[100]){
  int soma = 0, cont = 0;
  for(int i = 1; i < n; i++){
    if(n%i == 0){
      soma += i;
      vetor[cont] = i;
      cont++;
    }
  }

  if(soma == n){
    return cont;
  } else {
    return 0;
  }
}

int main(){
  int n;
  int vetor[100];

  printf("Insira um valor: ");
  scanf("%d", &n);
  
  int cont = nPerfeitos(n, vetor);

  if(cont){
    printf("Eh um numero perfeito, seus divisiveis sao: \n");
    for(int i = 0; i < cont; i++){
      printf("%d\n", vetor[i]);
    }
  } else {
    printf("Nao eh um numero perfeito");
  }
  return 0;
}