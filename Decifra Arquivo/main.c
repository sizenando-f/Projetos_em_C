#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void converte_texto(char texto_convertido[99000]){
    char texto[99000];

    FILE *fp = fopen("readme.code.txt", "r");

    if (fp == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO!\n");
        exit(100);
    }

    if (fgets(texto, sizeof(texto), fp) != NULL) {
        // Inicializa a string de texto_convertido
        texto_convertido[0] = '\0';

        // Usa strtok para separar os números entre colchetes
        char *numero_char = strtok(texto, "[]");
        while (numero_char != NULL) {
            int numero_int = atoi(numero_char);
            char letra = (char)numero_int;
            // Concatena o caractere à string texto_convertido
            strncat(texto_convertido, &letra, 1);
            numero_char = strtok(NULL, "[]");
        }
    }
    fclose(fp);
}

void formata_texto(char texto[99000], char texto_formatado[726][56]) {
    int linha = 0;
    int coluna = 0;
    int i = 0, cont = 0;
    
    while (texto[i] != '\0') {
        if (texto[i] == '\n' || cont == 56) {
            texto_formatado[linha][coluna] = '\0';
            linha++;  
            coluna = 0; 
            cont = 0;
        } else {
            texto_formatado[linha][coluna] = texto[i];
            coluna++;
        }
        i++;
        cont++;
    }
}

void exibe_linhas_com_numero(char texto[726][56]){
    for(int i = 0; i < 726; i++){
        printf("[linha: %d] %s\n", i+1, texto[i]);
    }
}

int main() {
    char texto_convertido[99000];
    converte_texto(texto_convertido);

    char texto_formatado[726][56];
    formata_texto(texto_convertido, texto_formatado);

    exibe_linhas_com_numero(texto_formatado);

    return 0;
}
