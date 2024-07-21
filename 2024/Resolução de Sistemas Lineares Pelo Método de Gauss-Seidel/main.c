#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Gera double aleatorio.
 * @param min Valor mínimo
 * @param max valor máximo
 */
double numAleatorio(double min, double max) {
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}

/**
 * @brief Gera uma matriz com valores aleatórios.
 * Utilizando a função auxiliar `numAleatorio`, realiza a inserção de valores aleatório em todas as casas da matriz
 * @param matriz Matriz a ser inicializada
 * @param ordem Dimensão da matriz
 */
void matrizAleatoria(double matriz[10][10], unsigned ordem, double termoInd[]){
  srand(time(NULL));
  for (unsigned i = 0; i < ordem; i++) {
    for(unsigned j = 0; j < ordem; j++){
      matriz[i][j] = numAleatorio(-100, 100);
    }
    termoInd[i] = numAleatorio(-100, 100);
  }
}

/**
 * @brief Exibe a matriz.
 * Será exibido a matriz no console
 * @param matriz Matriz de no máximo 10x10
 * @param ordem Dimensão da matriz
 */
void exibeMatriz(double matriz[10][10], unsigned ordem, double termoInd[]){
  for(unsigned i = 0; i < ordem; i++){
    for(unsigned j = 0; j < ordem; j++){
      if(matriz[i][j] < 0 && j != 0){
        printf("%.2fx%d ", matriz[i][j]*-1, j+1);
      } else {
        printf(" %.2fx%d ", matriz[i][j], j+1);
      }

      if(matriz[i][j+1] < 0 && j != ordem-1){
        printf(" - ");
      }
      if(matriz[i][j+1] >= 0 && j != ordem-1){
        printf(" + ");
      }
    }
    printf("= %lf ", termoInd[i]);
    printf("\n");
  }
}

/**
 * @brief Cria a matriz.
 * Usuário inicializa os valores da matriz seguido de uma confirmação onde será repetido indefinidas vezes até que o usuário aceite a matriz.
 * @param matriz Matriz de no máximo 10x10
 * @param ordem Ponteiro para armazenar a dimensão da matriz
 */
void defineMatriz(double matriz[10][10], unsigned *ordem, double termoInd[]){
  char esc;
  int forma;
  do{
    do{
      printf(" [ <- ] Insira a ordem do sistema > ");
      scanf(" %d", &(*ordem));
      if(*ordem < 1 || *ordem > 10){
        printf("[ #<- ERRO ] A ordem precisa ser entre 1 e 10, tente novamente...\n");
      }
    }while(*ordem < 1 || *ordem > 10);
    system("cls");
    do{
      printf("-----------------------\n");
      printf(" [\e[0;34m 1 \e[0;37m] Manualmente\n");
      printf(" [\e[0;34m 2 \e[0;37m] Automaticamente\n");
      printf("-----------------------\n");
      printf(" [ <- ] Como deseja preencher o sistema linear? > ");
      printf("\e[0;34m");
      scanf("%d", &forma);
      printf("\e[0;37m");
      switch (forma) {
      case 1:
        for(unsigned i = 0; i < *ordem; i++){
          system("cls");
          printf("------- Linha (\e[0;33m %d \e[0;37m) --------\n", i+1);
          for(unsigned j = 0; j < *ordem; j++){
            printf("[ <- ] Coeficiente %d > ", j+1);
            printf("\e[0;34m");
            scanf("%lf", &matriz[i][j]);
            printf("\e[0;37m");
          }
          printf("[ <- ] Termo independente > ");
          scanf("%lf", &termoInd[i]);
        }
        break;
      case 2:
        matrizAleatoria(matriz, *ordem, termoInd);
        break;
      default:
        system("cls");
        printf("\e[0;31m");
        printf("[ #<- ERRO ] Escolha invalida, tente novamente...\n");
        printf("\e[0;37m");
        break;
      }
    }while(forma != 1 && forma != 2);
    system("cls");
    printf(" [ -> ] O sistema linear criado foi: \n");
    exibeMatriz(matriz, *ordem, termoInd);
    printf(" [ <- ] Deseja continuar? (S/n) > ");
    printf("\e[0;34m");
    scanf(" %c", &esc);
    printf("\e[0;37m");
    system("cls");
  } while(esc == 'N' || esc == 'n');
}


/**
 * @brief Inicializa solução inicial.
 * O usuário insere por iteração cada xn da solução inicial.
 * @param solucao Vetor onde será armazenado cada xn
 * @param ordem Dimensão do sistema linear para saber xn existirão
 */
void inicializaSolucao(double solucao[10], unsigned int ordem){
  char esc;
  do{
    system("cls");
    printf(" --- Solucao Inicial ---\n");
    for(unsigned i = 0; i < ordem; i++){
      printf("[ <- ] x%d > ", i + 1);
      scanf("%lf", &solucao[i]);
    }
    printf("[ <- ] Deseja continuar? (S/n) > ");
    scanf(" %c", &esc);
  }while(esc == 'N' || esc == 'n');
}

/**
 * @brief Soluciona o sistema passado.
 * Armazena o resultado da solução do vetor de solução
 * @param matriz Sistema linear para ser aplicado as operações
 * @param termoInd Vetor com os termos independentes
 * @param solucao Vetor de solução anterior ou inicial
 * @param solucaoPost Vetor onde será armazenado a solução descoberta
 * @param ordem Dimensão do sistema linear
 */
void solucionaSistema(double matriz[10][10], double termoInd[10], double solucao[10], double solucaoPost[10], unsigned ordem){
  double temp, multiplicador;

  for(unsigned i = 0; i < ordem; i++){
    solucaoPost[i] = solucao[i];
  }

  for(unsigned i = 0; i < ordem; i++){
    multiplicador = 1/matriz[i][i];
    temp = multiplicador * termoInd[i];
    for(unsigned j = 0; j < ordem; j++){
      if(j == i){
        continue;
      }
      temp += (multiplicador*(matriz[i][j]*-1))*solucaoPost[j];
    }
    solucaoPost[i] = temp;
  }
}

/**
 * @brief Descobre a precisão de duas soluções.
 * Cálula o maior em módulo da diferença entre a solução anterior e atual e divide pelo maior em módulo da solução atual.
 * @param solucaoAnt Vetor com a solução anterior
 * @param solucaoPost Vetor com a solução atual
 * @param ordem Dimensão da matriz
 */
double descobrePrecisao(double solucaoAnt[10], double solucaoPost[10], unsigned ordem){
  double maiorModulo = 0, maiorSolucao = 0, temp;
  for(unsigned i = 0; i < ordem; i++){
    temp = solucaoAnt[i] - solucaoPost[i];
    if(temp < 0) temp *= -1;
    maiorModulo = temp > maiorModulo ? temp : maiorModulo;
  }
  for(unsigned i = 0; i < ordem; i++){
    temp = solucaoPost[i];
    if(temp < 0) temp *= -1;
    maiorSolucao = temp > maiorSolucao ? temp : maiorSolucao;
  }
  return maiorModulo/maiorSolucao;
}

int main(){
  double matriz[10][10], termoInd[10], precisao, solucaoAnt[10], solucaoPost[10], resp;
  unsigned ordem, cont = 1;
  system("cls");
  defineMatriz(matriz, &ordem, termoInd);
  printf(" [ <- ] Insira a precisao desejada > ");
  scanf("%lf", &precisao);
  inicializaSolucao(solucaoAnt, ordem);
  system("cls");
  do{
    solucionaSistema(matriz, termoInd, solucaoAnt, solucaoPost, ordem);
    printf("\n ---- SOLUCAO [X%d] ----\n", cont++);
    for(unsigned i = 0; i < ordem; i++){
      printf(" | %10.7lf |\n", solucaoPost[i]);
    }
    printf(" ------------------------\n");
    resp = descobrePrecisao(solucaoAnt, solucaoPost, ordem);
    printf(" [ -> ] Precisao descoberta: [%.7lf]\n", resp);
    if(resp > precisao){
      for(unsigned i = 0; i < ordem; i++){
        solucaoAnt[i] = solucaoPost[i];
      }
      printf(" [ -> ] Nova tentativa...\n");
    }
  }while(resp > precisao);

  printf(" ---- SOLUCAO FINAL ----\n");
  for(unsigned i = 0; i < ordem; i++){
    printf(" | %10.7lf |\n", solucaoPost[i]);
  }
  printf(" -----------------------\n");
  return 0;
}