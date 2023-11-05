#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 50

char opcionais[][TAM]={
  {"4.portas"},
  {"cambio.automatico"},
  {"vidros.eletricos"},
  {"abs"},
  {"air.bag"},
  {"ar.condicionado"},
  {"banco.couro"},
  {"sensor.estacionamento"}
};

typedef struct {
  long int id_reg;
  char placa[9];
  char modelo[TAM];
  char fabricante[TAM];
  int ano_fabricacao;
  int ano_modelo;
  char combustivel[TAM];
  char cor[TAM];
  int opcional[8];
  float preco_compra;
} CARRO;

void conta_registros(){
  FILE * fp = fopen("carro.dbf", "rb");

  if(fp == NULL){
    printf("ERRO AO ABRIR O ARQUIVO PARA CONTAGEM DE REGISTROS\n");
    exit(100);
  }

  fseek(fp, 0, SEEK_SET);

  CARRO carro;
  int cont = 0;

  while(!feof(fp)){
    if(fread(&carro, sizeof(carro), 1, fp) > 0){
      cont++;
    }
  }

  fclose(fp);

  printf("------------------\n");
  printf("O ARQUIVO POSSUI %d REGISTROS\n", cont);
  printf("------------------\n");
}

int compara_placa(const void *a, const void*b){
  const char *placa1 = ((CARRO *)a)->placa;
  const char *placa2 = ((CARRO *)b)->placa;
  return strcmp(placa1, placa2);
}

void ordena_registros(){
  FILE * fp = fopen("carro.dbf", "rb");
  if(fp == NULL){
    printf("ERRO AO ABRIR ARQUIVO PARA ORDENACAO\n");
    exit(100);
  }

  fseek(fp, 0, SEEK_SET);

  CARRO carro;
  CARRO carros[2283];
  int n = 0;

  while(!feof(fp)){
    if(fread(&carro, sizeof(carro), 1, fp) > 0){
      carros[n++] = carro;
    }
  }

  fclose(fp);

  qsort(carros, n, sizeof(carros[0]), compara_placa);

  FILE * ord = fopen("carro.ord", "wb");
  if(ord == NULL){
    printf("ERRO AO CRIAR ARQUIVO ORDENADO\n");
    exit(100);
  }

  for(int i = 0; i < n; i++){
    fwrite(&carros[i], sizeof(carros[0]), 1, ord);
  }
  
  fclose(ord);
  printf("OPERACAO REALIZADA COM SUCESSO!\n");
}

void mostra_registros_ord(){
  FILE * fp = fopen("carro.ord", "rb");
  if(fp == NULL){
    printf("ERRO AO ABRIR ARQUIVO DE EXIBICAO DE REGISTROS ORDENADOS\n");
    exit(100);
  }

  fseek(fp, 0, SEEK_SET);

  CARRO carro;

  system("cls");
  while(!feof(fp)){
    if(fread(&carro, sizeof(carro), 1, fp) > 0){
      printf("-------------------\n");
      printf("ID: %d\n", carro.id_reg);
      printf("PLACA: %s\n", carro.placa);
      printf("MODELO: %s\n", carro.modelo);
      printf("FABRICANTE: %s\n", carro.fabricante);
      printf("ANO DE FABRICACAO: %d\n", carro.ano_fabricacao);
      printf("ANO DO MODELO: %d\n", carro.ano_modelo);
      printf("COMBUSTIVEL: %s\n", carro.combustivel);
      printf("COR: %s\n", carro.cor);
      int cont = 0;
      for(int i = 0; i < 8; i++){
        if(carro.opcional[i] == 1){
          printf("OPCIONAL %d: %s\n", cont+1, opcionais[i]);
        }
      }
      printf("PRECO DE COMPRA: %.2f\n", carro.preco_compra);
    }
  }

  fclose(fp);
}

int main(){
  int esc;
  do{
    printf("---------------\n");
    printf("1. INFORMAR QUANTIDADE DE REGISTROS EM \"carro.dbf\"\n");
    printf("2. ORDERNAR REGISTROS DE \"carro.dbf\" EM ORDEM CRESCENTE PELA PLACA E ARMAZENA-LOS EM \"carro.ord\"\n");
    printf("3. MOSTRAR REGISTROS DE \"carro.ord\"\n");
    printf("4. INFORMAR A QUANTIDADE DE CARRO PARA CADA TIPO DE COMBUSTIVEL\n");
    printf("5. INFORMAR A QUANTIDADE DE CARRO PARA CADA TIPO DE OPCIONAL\n");
    printf("6. SAIR\n");
    printf("ESCOLHA -> ");
    scanf("%d", &esc);
    system("cls");
    switch (esc){
      case 1:
        conta_registros();
        system("pause");
        break;
      case 2:
        ordena_registros();
        system("pause");
        break;
      case 3:
        mostra_registros_ord();
        system("pause");
        break;
      default:
        break;
    }
  }while(esc != 6);

  return 0;
}