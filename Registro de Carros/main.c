#include <stdio.h>

void conta_registros(){
  FILE * fp = fopen("carro.dbf", "rb");
  
}

int main(){
  int esc;
  do{
    printf("---------------\n");
    printf("1. INFORMAR QUANTIDADE DE REGISTROS EM \"carro.dbf\"\n");
    printf("2. ORDERNAR REGISTROS DE \"carro.dbf\" EM ORDEM CRESCENTE E ARMAZENA-LOS EM \"carro.ord\"\n");
    printf("3. MOSTRAR REGISTROS DE \"carro.ord\"\n");
    printf("4. INFORMAR A QUANTIDADE DE CARRO PARA CADA TIPO DE COMBUSTIVEL\n");
    printf("5. INFORMAR A QUANTIDADE DE CARRO PARA CADA TIPO DE OPCIONAL\n");
    printf("6. SAIR\n");
    printf("ESCOLHA -> ");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        conta_registros();
        system("pause");
        break;
      default:
        break;
    }
  }
  return 0;
}