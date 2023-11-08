#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

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

void descobre_maior_linha(){
    int maior = 0, maior_l = 0, cont = 0; 
    char linha[56];
    if(access("readme.nlines.txt", F_OK) != -1){
        FILE * fp = fopen("readme.nlines.txt", "r");
        if(fp == NULL){
            printf("Erro ao abrir o arquivo\n"); exit(100);
        }

        while(fgets(linha, sizeof(linha), fp)){
            cont++;
            int test = strlen(linha);
            if(test > maior){
                maior = test;
                maior_l = cont;
            }
        }

        fclose(fp);
        printf("A linha [%d] e a maior com (%d) caracteres\n", maior_l, maior);
    } else {
        printf("Primeiro grave o numero da linha no arquivo (opcao 2)\n");
    }
}

void encontra_palavra(char * palavra){
    if(access("readme.nlines.txt", F_OK) != -1){
        FILE * fp = fopen("readme.nlines.txt", "r");
        if(fp == NULL){
            printf("Erro ao abrir o arquivo\n"); exit(100);
        }
        int cont = 0;
        char linha[56];
        printf("\nAs linhas em que a palavra \"%s\" ocorre:\n\n", palavra);
        while(fgets(linha, sizeof(linha), fp)){
            char * resultado = strstr(linha, palavra);
            if(resultado != NULL){
                printf("%s", linha);
                cont++;
            }
        }
        printf("\n\ne tem %d ocorrencias.", cont);

    } else {
        printf("Primeiro grave o numero da linha no arquivo (opcao 2)\n");
    }
}

int main() {
    char texto_convertido[99000];
    char texto_formatado[726][56];
    converte_texto(texto_convertido);
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
                formata_texto(texto_convertido, texto_formatado);
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
                    if(access("readme.nlines.txt", F_OK) != -1){
                        printf("O arquivo \"readme.nlines.txt\" tem (726) linhas.\n");
                    } else {
                        printf("Grave o numero de linhas no arquivo ( opcao 2) e entao tente novamente\n");
                    }
                    
                system("pause");
                break;
            case 4:
                descobre_maior_linha(texto_formatado);
                system("pause");
                break;
            case 5:
                printf("Qual palavra pesquisar: ");
                scanf("%s", palavra);
                encontra_palavra(palavra);
                system("pause");
                break;
            default:
                break;
        }
    }while(esc != 0);



    return 0;
}