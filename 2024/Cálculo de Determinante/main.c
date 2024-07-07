#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Descobre maior valor em módulo de uma coluna.
 * Compara um valor de cada linha da coluna escolhida e compara qual é o maior em módulo.
 * @param matriz Matriz onde estará os valores
 * @param ordem Dimensão da matriz
 * @param coluna Coluna a qual deve ser feito a comparação
 * @return Linha com o maior valor em módulo
 */
int descobreMaior(int matriz[10][10], unsigned ordem, unsigned coluna){
  int maior = matriz[0][coluna];
  unsigned temp1, temp2, linha = 0;
  for(unsigned i = 0; i < ordem; i++){
    temp1 = matriz[i][coluna];
    temp2 = maior;
    if(matriz[i][coluna] < 0){
      temp1 *= -1;
    }
    if(maior < 0){
      temp2 *= -1;
    }
    if(temp1 > temp2){
      maior = matriz[i][coluna];
      linha = i;
    }
  }
  return linha;
}

/**
 * @brief Exibe a matriz.
 * Será exibido a matriz no console
 * @param matriz Matriz de no máximo 10x10
 * @param ordem Dimensão da matriz
 */
void exibeMatriz(int matriz[10][10], unsigned ordem){
  for(unsigned i = 0; i < ordem; i++){
    printf("| ");
    for(unsigned j = 0; j < ordem; j++){
    printf("%d ", matriz[i][j]);
    }
    printf("|\n");
  }
}
/**
 * @brief Cria a matriz.
 * Usuário inicializa os valores da matriz seguido de uma confirmação onde será repetido indefinidas vezes até que o usuário aceite a matriz.
 * @param matriz Matriz de no máximo 10x10
 * @param ordem Ponteiro para armazenar a dimensão da matriz
 */
void defineMatriz(int matriz[10][10], unsigned *ordem){
  char esc;
  printf("[ -> ] Inicialzando matriz...\n");
  do{
    printf("[ <- ] Insira a ordem da matriz > ");
    scanf("%d", &(*ordem));
    for(unsigned i = 0; i < *ordem; i++){
      printf("------- Linha %d --------\n", i+1);
      for(unsigned j = 0; j < *ordem; j++){
        printf("Coluna %d >", j+1);
        scanf("%d", &matriz[i][j]);
      }
    }
    system("cls");
    printf("[ -> ] A matriz criada foi: \n");
    exibeMatriz(matriz, *ordem);
    printf("[ <- ] Deseja continuar? (S/n) > ");
    scanf(" %c", &esc);
  } while(esc == 'N' || esc == 'n');
}

int main(){
  int matriz[10][10];
  char esc;
  unsigned ordem;
  do{
    defineMatriz(matriz, &ordem);
    printf("[ <- ] Deseja testar outra matriz? (S/n) > ");
    scanf(" %c", &esc);
  }while(esc == 'S' || esc == 's');
  return 0;
}