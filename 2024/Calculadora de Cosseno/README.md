# Cálculo de Cosseno em C

Este programa em C calcula o cosseno de um ângulo, tanto em graus quanto em radianos, usando a série de Taylor. Ele permite ao usuário escolher o tipo de entrada (graus ou radianos), o valor do ângulo e o número de termos na série de Taylor para a aproximação.

## Funcionalidades

- **Calcula o Cosseno**: O programa calcula o cosseno de um ângulo dado em graus ou radianos.
- **Escolha do Usuário**: Permite ao usuário escolher entre inserir o ângulo em graus ou radianos.
- **Limitação de Número de Termos**: Restringe o número máximo de termos na série de Taylor para evitar computação excessiva.
- **Tratamento de Erros**: Detecta e corrige erros como números negativos e excesso de termos na série.
- **Medição de Tempo**: Registra o tempo gasto para calcular o cosseno.

## Pré-requisitos

- **Windows**: O programa utiliza funções específicas do Windows para limpar a tela (`system("cls")`) e pausar (`system("pause")`).

## Compilação e Execução

Para compilar o programa, utilize um compilador C compatível, como o GCC. Por exemplo:

```bash
gcc -o calculo_cosseno cos.c -lm
```

Para executar o programa:

```bash
./calculo_cosseno
```

## Como Usar

1. O programa exibirá um menu com as opções:
   - Grau: Insira um ângulo em graus.
   - Radiano: Insira um ângulo em radianos.
   - Sair: Encerra o programa.
2. Insira o valor do ângulo conforme solicitado.
3. Insira o número de termos na série de Taylor para a aproximação.
4. O programa calculará o cosseno do ângulo e exibirá o resultado.
5. O tempo gasto na operação será mostrado após o cálculo.

## Exemplos de Uso

```bash
Escolha o tipo de entrada > 1
[ <- ] Insira o valor em grau (ex.: 90) > 45
[ <- ] Insira o valor de N (max.: 100) > 5

[ -> ] Cosseno de 45.00 graus: 0.707106781186548
[ -> ] Tempo gasto: 0.000002

...

Escolha o tipo de entrada > 2
[ <- ] Insira o valor em radiano (ex.: 1.570796) > 1.570796
[ <- ] Insira o valor de N (max.: 100) > 3

[ -> ] Cosseno de 1.570796 radianos: 0.000000000000000
[ -> ] Tempo gasto: 0.000001

...

Escolha o tipo de entrada > 3
[ -> ] Programa encerrado

```
