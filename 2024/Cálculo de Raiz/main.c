#include <stdio.h>

void execute(double p, double m){

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
  return 0;
}