#include <stdio.h>

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
  double p, m;
  start(&p, &m);
  execute(p, m);
  return 0;
}