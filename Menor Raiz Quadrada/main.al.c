#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//saídas permitidas e que nao devem ser modificadas
#define SAIDA_1 "\nSAIDA_1 = O maior quadrado menor ou igual a N vale: %d\n"
#define SAIDA_2 "\nSAIDA_2 = N invalido, deve ser um valor positivo!\n"
#define SAIDA_3 "\nSAIDA_3 = Quantidade de argumento invalido!\n"

int main(int argc, char *argv[]) {

    if( argc == 1 ) {
        int n;
        float raizQ;
        int teste = 0;

        printf("Insira um valor inteiro e positivo N: ");
        scanf("%d", &n);

        if(n > 0){
           while(teste == 0){
            raizQ = sqrt(n);
            // fmod calcula resto de divisões de números flutuantes
            if(fmod(raizQ, 2) == 0 || fmod(raizQ, 2) == 1){
                teste = 1;
            } else {
                n--;
                }
            }
            printf(SAIDA_1, n);
        // Se for negativo
        } else {
            printf(SAIDA_2);
        }
        system("pause");

	return 0;
    } else {
	printf(SAIDA_3);

    system("pause");
    	return -1;
    }
}
