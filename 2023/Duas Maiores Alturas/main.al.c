#include <stdio.h>
#include <stdlib.h>

#define SAIDA_PRIMEIRA_MAIOR_ALTURA "\nSAIDA_PRIMEIRA_MAIOR_ALTURA = %.2f"
#define SAIDA_SEGUNDA_MAIOR_ALTURA "\nSAIDA_SEGUNDA_MAIOR_ALTURA = %.2f"
#define SAIDA_ALTURA_FORA_LIMITE "\nSAIDA_ALTURA_FORA_LIMITE = %.2f"

int main () {

    float altura = 1;
    float maiorAltura1 = 0, maiorAltura2 = 0;
    int tamanho = 0;
    int primeiraRepeticao = 1;
    float aux = 0;

    // Vetor dinamico
    float *vetor = (float*)malloc(tamanho * sizeof(float));

    while(altura != 0){
        printf("Insira a altura: ");
        scanf("%f", &altura);

        // Altura fora de parametro resulta em adição dessa altura no
        // vetor e no aumenro do tamanho do vetor para receber mais alturas
        if(altura < 1.50 || altura > 2.35){
            vetor[tamanho] = altura;
            if(altura != 0){
                tamanho++;
            }
        // Se for a primeira repetição, a primeira altura inserida será sempre a maior altura
        } else if(primeiraRepeticao == 1){
                maiorAltura1 = altura;
                primeiraRepeticao = 0;
            } else {
                // Se a maior altura for substituida, de forma lógica, a primeira
                // maior altura se torna a segunda maior altura
                if(altura > maiorAltura1){
                    aux = maiorAltura1;
                    maiorAltura1 = altura;
                    maiorAltura2 = aux;
                } else if(altura > maiorAltura2){
                    maiorAltura2 = altura;
                }
            }

        }

    // Printa todos os valores fora do limite estabelecido
    for(int i = 0; i < tamanho; i++){
        printf(SAIDA_ALTURA_FORA_LIMITE, vetor[i]);
    }
    printf(SAIDA_PRIMEIRA_MAIOR_ALTURA, maiorAltura1);
    printf(SAIDA_SEGUNDA_MAIOR_ALTURA, maiorAltura2);
    free(vetor);
    system("pause");
    return(0);
}
