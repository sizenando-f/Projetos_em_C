# Análise de DNA com Bactérias e Vírus

Este projeto em C recebe uma lista de bactérias e um vírus, remove a sequência de RNA do vírus das bactérias e encontra a maior sequência de DNA comum entre as bactérias restantes. O programa recebe os dados via linha de comando.

## Funcionalidades

- **Remoção de RNA Viral**: Remove todas as ocorrências da sequência de RNA do vírus das bactérias.
- **Cálculo da Maior Substring Comum**: Encontra a maior sequência de DNA comum entre todas as bactérias após a remoção do RNA viral.
- **Validação de Entrada**: Valida a quantidade de bactérias, o tamanho das sequências de DNA das bactérias e a sequência de RNA do vírus.

## Estrutura do Código

### Definições e Macros

```c
#define LIMITE_BACTERIAS 10
#define COMPR_BACTERIAS 1000
#define COMPR_VIRUS 1000

#define SAIDA_2 "\nSAIDA_2 = Quantidade de bacterias: %d"
#define SAIDA_3 "\nSAIDA_3 = Bacteria: %s"
#define SAIDA_4 "\nSAIDA_4 = Virus: %s"
#define SAIDA_5 "\nSAIDA_5 = Bacteria infectada: %s"
#define SAIDA_6 "\nSAIDA_6 = DNA resultante: %s"
```

### Funções

#### `dnaResultante`

Descobre a maior substring comum entre todas as bactérias.

```c
void dnaResultante(char bacterias[][COMPR_BACTERIAS], int nBacterias, char resultado[]) {
    size_t tamBac = strlen(bacterias[0]);
    char bacteriaPrinc[tamBac];
    char resultadoFinal[COMPR_BACTERIAS] = "";

    strcpy(bacteriaPrinc, bacterias[0]);

    for(size_t tam = tamBac; tam >= 1; tam--){
        for(size_t i = 0; i <= tamBac - tam; i++){
            char temp[tam + 1];
            strncpy(temp, bacteriaPrinc + i, tam);
            temp[tam] = '\0';

            int cont = 0;
            for(int k = 0; k < nBacterias; k++){
                if(strstr(bacterias[k], temp)){
                    cont++;
                }
            }

            if(cont == nBacterias && strlen(temp) > strlen(resultadoFinal)){
                strcpy(resultadoFinal, temp);
            }
        }
    }
    strcpy(resultado, resultadoFinal);
}
```

#### `destroiDNA`

Remove todas as ocorrências do RNA viral das bactérias.

```c
void destroiDNA(char bacterias[][COMPR_BACTERIAS], int nBacterias, char virus[COMPR_VIRUS]) {
    for(int i = 0; i < nBacterias; i++){
        char *resultado = strstr(bacterias[i], virus);
        while(resultado){
            size_t len = strlen(virus);
            memmove(resultado, resultado + len, strlen(resultado + len) + 1);
            resultado = strstr(bacterias[i], virus);
        }
    }
}
```

### Função Principal

Recebe os argumentos da linha de comando, valida os dados e executa as funções de remoção de RNA e cálculo da maior substring comum.

```c
int main(int argc, char *argv[]) {
    int nBacterias;
    char BACTERIAS[LIMITE_BACTERIAS][COMPR_BACTERIAS];
    char VIRUS[COMPR_VIRUS];
    char dnaResult[COMPR_BACTERIAS];
    int validaBacteria = 0;

    nBacterias = atoi(argv[1]);
    if(nBacterias >= 1 && nBacterias <= 10){
        if((argc - 3) == nBacterias){
            for(int j = 2; j < argc - 1; j++){
                if((strlen(argv[j]) >= 1) && (strlen(argv[j]) <= 1000)){
                    validaBacteria++;
                }
            }
            if(validaBacteria == nBacterias){
                if((strlen(argv[argc-1]) >= 1) && (strlen(argv[argc-1]) <= 1000)){
                    printf(SAIDA_2, nBacterias);
                    for(int i = 2; i < argc - 1; i++) {
                        strcpy(BACTERIAS[i-2], argv[i]);
                        printf(SAIDA_3, BACTERIAS[i-2]);
                    }
                    strcpy(VIRUS, argv[argc-1]);
                    printf(SAIDA_4, VIRUS);

                    destroiDNA(BACTERIAS, nBacterias, VIRUS);
                    for(int i = 0; i < nBacterias; i++){
                        printf(SAIDA_5, BACTERIAS[i]);
                    }

                    dnaResultante(BACTERIAS, nBacterias, dnaResult);
                    if(!strcmp(dnaResult, "")){
                        printf(SAIDA_6, "DNA COMUM INEXISTENTE");
                    } else {
                        printf(SAIDA_6, dnaResult);
                    }
                } else {
                    printf("O RNA DO VIRUS POSSUI O TAMANHO INCORRETO\n");
                }
            } else {
                printf("UMA DAS BACTERIAS NAO POSSUI A QUANTIDADE CORRETA DE PROTEINA\n");
            }
        } else {
            printf("QUANTIDADE INCORRETA DA BACTERIAS INFORMADA\n");
        }
    } else {
        printf("NUMERO INVALIDO DE BACTERIAS, PRECISA SER >= 1 E <= 10\n");
    }

    return 0;
}
```

## Como Usar

### Compilação

Para compilar o código, use o seguinte comando:

```sh
gcc -o dna_analysis main.al.c
```

### Execução

Para executar o programa, passe a quantidade de bactérias, as sequências de DNA das bactérias e a sequência de RNA do vírus como argumentos:

```sh
./dna_analysis <num_bacterias> <bacteria_1> <bacteria_2> ... <virus>
```

### Exemplo

```sh
./dna_analysis 3 GCTTTCGACGAT GATCGAGCTTCGAA GGTCTAGCTAAT TCGA
```

Saída esperada:

```
SAIDA_2 = Quantidade de bacterias: 3
SAIDA_3 = Bacteria: GCTTTCGACGAT
SAIDA_3 = Bacteria: GATCGAGCTTCGAA
SAIDA_3 = Bacteria: GGTCTAGCTAAT
SAIDA_4 = Virus: TCGA
SAIDA_5 = Bacteria infectada: GCTT
SAIDA_5 = Bacteria infectada: GAGCTA
SAIDA_5 = Bacteria infectada: GGTCTAGCTAAT
SAIDA_6 = DNA resultante: GCT
```

### Outro Exemplo

```sh
./dna_analysis 5 AGCT TGC CGCAA TGTC ATGTTC T
```

Saída esperada:

```
SAIDA_2 = Quantidade de bacterias: 5
SAIDA_3 = Bacteria: AGCT
SAIDA_3 = Bacteria: TGC
SAIDA_3 = Bacteria: CGCAA
SAIDA_3 = Bacteria: TGTC
SAIDA_3 = Bacteria: ATGTTC
SAIDA_4 = Virus: T
SAIDA_5 = Bacteria infectada: AGC
SAIDA_5 = Bacteria infectada: GC
SAIDA_5 = Bacteria infectada: CGCAA
SAIDA_5 = Bacteria infectada: GC
SAIDA_5 = Bacteria infectada: AGC
SAIDA_6 = DNA resultante: GC
```

---

Espero que este README detalhado ajude você a entender e usar o programa de análise de DNA com bactérias e vírus. Se você tiver alguma dúvida ou encontrar algum problema, sinta-se à vontade para entrar em contato.
