// T8

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

// Conta quantos registros existem
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

  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
  printf("O ARQUIVO POSSUI %d REGISTROS\n", cont);
  printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

// Retorna as placas ordenadas de forma crescente de todos carros recebidos
int compara_placa(const void *a, const void*b){
  const char *placa1 = ((CARRO *)a)->placa;
  const char *placa2 = ((CARRO *)b)->placa;
  return strcmp(placa1, placa2);
}

// Ordena os registros e armazena noutro arquivo
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

  qsort(carros, n, sizeof(carros[0]), compara_placa); // Função de ordenação da biblioteca stdlib.h

  FILE * ord = fopen("carro.ord", "wb");
  if(ord == NULL){
    printf("ERRO AO CRIAR ARQUIVO ORDENADO\n");
    exit(100);
  }

  for(int i = 0; i < n; i++){
    fwrite(&carros[i], sizeof(carros[0]), 1, ord);
  }
  
  fclose(ord);
  printf("------------- SUCESSO -------------\n");
  printf("| OPERACAO REALIZADA COM SUCESSO! |\n");
  printf("-----------------------------------\n");
}

// Exibe os registros ordenados
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
      printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
      printf("-                ID- > %ld\n", carro.id_reg);
      printf("-             PLACA -> %s\n", carro.placa);
      printf("-            MODELO -> %s\n", carro.modelo);
      printf("-        FABRICANTE -> %s\n", carro.fabricante);
      printf("- ANO DE FABRICACAO -> %d\n", carro.ano_fabricacao);
      printf("-     ANO DO MODELO -> %d\n", carro.ano_modelo);
      printf("-       COMBUSTIVEL -> %s\n", carro.combustivel);
      printf("-               COR -> %s\n", carro.cor);
      int cont = 0;
      for(int i = 0; i < 8; i++){
        if(carro.opcional[i] == 1){
          printf("-       OPCIONAL %d -> %s\n", cont+1, opcionais[i]); cont++;
        }
      }
      printf("-   PRECO DE COMPRA -> R$%.2f\n", carro.preco_compra);
    }
  }

  fclose(fp);
}

// Descobre quais combustíveis existem
void descobre_combustiveis(char combustiveis[4][TAM]){
  FILE * fp = fopen("carro.dbf", "rb");
  if(fp == NULL){
    printf("ERRO AO ABRIR ARQUIVO PARA ORDENACAO\n");
    exit(100);
  }

  int n = 0;
  CARRO carro;

  fseek(fp, 0, SEEK_SET);

  while(!feof(fp)){
    if(fread(&carro, sizeof(carro), 1, fp) > 0){
      int check = 0;
      for(int i = 0; i < n; i++){
        if(strcmp(carro.combustivel, combustiveis[i]) == 0){
          check = 1;
        }
      }
      if(!check){
        strcpy(combustiveis[n++], carro.combustivel);
      }
    }
  }

  fclose(fp);
}

// Mostra quantidade de carros para cada tipo de combustível
void combustivel_para_carro(){
  char combustiveis[4][TAM];
  int cont_comb[4];

  descobre_combustiveis(combustiveis);

  FILE * fp = fopen("carro.dbf", "rb");
  if(fp == NULL){
    printf("ERRO AO ABRIR ARQUIVO PARA ORDENACAO\n");
    exit(100);
  }

  CARRO carro;

  for(int i = 0; i < 4; i++){
    fseek(fp, 0, SEEK_SET);
    int cont = 0;
    while(!feof(fp)){
      if(fread(&carro, sizeof(carro), 1, fp) > 0){
        if(strcmp(carro.combustivel, combustiveis[i]) == 0){
          cont++;
        }
      }
    }
    cont_comb[i] = cont;
  }
  
  for(int i = 0; i < 4; i++){
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("- COMBUSTIVEL -> %s\n- QUANTIDADE -> %d\n", combustiveis[i], cont_comb[i]);
  }

  fclose(fp);
}

// Mostra a quantidade de carros com cada opcional
void opcional_para_carro(){
  FILE * fp = fopen("carro.dbf", "rb");
  if(fp == NULL){
    printf("ERRO AO ABRIR ARQUIVO PARA ORDENACAO\n");
    exit(100);
  }

  CARRO carro;
  int cont_opc[8];

  for(int i = 0; i < 8; i++){
    fseek(fp, 0, SEEK_SET);
    int cont = 0;
    while(!feof(fp)){
      if(fread(&carro, sizeof(carro), 1, fp) > 0){
        for(int j = 0; j < 8; j++){
          if(carro.opcional[j] == 1){
            if(j == i){
              cont++;
            }
          }
        }
      }
    }
    cont_opc[i] = cont;
  }

  for(int i = 0; i < 8; i++){
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("- OPCIONAL -> %s\n- QUANTIDADE -> %d\n", opcionais[i], cont_opc[i]);
  }

  fclose(fp);
}

int main(){
  int esc;
  do{
    system("cls");
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("| 1. INFORMAR QUANTIDADE DE REGISTROS EM \"carro.dbf\"                                                   |\n");
    printf("| 2. ORDERNAR REGISTROS DE \"carro.dbf\" EM ORDEM CRESCENTE PELA PLACA E ARMAZENA-LOS EM \"carro.ord\"     |\n");
    printf("| 3. MOSTRAR REGISTROS DE \"carro.ord\"                                                                  |\n");
    printf("| 4. INFORMAR A QUANTIDADE DE CARRO PARA CADA TIPO DE COMBUSTIVEL                                      |\n");
    printf("| 5. INFORMAR A QUANTIDADE DE CARRO PARA CADA TIPO DE OPCIONAL                                         |\n");
    printf("| 6. SAIR                                                                                              |\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
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
      case 4:
        combustivel_para_carro();
        system("pause");
        break;
      case 5:
        opcional_para_carro();
        system("pause");
        break;
      case 6:
        printf("-------- AVISO -------\n");
        printf("| PROGRAMA ENCERRADO |\n");
        printf("----------------------\n");
        break;
      default:
        printf("ENTRADA INVALIDA\n");
        system("pause");
        break;
    }
  }while(esc != 6);

  return 0;
}