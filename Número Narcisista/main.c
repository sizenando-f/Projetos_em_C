#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h> // unistd.h - sleep() ou a usleep()

int numero_narcisista(int num, int algarismos){
  int soma = 0;

  for(int i = 0; i < algarismos; i++){
    soma += pow((num%10), algarismos);
    num /= 10;
  }

  return soma;
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
  int fim = 1, indice = 0, esc, check = 0;
  int * numeros = malloc(fim * sizeof(int));
  do{
    system("cls");
    printf("-----# NUM3R0 N4RC1S1ST4 #-----\n");
    printf(" 1. ARMAZENAR NUMEROS\n");
    printf(" 2. EXCLUIR NUMERO\n");
    printf(" 3. MOSTRAR NUMEROS NARCISISTAS\n");
    printf(" 4. SAIR\n");
    printf("-------------------------------\n");
    printf("ESCOLHA ~# ");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        do{
          printf("INSIRA UM VALOR FINAL (INTERVALO ACEITO: 10 - 1.000.000): ");
          scanf("%d", &fim);
          if(fim > 1000000 || fim < 10){
            printf("ENTRADA INCORRETA, TENTE NOVAMENTE!\n");
            system("pause");
          }

        }while(fim > 1000000 || fim < 10);

        printf("DESCOBRINDO NUMEROS NARCISISTAS ENTRE 10 E %d...\n", fim);
        for(int i = 10; i <= fim; i++){
          int potencia = descobre_algarismos(i);
          int num = numero_narcisista(i, potencia);
          if(num == i) numeros[indice++] = i; // Armazena no vetor
        }
        check = 1; // Numeros foram criados

        break;

      case 2:
        if(check){
          printf("EXCLUIDO\n"); // usar função da biblioteca de exclusão e depois exibir
        } else{
          printf("EH NECESSARIO DESCOBRIR OS VALORES NARCISISTA, POR FAVOR, VOLTE E ESCOLHA A OPCAO 1\n");
        }
        system("pause");
        break;

      case 3:
        if(check){
          printf("---- NUMEROS DESCOBERTOS ----\n");
          for(int i = 0; i < indice; i++){
            printf("%d ", numeros[i]);
          }
          printf("\n-----------------------------\n");
        } else {
          printf("EH NECESSARIO DESCOBRIR OS VALORES NARCISISTA, POR FAVOR, VOLTE E ESCOLHA A OPCAO 1\n");
        }
        system("pause");
        break;
      
      case 4:
        printf("ENCERRANDO PROGRAMA\n");
        Sleep(2); // Usar função que encerra vetor
        break;
      default:
        printf("ENTRADA INVALIDA, TENTE NOVAMENTE!\n");
        system("pause");
        break;
    }
  }while(esc != 4);

  free(numeros);

  return 0;
}