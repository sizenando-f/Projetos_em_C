#include <stdio.h>

// Opção 1
void bubbleSortOriginal(int vetor[], unsigned n){
  for(unsigned i = 0; i < n; i++){
    for(unsigned j = 0; j < n-1; j++){
      if(vetor[j] > vetor[j+1]){
        int aux = vetor[j+1];
        vetor[j+1] = vetor[j];
        vetor[j] = aux;
      }
    }
  }
}

// Opção 2
void bubbleSortMelhorado(int vetor[], unsigned n){
  int mudou = 1;
  unsigned nLinha = n, guarda = n;

  while(mudou){
    unsigned j = 0;
    mudou = 0;
    while(j < nLinha){
      if(vetor[j] > vetor[j+1]){
        int aux = vetor[j+1];
        vetor[j+1] = vetor[j];
        vetor[j] = aux;
        mudou = 1;
        guarda = j;
      }
      j++;
    }
    nLinha = guarda;
  }
}

int main(){
  int vetor[] = {1,5,6,2,4,8,3,4,98,0};

  // bubbleSortOriginal(vetor, 10);
  bubbleSortMelhorado(vetor, 10);

  for(int i = 0; i < 10; i++){
    printf("%d\n", vetor[i]);
  }
  return 0;
}