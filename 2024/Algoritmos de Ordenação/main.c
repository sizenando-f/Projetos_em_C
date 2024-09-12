#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>

void geraNaleatorio(int *vetor, int n){
  for(int i = 0; i < n; i++){
    vetor[i] = rand()%400;
  }
}

// Opção 1
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

// Opção 2
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

// Opção 3
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

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l..r
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

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// l is for left index and r is right index of the
// sub-array of arr to be sorted
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int mediana(int vet[], int ini, int fim){
    int meio = ini + (fim - ini)/2;
    if(vet[meio] > vet[ini] && vet[meio] < vet[fim]) return meio;
    if(vet[ini] > vet[meio] && vet[ini] < vet[fim]) return ini;
    return fim;
}

int main(){
  // int vetor[] = {1,5,6,2,4,8,3,4,98,0, 87, 23, 54, 76, 88, 22};
  int n = 50;
  int *vetor = (int*) malloc(n * sizeof(int));

  srand(time(NULL));
  geraNaleatorio(vetor, n);

  printf("Antes:\n");
  for(int i = 0; i < n; i++){
    printf("%d ", vetor[i]);
  }

  // bubbleSortOriginal(vetor, n);
  // bubbleSortMelhorado(vetor, n);
  // insertionSort(vetor, n);
  mergeSort(vetor, 0, n-1);

  printf("\nDepois:\n");
  for(int i = 0; i < n; i++){
    printf("%d\n", vetor[i]);
  }
  free(vetor);
  return 0;
}