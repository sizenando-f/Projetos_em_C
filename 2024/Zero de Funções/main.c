#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void execute(unsigned n, float a, float b, float constantes[]){
  
}

/**
 * @brief Inicia variáveis
 * 
 * @param n Número de constantes
 * @param a Primerio valor do intervalo inicial
 * @param b Segundo valor do intervalo inicial
 * @param constantes Constantes da função 
 * @param erro Condição de parada
 */
void start(unsigned *n, float *a, float *b, float **constantes){
  printf("[ <- ] Entre com o numero de constantes (n) > ");
  scanf("%u", n);
  getchar();

  *constantes = (float*) malloc((*n) * sizeof(float));
  for(unsigned i = 0; i < (*n); i++){
    printf("[ <- ] Entre com a constante C%d > ", i);
    scanf("%f", &(*constantes)[i]);
  }
  getchar();

  printf("[ <- ] Entre com o intervalo no formato '[a,b]' > ");
  scanf("[%f,%f]", a, b);
}

int main(){
  unsigned n;
  float a, b, *constantes;

  start(&n, &a, &b, &constantes);

  free(constantes);
  return 0;
}