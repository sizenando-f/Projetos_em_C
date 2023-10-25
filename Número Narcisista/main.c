// ADICIONAR -lm AO COMPILAR -> cc -o app main.c myList.a -lm
// -lm eh referente a biblioteca math

#include <stdio.h>      // Entrada e saida
#include <stdlib.h>     // system()
#include <math.h>       // pow()
#include <unistd.h>     // sleep()
#include <myList.h>     // Biblioteca do trabalho

// Retorna o resultado do calculo da potencia dos algarismos
int numero_narcisista(int num, int algarismos){
  int soma = 0;

  for(int i = 0; i < algarismos; i++){
    int temp = num%10;  // Pega ultimo algarismo
    soma += pow(temp, algarismos);
    num /= 10;          // Passa para o proximo algarismo
  }

  return soma;
}

// Retorna  quantos algarismos um numero possui
int descobre_algarismos(int num){
  int temp = num;
  int cont = 0;
  while(temp != 0){
    if(temp != 0){      // Verifica se numero possui algarismos
      cont++;
    }
    temp /= 10;         // Diminui um algarismo do numero
  }
  return cont;
}

int main(){
  int fim = 1, indice = 0, esc, excluir, conta_n = 0;
  inicializa(); // Inicializa lista
  do{
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
          printf("INSIRA UM VALOR FINAL (INTERVALO ACEITO: 10 - 1000000): ");
          scanf("%d", &fim);
          if(fim > 1000000 || fim < 10){        // Valida entrada
            printf("ENTRADA INCORRETA, TENTE NOVAMENTE!\n");
            sleep(2);
          }
        }while(fim > 1000000 || fim < 10);

        printf("DESCOBRINDO NUMEROS NARCISISTAS ENTRE 10 E %d...\n", fim);
        for(int i = 10; i <= fim; i++){
          int potencia = descobre_algarismos(i);
          int num = numero_narcisista(i, potencia);
          if(num == i){         // Verifica se eh um numero narcisista
            if(!busca(i)){      // Verifica se numero ja existe na lista
              insereOrdem(i);   // Insere numero na lista
              conta_n++;        // Conta quantos numeros esta na lista
            }
          }
        }
        sleep(3);
        system("clear");
        break;

      case 2:
        if(conta_n){            // Verifica se a lista possui numeros
          printf("QUAL NUMERO DESEJA EXCLUIR? ~# ");
          scanf("%d", &excluir);
          if(busca(excluir)){   // Verifica se numero existe
            retira(excluir);    // Remove numero
            printf("EXCLUSAO BEM SUCEDIDA\n");
            conta_n--;
            system("clear");
            imprime();
          } else{
            system("clear");
            printf("--------------------\n");
            printf("NUMERO INEXISTENTE!\n");
            printf("--------------------\n");
          }
        } else{
          system("clear");
          printf("------------------------------\n");
          printf("EH NECESSARIO DESCOBRIR OS VALORES NARCISISTAS, POR FAVOR, ESCOLHA A OPCAO 1\n");
          printf("------------------------------\n");
        }
        break;

      case 3:
        system("clear");
        if(conta_n){    // Verifica se a lista possui numeros
          printf("---- NUMEROS DESCOBERTOS ----\n");
          imprime();
          printf("\n-----------------------------\n");
        } else {
          printf("------------------------------\n");
          printf("NAO EXISTE NUMEROS NARCISISTAS NA LISTA, POR FAVOR, ESCOLHA A OPCAO 1\n");
          printf("------------------------------\n");
        }
        break;

      case 4:
        printf("ENCERRANDO PROGRAMA...\n");
        esvazia();      // Encerra lista
        sleep(2);
        break;
      default:
        printf("ENTRADA INVALIDA, TENTE NOVAMENTE!\n");
        sleep(2);
        system("clear");
        break;
    }
  }while(esc != 4);

  return 0;
}
