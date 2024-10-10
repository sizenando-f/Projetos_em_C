#include <stdio.h>
#include <string.h>

#define MAX_ALGORITMOS 8
#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    double tempos[3];
    double media;
} Algoritmo;

// Função para calcular a média de três tempos
double calcular_media(double tempos[], int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += tempos[i];
    }
    return soma / n;
}

int main() {
    FILE *entrada, *saida;
    Algoritmo algoritmos[MAX_ALGORITMOS];
    char nome_algoritmo[MAX_NOME];
    double tempo;
    int id, nElementos;
    int contagem[MAX_ALGORITMOS] = {0};  // Contagem de tempos lidos por algoritmo

    // Abre o arquivo de entrada
    entrada = fopen("resultados.txt", "r");
    if (entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada!\n");
        return 1;
    }

    // Inicializa os algoritmos
    strcpy(algoritmos[0].nome, "Bubblesort Original");
    strcpy(algoritmos[1].nome, "Bubblesort Melhorado");
    strcpy(algoritmos[2].nome, "Insertion Sort");
    strcpy(algoritmos[3].nome, "Mergesort");
    strcpy(algoritmos[4].nome, "Quicksort Ultimo Elemento");
    strcpy(algoritmos[5].nome, "Quicksort Elemento Aleatorio");
    strcpy(algoritmos[6].nome, "Quicksort Pivo Mediana");
    strcpy(algoritmos[7].nome, "Heapsort");

    // Inicializa tempos para evitar problemas
    for (int i = 0; i < MAX_ALGORITMOS; i++) {
        for (int j = 0; j < 3; j++) {
            algoritmos[i].tempos[j] = 0.0;
        }
    }

    // Lê os dados do arquivo
    while (fscanf(entrada, " Algoritmo(%d): %[^,], nElementos: (%d), TempoGasto: (%lf)", &id, nome_algoritmo, &nElementos, &tempo) != EOF) {
        int indice = id - 1;  // Ajusta o índice para começar de 0
        if (contagem[indice] < 3) {  // Verifica se já não há mais que 3 tempos
            algoritmos[indice].tempos[contagem[indice]] = tempo;
            contagem[indice]++;
        }
    }

    fclose(entrada);

    // Calcula a média de tempo para cada algoritmo
    for (int i = 0; i < MAX_ALGORITMOS; i++) {
        if (contagem[i] == 3) {
            algoritmos[i].media = calcular_media(algoritmos[i].tempos, 3);
        } else {
            printf("Algoritmo %s não possui 3 tempos!\n", algoritmos[i].nome);
            algoritmos[i].media = 0.0;  // Atribui 0.0 se não houver 3 tempos
        }
    }

    // Abre o arquivo de saída
    saida = fopen("medias.txt", "w");
    if (saida == NULL) {
        printf("Erro ao abrir o arquivo de saída!\n");
        return 1;
    }

    // Escreve as médias no arquivo de saída
    for (int i = 0; i < MAX_ALGORITMOS; i++) {
        fprintf(saida, "Algoritmo: %s, Media Tempo: %.9lf\n", algoritmos[i].nome, algoritmos[i].media);
    }

    fclose(saida);

    printf("Médias calculadas e gravadas em 'medias.txt'.\n");

    return 0;
}
