#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

/**
 * @brief Itera sobre todos valores dentro do intervalo
 * 
 * @param n Numero de constantes
 * @param a Primeiro valor do intervalo
 * @param b Segundo valor do intervalo
 * @param constantes Constantes da função polinomial
 */
void execute(unsigned n, int a, int b, double constantes[]){
  double resultAnt = 0, result;
  int cont = 0, check = 0;

  printf("\n[ -> ] Analisando...\n");
  for(int i = a; i <= b; i++){
    result = 0;
    for(unsigned j = 0; j < n; j++){
      result += constantes[j]*pow(i, j);  // f(x) = Cn*x^n
    }
    if(cont++){
      if(resultAnt*result < 0){ // f(a).f(b) < 0
        check = 1;
        printf("[ ! ] Raiz no intervalo: (%d, %d)\n", i-1, i);
        printf("[ -> ] f(%d) = %.7lf | f(%d) = %.7lf\n\n", i-1, resultAnt, i, result);
      }
    }
    resultAnt = result;
  }

  if(!check){
    printf("[ -> ] Nenhuma raiz foi encontrado\n");
  }
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
void start(unsigned *n, int *a, int *b, double **constantes){
  printf("[ <- ] Entre com o numero de constantes (n) > ");
  scanf("%u", n);
  getchar();

  *constantes = (double*) malloc((*n) * sizeof(double));
  for(unsigned i = 0; i < (*n); i++){
    printf("[ <- ] Entre com a constante C%d > ", i);
    scanf("%lf", &(*constantes)[i]);
  }
  getchar();

  printf("[ <- ] Entre com o intervalo no formato '[a,b]' > ");
  scanf("[%d,%d]", a, b);
}

int main(){
  unsigned n;
  int a, b;
  double *constantes, tempo_gasto;
  struct timespec inicio, fim;

  start(&n, &a, &b, &constantes);

  clock_gettime(CLOCK_MONOTONIC, &inicio);
  execute(n, a, b, constantes);
  clock_gettime(CLOCK_MONOTONIC, &fim);
  tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec)/1E9;
  printf("[ info ] Tempo de execucao: %lfs\n", tempo_gasto);
  free(constantes);
  system("pause");
  return 0;
}