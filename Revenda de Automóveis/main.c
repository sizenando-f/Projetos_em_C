#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TAM 50

char opcionais[][TAM]={ {"4.portas"}, {"cambio.automatico"}, {"vidros.eletricos"}, {"abs"}, {"air.bag"}, {"ar.condicionado"},
  {"banco.couro"}, {"sensor.estacionamento"}};

struct CARRO {
  char placa[9]; //AAA-1234
  char modelo[TAM]; //gol, celta, palio, ...
  char fabricante[TAM]; //chevrolet, fiat, ...
  int ano_fabricacao; //1980 à 2016
  int ano_modelo; //1980 à 2017
  char combustivel[TAM]; //alcool, gasolina, flex, diesel
  char cor[TAM]; //branca, prata, preta
  int opcional[8]; //ver matriz opcionais
  float preco_compra;
};

struct ENDERECO {
  char rua[TAM];
  int numero;
  char bairro[TAM];
  char cidade[TAM];
  char estado[3];
  char cep[11]; //99.999-999
};

struct TELEFONE {
 char telefone[14]; //99 99999-9999
};

struct CLIENTE {
  char nome[TAM];
  char cpf[15]; //999.999.999-99
  struct ENDERECO endereco;
  struct TELEFONE residencial;
  struct TELEFONE celular[5];
  float renda_mensal;
};

struct DATA {
  int dia, mes, ano;
};

struct VENDA_CARRO {
  char placa_car[9];
  char cpf_cli[15];
  float preco_venda;
  struct DATA data_venda;
};

void geraPlaca(char * placa){
  char letras[27][2];
  char *numeros[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

  for(int i = 0; i < 26; i++){
    letras[i][0] = 'A' + i;
    letras[i][1] = '\0';
  }

  srand(time(NULL));

  for(int i = 0; i < 3; i++){
    int nAleatorio = rand() % 26;
    strcat(placa, letras[nAleatorio]);
  }
  strcat(placa, "-");
  for(int i = 0; i < 3; i++){
    int nAleatorio = rand() % 9;
    strcat(placa, numeros[nAleatorio]);
  }

}

void inserirCarro(){
  char placa[9];
  geraPlaca(placa);
  printf("%s", placa);
}

void menuCarro(){
  int esc;
  do{
    printf("1. INSERIR UM CARRO\n");
    printf("2. EXCLUIR UM CARRO\n");
    printf("3. LISTAR CARROS DISPONIVEIS PARA VENDAORDENADOS POR FABRICANTE E MODELO\n");
    printf("4. LISTAR CAROS DISPONIVEIS PARA VENDA POR SELECAO DE UM OU MAIS OPCIONAIS\n");
    printf("5. LISTAR OS CARROS DISPONIVEIS PARA VENDA POR SELECAO DA FAIXA DO ANO DE FABRICACAO\n");
    printf("6. SAIR\n");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        break;
      default:
        break;
    }
  } while(esc != 6);
}

int main(){
  int esc;
  do{
    printf("1. CARRO\n");
    printf("2. CLIENTE\n");
    printf("3. VENDA\n");
    printf("4. SAIR\n");
    scanf("%d", &esc);

    switch (esc){
      case 1:
        // menuCarro();
        inserirCarro();
        break;
      default:
        break;
    }
  } while(esc != 4);

  return 0;
}