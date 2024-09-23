#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>

void geraNaleatorio(int *vetor, int n){
  for(int i = 0; i < n; i++){
    vetor[i] = rand()%400;
  }
}

// Opção 1 : Bubble-sort original
void bubbleSortOriginal(int vetor[], unsigned n){
  for(unsigned i = 0; i < n; i++){
    for(unsigned j = 0; j < n-1; j++){
      if(vetor[j] > vetor[j+1]){
        int aux = vetor[j+1];
        vetor[j+1] = vetor[j];
        vetor[j] = aux;
      }
    }
  }
}

// Opção 2 : Bubble-sort melhorado
void bubbleSortMelhorado(int vetor[], unsigned n){
  int mudou = 1;
  unsigned nLinha = n, guarda = n;

  while(mudou){
    unsigned j = 0;
    mudou = 0;
    while(j < nLinha){
      if(vetor[j] > vetor[j+1]){
        int aux = vetor[j+1];
        vetor[j+1] = vetor[j];
        vetor[j] = aux;
        mudou = 1;
        guarda = j;
      }
      j++;
    }
    nLinha = guarda;
  }
}

// Opção 3 : Insertion-sort
void insertionSort(int *vetor, unsigned n){
  for(int i = 1; i < (int) n; i++){
    int prov = vetor[i];
    int valor = vetor[i];
    int j = i - 1;

    while(j >= 0 && valor < vetor[j]){
      vetor[j + 1] = vetor[j];
      j--;
    }
    vetor[j+1] = prov;
  }
}

// Função auxiliar de opção 4
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Opção 4 : Mergesort
void mergeSort(int arr[], int esq, int dir)
{
    if (esq < dir) {
        int m = esq + (dir - esq) / 2;

        mergeSort(arr, esq, m);
        mergeSort(arr, m + 1, dir);

        merge(arr, esq, m, dir);
    }
}

// Função auxiliar para opções 5, 6 e 7
int mediana(int vet[], int ini, int fim){
  int meio = ini + (fim+1 - ini)/2;
  if(vet[meio] > vet[ini] && vet[meio] < vet[fim]) return meio;
  if(vet[ini] > vet[meio] && vet[ini] < vet[fim]) return ini; 
  return fim;
}

// Opção 5 : Quicksort com pivô sendo o último elemento
void quicksort1(int vetor[], int ini, int fim){
  int i = ini, j = fim, pivo = vetor[fim];
  while(i <= j){
    while(vetor[i] < pivo) i++;
    while(vetor[j] > pivo) j--;
    if(i <= j){
      int aux = vetor[j];
      vetor[j] = vetor[i];
      vetor[i] = aux;
      i++; j--;
    }
  }
  if(ini < j) quicksort1(vetor, ini, j);
  if(i < fim) quicksort1(vetor, i, fim);
}

// Opção 6 : Quicksort com pivô sendo um elemento aleatório
void quicksort2(int vetor[], int ini, int fim){
  int nAleatorio = rand() % (fim - ini + 1) + ini;
  int i = ini, j = fim, pivo = vetor[nAleatorio];
  while(i <= j){
    while(vetor[i] < pivo) i++;
    while(vetor[j] > pivo) j--;
    if(i <= j){
      int aux = vetor[j];
      vetor[j] = vetor[i];
      vetor[i] = aux;
      i++; j--;
    }
  }
  if(ini < j) quicksort2(vetor, ini, j);
  if(i < fim) quicksort2(vetor, i, fim);
}


// Opção 7 : Quicksort com pivô sendo a mediana de três
void quicksort3(int vetor[], int ini, int fim){
  int i = ini, j = fim, pivo = vetor[mediana(vetor, ini, fim)];
  while(i <= j){
    while(vetor[i] < pivo) i++;
    while(vetor[j] > pivo) j--;
    if(i <= j){
      int aux = vetor[j];
      vetor[j] = vetor[i];
      vetor[i] = aux;
      i++; j--;
    }
  }
  if(ini < j) quicksort3(vetor, ini, j);
  if(i < fim) quicksort3(vetor, i, fim);
}

// Opção 8 : Heapsort
void heapsort(int vetor[], int n){
  int i = n/2, pai, filho, t;
  while(1){
    if(i > 0){
      i--;
      t = vetor[i];
    } else {
      n--;
      if(n <= 0) return;
      t = vetor[n];
      vetor[n] = vetor[0];
    }
    pai = i;
    filho = i * 2 + 1;
    while(filho < n){
      if((filho + 1 < n) && (vetor[filho+1] > vetor[filho])) filho++;
      if(vetor[filho] > t){
        vetor[pai] = vetor[filho];
        pai = filho;
        filho = pai * 2 + 1;
      } else {
        break;
      }
    }
    vetor[pai] = t;
  }
}

// Argc 0 = Programa (ignora)
// Argc 1 = Qual algoritmo será executado (1 a 8)
// Argc 2 = Nome do arquivo de entrada
// Argc 3 = Nome do arquivo de saída
// Não deve ter interação com usuário mas pode mostrar feedbacks
// Tem que mostrar o tempo de execução do algoritmo escolhido
int main(int argc, char* argv[]){
  struct timespec inicio, fim;
  double tempo_gasto;
  char alg_extenso[30];

  if(argc < 4 || argc > 4){
    printf("Uso: %s 1-8 arquivo-in arquivo-out\n", argv[0]);
    return 1;
  }
  FILE *fp = fopen(argv[2], "rb");
  if(fp == NULL){
    printf("Nao foi possivel abrir o arquivo, verifique o nome ou se o arquivo esta na mesma pasta que o executavel\n");
    return 1;
  }
  fseek(fp, 0, SEEK_END);
  long n = ftell(fp)/sizeof(int32_t);
  printf("O arquivo possui %lu elementos\n\n", n);
  fseek(fp, 0, SEEK_SET);

  int32_t *vetor = (int32_t*) malloc(n * sizeof(int32_t));
  printf("Lendo o arquivo...\n");
  fread(vetor, sizeof(int32_t), n, fp);
  printf("Leitura concluida\n\n");
  fclose(fp);
  
  printf("Ordenando...\n");
  clock_gettime(CLOCK_MONOTONIC, &inicio);
  switch (atoi(argv[1])){
  case 1:
    bubbleSortOriginal(vetor, n);
    strcpy(alg_extenso, "Bubblesort Original");
    break;
  case 2:
    bubbleSortMelhorado(vetor, n);
    strcpy(alg_extenso, "Bubblesort Melhorado");
    break;
  case 3:
    insertionSort(vetor, n);
    strcpy(alg_extenso, "Insertion Sort");
    break;
  case 4:
    mergeSort(vetor, 0, n-1);
    strcpy(alg_extenso, "Mergesort");
    break;
  case 5:
    quicksort1(vetor, 0, n-1);
    strcpy(alg_extenso, "Quicksort Ultimo Elemento");
    break;
  case 6:
    quicksort2(vetor, 0, n-1);
    strcpy(alg_extenso, "Quicksort Elemento Aleatorio");
    break;
  case 7:
    quicksort3(vetor, 0, n-1);
    strcpy(alg_extenso, "Quicksort Pivo Mediana");
    break;
  case 8:
    heapsort(vetor, n);
    strcpy(alg_extenso, "Heapsort");
    break;
  default:
    printf("Algoritmo invalido\n");
    free(vetor);
    return 1;
  }
  clock_gettime(CLOCK_MONOTONIC, &fim);
  tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec)/1E9;
  printf("Ordenacao concluida, tempo gasto para ordenacao: %f segundos\n\n", tempo_gasto);

  fp = fopen(argv[3], "w");
  printf("Gravando ordenacao em novo arquivo...\n");
  fwrite(vetor, sizeof(int32_t), n, fp);
  fclose(fp);
  printf("Gravacao concluida\n");

  // Extra
  char tempo_gasto_str[10];
  char n_str[12];
  char nome_arquivo[30];
  sprintf(tempo_gasto_str, "%f", tempo_gasto);
  sprintf(n_str, "%ld", n);
  strcpy(nome_arquivo, "resultados");
  strcat(nome_arquivo, n_str);
  strcat(nome_arquivo, ".txt");
  FILE *arquivo = fopen(nome_arquivo, "a");
  if(arquivo == NULL){
    printf("Falha ao abrir o arquivo para armazenar os resultados...\n");
    return 1;
  }
  fprintf(arquivo, "Algoritmo(%s): %s, nElementos: %s, TempoGasto: %s\n", argv[1], alg_extenso, n_str, tempo_gasto_str);
  fclose(arquivo);
  printf("\nResultado salvo com sucesso\n");
  return 0;
}
