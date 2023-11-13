#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void converte_texto_n(char texto_convertido[99000]){
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
    int linha = 0, coluna = 0, i = 0, cont = 0;
    char esc;
    
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
    system("cls");
    printf("Decodificacao realizada com sucesso\n");
    printf("Deseja exibi-lo? (s/n)-> ");
    getchar();
    scanf("%c", &esc);
    esc = toupper(esc);
    do{
        switch (esc){
            case 'S':
                printf("%s\n", texto); break;
            case 'N':
                printf("Exibicao recusada\n"); break;
            default:
                printf("Entrada invalida\n");
                break;
        }
    }while(esc != 'S' && esc != 'N');
    
}

void cria_linhas_com_numero(char texto[726][56]){
    char esc;
    FILE * fp = fopen("readme.nlines.txt", "w+");
    if(fp == NULL){
        printf("ERRO AO ABRIR O ARQUIVO\n");
        exit(100);
    }

    for(int i = 0; i < 726; i++){
        fprintf(fp, "[linha: %d] %s\n", i+1, texto[i]);
    }

    fclose(fp);
    system("cls");
    printf("Operacao realizada com sucesso\n");
    printf("Deseja exibi-lo? (s/n) ->");
    getchar();
    scanf("%c", &esc);
    esc = toupper(esc);
    do{
        switch (esc){
            case 'S':
                for(int i = 0; i < 726; i++){
                    printf("[linha: %d] %s\n", i+1, texto[i]);
                }
                break;
            case 'N':
                printf("Exibicao recusada\n"); break;
            default:
                printf("Entrada invalida\n");
                break;
        }
    }while(esc != 'S' && esc != 'N');
}

void descobre_maior_linha(char * texto){
    int i = 0, linha = 1, cont_char = 0, maior = 0, maior_l = 0;
    while(texto[i] != '\0'){
        cont_char++;
        if(texto[i] == '\n'){
            if(cont_char > maior){
                maior = cont_char;
                maior_l = linha;
            }
            cont_char = 0;
            linha++;
        }
        i++;
    }
    printf("A linha [%d] e a maior com (%d) caracteres\n", maior_l, maior);
}

void encontra_palavra(char * palavra, char texto_f[711][61]){
    
}

void conta_linhas(char * texto_convertido){
    int i = 0, cont = 0;
    while(texto_convertido[i] != '\0'){
        if(texto_convertido[i] == '\n') cont++;
        i++;
    }
    printf("O arquivo \"readme.code.txt\" tem (%d) linhas\n", cont+1);
}

void converte_texto_f(char texto_n[99000], char texto_f[711][61]){
    int linha = 0, coluna = 0, i = 0, cont = 0; 
    while (texto_n[i] != '\0') {
        if (texto_n[i] == '\n' || cont == 61) {
            texto_f[linha][coluna] = '\0';
            linha++;  
            coluna = 0; 
            cont = 0;
        } else {
            texto_f[linha][coluna] = texto_n[i];
            coluna++;
        }
        i++;
        cont++;
    }
}

int main() {
    char texto_convertido_n[99000];
    char texto_convertido_f[712][61];
    char texto_formatado[726][56];
    converte_texto_n(texto_convertido_n);
    converte_texto_f(texto_convertido_n, texto_convertido_f);
    int esc = 0, check = 0;
    char palavra[20];

    do{
        system("cls");
        printf("1-Decifra arquivo\n");
        printf("2-Grava numero linha em arquivo\n");
        printf("3-Quantas linhas o arquivo tem\n");
        printf("4-Qual a maior linha\n");
        printf("5-Pesquisar uma palavra\n");
        printf("0-Encerrar\n");
        printf("\nQual opcao: ");
        scanf("%d", &esc);
        switch (esc){
            case 1:
                formata_texto(texto_convertido_n, texto_formatado);
                check = 1;
                system("pause");
                break;
            case 2:
                if(check){
                    cria_linhas_com_numero(texto_formatado);
                    check++;
                } else {
                    printf("Primeiro selecione a opcao de decifrar o arquivo e entao tente novamente\n");
                }
                system("pause");
                break;
            case 3:
                conta_linhas(texto_convertido_n);    
                system("pause");
                break;
            case 4:
                descobre_maior_linha(texto_convertido_n);
                system("pause");
                break;
            case 5:
                printf("Qual palavra pesquisar: ");
                scanf("%s", palavra);
                encontra_palavra(palavra, texto_convertido_f);
                system("pause");
                break;
            default:
                break;
        }
    }while(esc != 0);



    return 0;
}
