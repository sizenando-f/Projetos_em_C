# Coleta e Análise de Alturas

Este programa em C coleta alturas inseridas pelo usuário, determina a maior e a segunda maior altura dentro de um intervalo especificado, e armazena alturas fora desse intervalo em um vetor dinâmico.

## Funcionalidades

- **Coleta de Alturas**: O programa permite ao usuário inserir várias alturas até que uma altura de 0 seja inserida.
- **Determinação de Maiores Alturas**: O programa identifica a maior e a segunda maior altura inserida que esteja dentro do intervalo de 1.50 a 2.35 metros.
- **Armazenamento de Alturas Fora do Limite**: Alturas fora do intervalo especificado são armazenadas em um vetor dinâmico.
- **Exibição de Resultados**: O programa exibe as alturas fora do limite, a maior altura e a segunda maior altura.

## Estrutura do Código

### Constantes

- `SAIDA_PRIMEIRA_MAIOR_ALTURA`: Mensagem para a maior altura.
- `SAIDA_SEGUNDA_MAIOR_ALTURA`: Mensagem para a segunda maior altura.
- `SAIDA_ALTURA_FORA_LIMITE`: Mensagem para alturas fora do limite.

### Função Principal

- `int main()`: A função principal coleta as alturas, processa os dados para determinar as maiores alturas e armazena as alturas fora do limite em um vetor dinâmico. No final, exibe os resultados.

  ```c
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
          // vetor e no aumento do tamanho do vetor para receber mais alturas
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
                  // Se a maior altura for substituída, de forma lógica, a primeira
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
  ```

## Uso

1. Compile o programa utilizando um compilador C, por exemplo:

   ```sh
   gcc -o alturas main.c
   ```

2. Execute o programa:

   ```sh
   ./alturas
   ```

3. Insira alturas conforme solicitado. O programa continuará solicitando alturas até que o valor 0 seja inserido.

## Exemplo de Execução

```
Insira a altura: 1.60
Insira a altura: 1.55
Insira a altura: 2.00
Insira a altura: 1.45
Insira a altura: 2.40
Insira a altura: 0
SAIDA_ALTURA_FORA_LIMITE = 1.45
SAIDA_ALTURA_FORA_LIMITE = 2.40
SAIDA_PRIMEIRA_MAIOR_ALTURA = 2.00
SAIDA_SEGUNDA_MAIOR_ALTURA = 1.60
```

No exemplo acima, as alturas 1.45 e 2.40 estão fora do intervalo e são armazenadas no vetor. As maiores alturas dentro do intervalo são 2.00 e 1.60.
