#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Exibe a matriz.
 * Será exibido a matriz no console
 * @param matriz Matriz de no máximo 10x10
 * @param ordem Dimensão da matriz
 */
void exibeMatriz(double matriz[10][10], unsigned ordem){
  for(unsigned i = 0; i < ordem; i++){
    printf("| ");
    for(unsigned j = 0; j < ordem; j++){
    printf("%5.1f", matriz[i][j]);
    }
    printf("|\n");
  }
}

/**
 * @brief Deixa o pivô da coluna com o maior número.
 * Compara um valor de cada linha da coluna escolhida e compara qual é o maior em módulo. Após isso, realiza a troca de linhas caso necessário
 * para deixar o pivô da coluna sendo maior entre eles. Exibindo qual linha foi trocada.
 * @param matriz Matriz onde estará os valores
 * @param ordem Dimensão da matriz
 * @param linhaPivo Linha onde o número pivô está
 * @param colunaPivo Coluna onde o número pivô está
 */
void fixColunaPivo(double matriz[10][10], unsigned ordem, unsigned linhaPivo, unsigned colunaPivo){
  double maior = matriz[linhaPivo][colunaPivo];
  double temp2, temp1;
  unsigned linha = linhaPivo;

  for(unsigned i = linhaPivo; i < ordem; i++){
    temp1 = matriz[i][colunaPivo];
    temp2 = maior;
    if(matriz[i][colunaPivo] < 0){
      temp1 *= -1;
    }
    if(maior < 0){
      temp2 *= -1;
    }
    if(temp1 > temp2){
      maior = matriz[i][colunaPivo];
      linha = i;
    }
  }

  double temp;
  if(maior != matriz[linhaPivo][colunaPivo]){
    printf("\n [ ! ] Trocando linha (%d) com linha (%d)\n", linha+1, linhaPivo+1);
    printf("\n [ -> ] Antes: \n");
    exibeMatriz(matriz, ordem);
    for(unsigned i = 0; i < ordem; i++){
      temp = matriz[linhaPivo][i];
      matriz[linhaPivo][i] = matriz[linha][i];
      matriz[linha][i] = temp;
    }
    printf("\n [ -> ] Depois: \n");
    exibeMatriz(matriz, ordem);
  }
}

/**
 * @brief Cria a matriz.
 * Usuário inicializa os valores da matriz seguido de uma confirmação onde será repetido indefinidas vezes até que o usuário aceite a matriz.
 * @param matriz Matriz de no máximo 10x10
 * @param ordem Ponteiro para armazenar a dimensão da matriz
 */
void defineMatriz(double matriz[10][10], unsigned *ordem){
  char esc;
  do{
    printf(" [ <- ] Insira a ordem da matriz > ");
    scanf("%d", &(*ordem));
    for(unsigned i = 0; i < *ordem; i++){
      system("cls");
      printf("------- Linha (%d) --------\n", i+1);
      for(unsigned j = 0; j < *ordem; j++){
        printf("[ <- ] Coluna %d > ", j+1);
        scanf("%lf", &matriz[i][j]);
      }
    }
    system("cls");
    printf(" [ -> ] A matriz criada foi: \n");
    exibeMatriz(matriz, *ordem);
    printf(" [ <- ] Deseja continuar? (S/n) > ");
    scanf(" %c", &esc);
  } while(esc == 'N' || esc == 'n');
}

/**
 * @brief Multiplica os elementos de uma linha especifica.
 * Utiliza o multiplicador e aplica em cada elemento de uma linha desejada.
 * @param matriz Matriz para ser feita a operação
 * @param ordem Dimensão da matriz
 * @param multiplicador Multiplicador para ser aplicado nos elementos
 * @param linhaPivo Linha onde se econtra o pivô
 * @param linhaAlt Linha a qual deseja ser alterada 
 */
void operaMatriz(double matriz[10][10], unsigned ordem, double multiplicador, unsigned linhaPivo, unsigned linhaAlt){
  printf(" [ -> ] Aplicando multiplicador na linha (%d)...\n\n [ -> ] Antes:\n", linhaAlt+1);
  exibeMatriz(matriz, ordem);
  for(unsigned i = 0; i < ordem; i++){
    matriz[linhaAlt][i] = (multiplicador * matriz[linhaPivo][i]) + matriz[linhaAlt][i];
  }
  printf("\n [ -> ] Depois:\n");
  exibeMatriz(matriz, ordem);
}

/**
 * @brief Transforma uma matriz numa matriz superior.
 * Através de trocas consecutivas de linhas caso necessário usando a função auxiliar `fixColunaPivo` e aplica os multiplicadores nas linhas
 * para que os valores da pirâmide inferior sejam zerados com a função `operaMatriz`.
 * @param matriz Matriz para ser transformada
 * @param ordem Dimensão da matriz
 */
void transformaMatrizSuperior(double matriz[10][10], unsigned ordem){
  double multiplicador;
  system("cls");
  for(unsigned coluna = 0; coluna < ordem-1; coluna++){
    printf("\n=-=-=-=-=-=-=-= Operacao numero: [%d] =-=-=-=-=-=-=-=", coluna+1);
    fixColunaPivo(matriz, ordem, coluna, coluna);
    for(unsigned linha = coluna+1; linha < ordem; linha++){
      multiplicador = -(matriz[linha][coluna]/matriz[coluna][coluna]);
      printf("\n [ -> ] Multiplicador do indice (%d)(%d) descoberto: (%lf)\n", linha+1, coluna+1, multiplicador);
      operaMatriz(matriz, ordem, multiplicador, coluna, linha);
    }
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  }
}

int main(){
  double matriz[10][10];
  char esc;
  unsigned ordem;
  do{
    defineMatriz(matriz, &ordem);
    transformaMatrizSuperior(matriz, ordem);
    printf("\n\n [ -> ] Resultado final: \n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    exibeMatriz(matriz, ordem);
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf(" [ <- ] Deseja testar outra matriz? (S/n) > ");
    scanf(" %c", &esc);
  }while(esc == 'S' || esc == 's');
  return 0;
}