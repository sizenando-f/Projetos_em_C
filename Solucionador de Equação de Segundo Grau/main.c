
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

int calculaDelta(int a, int b, int c){
  printf("CALCULANDO DELTA...\n");
  printf("| %c = b%c - 4.a.c \n", 30, 253);
  printf("---------------------------\n");
  sleep(1);
  printf("  %c = %d%c - 4.%d.%d  \n", 30, b, 253, a, c);
  sleep(1);
  printf("  %c = %.0f - %d  \n", 30, pow(b, 2), 4*a*c);
  int delta = pow(b,2) - (4*a*c);
  sleep(1);
  printf("  %c = %d\n", 30, delta);

  printf("---------------------------\n");
  printf("DELTA DESCOBERTO!\n");
  if(delta > 0){
    printf(" - VAI EXISTIR DUAS RAIZES REAIS DISTINTAS, \n - POIS O DELTA %c MAIOR QUE 0\n", 144);
  } else if(delta == 0){
    printf(" - VAI EXISTIR DUAS RAIZES REAIS E IGUAIS, \n - POIS O DELTA %c IGUAL A 0\n", 144);
  } else {
    printf(" - VAI EXISTIR DUAS RAIZES COMPLEXAS CONJUGADAS, \n - POIS O DELTA %c MENOR QUE 0\n", 144);
  }
  printf("---------------------------\n\n");
  sleep(3);
  return delta;
}

void calcularQuadratica(int a, int b, int delta){
  printf("---------------------------\n");
  printf("UTILIZANDO F%cRMULA QUADRATICA...\n", 224);
  printf("| x = (-b %c raizquadrada(%c))/2.a\n", 241, 30);
  printf("---------------------------\n");
  sleep(1);
  printf(" x = (-(%d) %c raizquadrada(%d))/2.%d\n", b, 241, delta, a);
  b *= -1;
  sleep(1);
  printf(" x = (%d %c %.0f)/%d\n\n", b, 241, sqrt(delta), 2*a);
  delta = sqrt(delta);
  a *= 2;
  sleep(2);
  printf("--- x%c\n", 251);
  printf(" x%c = (%d + %d)/%d\n", 251, b, delta, a);
  sleep(1);
  printf(" x%c = %d/%d\n", 251, b + delta, a);
  sleep(1);
  int x1 = (b + delta) / a;
  printf(" x%c = %d\n\n", 251, x1);
  sleep(1);

  sleep(2);
  printf("--- x%c\n", 253);
  printf(" x%c = (%d - %d)/%d\n", 253, b, delta, a);
  sleep(1);
  printf(" x%c = %d/%d\n", 253, b - delta, a);
  sleep(1);
  int x2 = (b - delta) / a;
  printf(" x%c = %d\n", 253, x2);
  sleep(1);
  printf("---------------------------\n");
  printf("EQUA%cAO RESOLVIDA!\n", 128);
  printf("X%c = %d, X%c = %d\n", 251, x1, 253, x2);
  printf("---------------------------\n");

}

int main(){
  int a, b, c;
  char esc;

  do{
    system("cls");
    printf("|- INSIRA O VALOR DE A: ");
    scanf("%d", &a);
    printf("---------------------------\n");
    printf("|- INSIRA O VALOR DE B: ");
    scanf("%d", &b);
    printf("---------------------------\n");
    printf("|- INSIRA O VALOR DE C: ");
    scanf("%d", &c);
    system("cls");
    printf(" --- F%cRMULA CRIADA ---\n", 224);
    printf("| (%dx%c) + (%dx) + (%d) |\n", a, 253, b, c);
    printf("------------------------\n");
    do{
      printf("DESEJA CONTINUAR? (S/N): ");
      scanf(" %c", &esc);
      esc = toupper(esc);
    } while((esc != 'S') && (esc != 'N'));
  } while(esc == 'N');
  system("cls");

  int delta = calculaDelta(a, b, c);
  calcularQuadratica(a, b, delta);
  system("pause");
  return 0;
}