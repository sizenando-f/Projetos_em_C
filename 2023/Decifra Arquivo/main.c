#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

//  Converte binário em caractere
void converte_texto_n(char texto_convertido[99000]){
    char texto[99000];

    FILE *fp = fopen("readme.code.txt", "r");

    if (fp == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO!\n");
        exit(100);
    }

    if (fgets(texto, sizeof(texto), fp) != NULL) {
        texto_convertido[0] = '\0'; // Inicializa vetor
        char *numero_char = strtok(texto, "[]");    // Pega bloco do código
        while (numero_char != NULL) {
            int numero_int = atoi(numero_char); // Converto para inteiro
            char letra = (char)numero_int;  // Converte de inteiro para caractere da tabela ASCII
            strncat(texto_convertido, &letra, 1);   // Concatena no vetor
            numero_char = strtok(NULL, "[]");   // Passa para próximo bloco
        }
    }
    fclose(fp);
}

// Cria arquivo "readme.decrifra.txt" com texto decifrado e prepara texto para linhas
void formata_texto(char texto[99000], char texto_formatado[726][56], char texto_f[712][61]) {
    FILE * fp = fopen("readme.decifra.txt", "w+");
    if(fp == NULL){
        printf("ERRO AO ABRIR O ARQUIVO\n");
        exit(100);
    }

    for(int i = 0; i < 711; i++){
        fprintf(fp, "%s\n", texto_f[i]);    // Insere texto convertido no arquivo
    }

    fclose(fp);

    int linha = 0, coluna = 0, i = 0, cont = 0;
    char esc;
    
    // Prepara texto para inserção de linhas
    while (texto[i] != '\0') {
        if (texto[i] == '\n' || cont == 56) {   // Se houver quebra de linhas ou atingido o tamanho definido no trabalho
            texto_formatado[linha][coluna] = '\0';  // Inicializa
            linha++;     // Pula linha
            coluna = 0;     // Vai para primeira coluna
            cont = 0;   // Reinicia contador de caracteres
        } else {
            texto_formatado[linha][coluna] = texto[i];  // Insere caractere na linha
            coluna++;   //  Passa para próxima coluna
        }
        i++;    // Passa para próximo caractere
        cont++; // Aumenta contador de caractere
    }
    
    // Exibe ou não texto convertido
    do{
        system("cls");
        printf("\nDecodificacao realizada com sucesso\n");
        printf("\nDeseja exibi-lo? (s/n)-> ");
        getchar();
        scanf("%c", &esc);
        esc = toupper(esc);
        switch (esc){
            case 'S':
                printf("%s\n", texto); break;
            case 'N':
                printf("\nExibicao recusada\n\n"); break;
            default:
                printf("\nEntrada invalida\n\n");
                system("pause");
                break;
        }
    }while(esc != 'S' && esc != 'N');
    
}

// Cria o arquivo "readme.nlines.txt"
void cria_linhas_com_numero(char texto[726][56]){
    char esc;
    FILE * fp = fopen("readme.nlines.txt", "w+");
    if(fp == NULL){
        printf("ERRO AO ABRIR O ARQUIVO\n\n");
        exit(100);
    }

    for(int i = 0; i < 726; i++){
        fprintf(fp, "[linha: %d] %s\n", i+1, texto[i]); // Insere linhas com números
    }

    fclose(fp);
    
    // Exibe ou não texto com números
    do{
        system("cls");
        printf("\nOperacao realizada com sucesso\n");
        printf("\nDeseja exibi-lo? (s/n) ->");
        getchar();
        scanf("%c", &esc);
        esc = toupper(esc);
        switch (esc){
            case 'S':
                for(int i = 0; i < 726; i++){
                    printf("[linha: %d] %s\n", i+1, texto[i]);
                }
                break;
            case 'N':
                printf("\nExibicao recusada\n\n"); break;
            default:
                printf("\nEntrada invalida\n\n");
                system("pause");
                break;
        }
    }while(esc != 'S' && esc != 'N');
}

// Mostra qual maior linha e quantos caracteres ela possui
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
    printf("\nA linha [%d] e a maior com (%d) caracteres\n\n", maior_l, maior);
}

// Exibe palavra recebida do usuário
void encontra_palavra(char palavra[20], char texto_f[711][61]){
    system("cls");
    printf("\nAs linhas em que a palavra \"%s\" ocorre: \n", palavra);
    int cont = 0;
    for(int i = 0; i < 711; i++){
        if(strstr(texto_f[i], palavra)){    // Se palavra existir na string atual
            printf("\n[linha: %d] %s", i+1, texto_f[i]);    // Mostra frase com a palavra
            cont++; // Incrementa no contador de ocorrências
        }
    }
    if(cont){
        printf("\n\ne tem %d ocorrencias.\n\n", cont);
    } else {
        printf("\nnao existe ocorrencia da palavra.\n\n");
    }
}

// Exibe quantidade de linhas no texto
void conta_linhas(char * texto_convertido){
    int i = 0, cont = 0;
    while(texto_convertido[i] != '\0'){
        if(texto_convertido[i] == '\n') cont++;
        i++;
    }
    printf("\nO arquivo \"readme.decifra.txt\" tem (%d) linhas\n\n", cont+1);
}

// Insere texto com o formato original
void converte_texto_f(char texto_n[99000], char texto_f[711][61]){
    int linha = 0, coluna = 0, i = 0, cont = 0; 
    while (texto_n[i] != '\0') {
        if (texto_n[i] == '\n' || cont == 61) { // Se houver quebra de linha ou se for tiver a quantidade de caracteres máximo
            texto_f[linha][coluna] = '\0';  // Inicializa
            linha++;    // Passa para próxima linha
            coluna = 0;    // Inicializa coluna
            cont = 0;   // Inciializa contador de caracteres
        } else { 
            texto_f[linha][coluna] = texto_n[i];    // Inserece caractere na linha
            coluna++;   // Passa para próxima coluna
        }
        i++;    // Passa para próximo caractere
        cont++; // Incrementa contador de caracteres
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
                formata_texto(texto_convertido_n, texto_formatado, texto_convertido_f);
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
                printf("\nQual palavra pesquisar: ");
                scanf("%s", palavra);
                encontra_palavra(palavra, texto_convertido_f);
                system("pause");
                break;
            case 0:
                printf("\nPrograma encerrado\n\n");
                break;
            default:
                printf("\nEntrada invalida\n\n");
                system("pause");
                break;
        }
    }while(esc != 0);
    return 0;
}
