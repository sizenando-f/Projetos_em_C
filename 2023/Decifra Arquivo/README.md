# Decifra Arquivo

Este código em C realiza várias operações com um arquivo de texto cifrado, incluindo decifração, contagem de linhas, formatação, e busca por palavras específicas. Vamos dar uma olhada no funcionamento e propósito de cada parte do código.

### Funções e suas funcionalidades

1. **`converte_texto_n`**: Lê um arquivo `readme.code.txt`, que contém números inteiros representando caracteres ASCII, e converte esses números em uma string de texto decifrado.

   ```c
   void converte_texto_n(char texto_convertido[99000]) {
       // ...
   }
   ```

2. **`formata_texto`**: Formata o texto decifrado em linhas com um comprimento máximo de 56 caracteres e grava esse texto em um arquivo `readme.decifra.txt`. Também permite ao usuário escolher exibir o texto formatado.

   ```c
   void formata_texto(char texto[99000], char texto_formatado[726][56], char texto_f[712][61]) {
       // ...
   }
   ```

3. **`cria_linhas_com_numero`**: Gera um arquivo `readme.nlines.txt` onde cada linha do texto formatado é precedida por seu número de linha.

   ```c
   void cria_linhas_com_numero(char texto[726][56]) {
       // ...
   }
   ```

4. **`descobre_maior_linha`**: Identifica a linha mais longa do texto e exibe o número da linha e a quantidade de caracteres.

   ```c
   void descobre_maior_linha(char *texto) {
       // ...
   }
   ```

5. **`encontra_palavra`**: Pesquisa uma palavra específica no texto e exibe as linhas em que a palavra ocorre, juntamente com o número de ocorrências.

   ```c
   void encontra_palavra(char palavra[20], char texto_f[711][61]) {
       // ...
   }
   ```

6. **`conta_linhas`**: Conta e exibe o número total de linhas no texto decifrado.

   ```c
   void conta_linhas(char *texto_convertido) {
       // ...
   }
   ```

7. **`converte_texto_f`**: Formata o texto decifrado em linhas com um comprimento máximo de 61 caracteres.
   ```c
   void converte_texto_f(char texto_n[99000], char texto_f[711][61]) {
       // ...
   }
   ```

### Função Principal

A função `main` gerencia a interação com o usuário através de um menu que permite escolher entre as várias operações suportadas pelo programa.

```c
int main() {
    char texto_convertido_n[99000];
    char texto_convertido_f[712][61];
    char texto_formatado[726][56];

    converte_texto_n(texto_convertido_n);
    converte_texto_f(texto_convertido_n, texto_convertido_f);

    int esc = 0, check = 0;
    char palavra[20];

    do {
        system("cls");
        printf("1-Decifra arquivo\n");
        printf("2-Grava numero linha em arquivo\n");
        printf("3-Quantas linhas o arquivo tem\n");
        printf("4-Qual a maior linha\n");
        printf("5-Pesquisar uma palavra\n");
        printf("0-Encerrar\n");
        printf("\nQual opcao: ");
        scanf("%d", &esc);
        switch (esc) {
            case 1:
                formata_texto(texto_convertido_n, texto_formatado, texto_convertido_f);
                check = 1;
                system("pause");
                break;
            case 2:
                if (check) {
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
    } while (esc != 0);
    return 0;
}
```

### Pontos de Atenção

1. **Manipulação de Strings**: A função `strncat` usada em `converte_texto_n` é segura, mas deve-se sempre garantir que o tamanho dos buffers seja suficiente para evitar sobrecarga de buffer.
2. **Uso de `system("cls")` e `system("pause")`**: Essas funções são específicas para Windows. Em sistemas Unix-like, o comando `system("clear")` poderia ser usado em vez de `cls`.
3. **Verificação de Entrada do Usuário**: Ao usar `scanf` para entrada do usuário, é importante validar a entrada para evitar comportamentos inesperados.
4. **Gerenciamento de Memória**: Embora o código use arrays estáticos, o uso de alocação dinâmica de memória poderia melhorar a flexibilidade do programa.

Esse programa cobre um conjunto útil de funcionalidades para manipulação de arquivos de texto e decifração, o que pode ser particularmente útil em contextos onde dados precisam ser extraídos ou processados de forma automatizada.
