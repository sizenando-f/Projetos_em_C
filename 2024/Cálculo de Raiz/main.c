#include <stdio.h>
#include <time.h>

void execute(double p, double m){
  double X, Xant = 1;
  unsigned cont = 0;
  do{
    printf("[ -> ] X%d: %.16lf\n", cont++, X);
    X = (1/m)*(Xant + (p/Xant));
    if(X == Xant) break;
    Xant = X;
  }while(1);
}

void start(double *p, double *m){
  printf("[ <- ] Insira o grau da raiz (m) > ");
  scanf("%lf", m);
  getchar();
  printf("[ <- ] Insira valor interno (p) > ");
  scanf("%lf", p);
}

int main(){
  double p, m, tempo_gasto;
  struct timespec inicio, fim;
  start(&p, &m);

  clock_gettime(CLOCK_MONOTONIC, &inicio);
  execute(p, m);
  clock_gettime(CLOCK_MONOTONIC, &fim);
  tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec)/1E9;
  printf("[ info ] Tempo de execucao: %lfs\n", tempo_gasto);

  return 0;
}