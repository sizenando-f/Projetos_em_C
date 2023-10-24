#include <stdio.h>

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
  int num = 193;
  int algarismos = descobre_algarismos(num);
  int fim;
  printf("Insira um valor: ");
  scanf("%d", &fim);
  return 0;
}