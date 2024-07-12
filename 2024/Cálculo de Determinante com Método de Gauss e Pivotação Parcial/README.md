# Determinante de Matriz

Este projeto em C calcula o determinante de uma matriz quadrada de até 10x10, utilizando o método de eliminação Gaussiana para transformar a matriz em uma matriz triangular superior.

## Funcionalidades

- **Geração Aleatória de Matrizes**: Gera uma matriz com valores aleatórios entre -999 e 999.
- **Exibição de Matrizes**: Exibe a matriz no console de uma forma organizada.
- **Troca de Linhas**: Troca as linhas da matriz para garantir que o pivô da coluna tenha o maior valor absoluto.
- **Transformação para Matriz Triangular Superior**: Aplica multiplicadores para zerar os valores abaixo da diagonal principal.
- **Cálculo do Determinante**: Calcula o determinante da matriz a partir da matriz triangular superior.

## Funções Principais

### `double numAleatorio(double min, double max)`

Gera um número aleatório do tipo double entre os valores `min` e `max`.

### `void matrizAleatoria(double matriz[10][10], unsigned ordem)`

Preenche a matriz com valores aleatórios utilizando a função `numAleatorio`.

### `void exibeMatriz(double matriz[10][10], unsigned ordem)`

Exibe a matriz no console de forma formatada.

### `void fixColunaPivo(double matriz[10][10], unsigned ordem, unsigned linhaPivo, unsigned colunaPivo, unsigned *trocas)`

Garante que o valor pivô de uma coluna seja o maior em valor absoluto, trocando as linhas se necessário.

### `void defineMatriz(double matriz[10][10], unsigned *ordem)`

Permite ao usuário definir os valores da matriz manualmente ou gerar aleatoriamente.

### `void operaMatriz(double matriz[10][10], unsigned ordem, double multiplicador, unsigned linhaPivo, unsigned linhaAlt)`

Aplica um multiplicador em uma linha específica para zerar os valores abaixo da diagonal principal.

### `void transformaMatrizSuperior(double matriz[10][10], unsigned ordem, unsigned *trocas)`

Transforma a matriz em uma matriz triangular superior, utilizando as funções auxiliares `fixColunaPivo` e `operaMatriz`.

### `double achaDeterminante(double matriz[10][10], unsigned ordem, unsigned trocas)`

Calcula o determinante da matriz a partir dos valores da diagonal principal da matriz triangular superior.

## Como Usar

1. Compile o código com o comando:

   ```sh
   gcc -o determinante main.c
   ```

2. Execute o programa:

   ```sh
   ./determinante
   ```

3. Siga as instruções no console para definir a matriz e calcular o determinante.

## Exemplo

Ao executar o programa, você poderá escolher entre definir a matriz manualmente ou gerar valores aleatórios. Após a confirmação da matriz, o programa exibirá o processo de transformação da matriz em uma matriz triangular superior e, por fim, calculará e exibirá o determinante.

## Autor

- [Seu Nome](seu-email@example.com)

## Exemplo de Saída

```plaintext
 [ <- ] Insira a ordem da matriz > 3

 [ -> ] A matriz criada foi:
              [1]     [2]     [3]
        [ 1]   42.3   15.2   -9.6
        [ 2]  -18.3    4.2   22.5
        [ 3]   13.7  -45.2    8.1

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= Operacao numero: [1] =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 [ -> ] Multiplicador do indice (2)(1) descoberto: (-0.43)
 [ -> ] Aplicando multiplicador na linha (2)...

 [ -> ] Resultado:
              [1]     [2]     [3]
        [ 1]   42.3   15.2   -9.6
        [ 2]    0.0  -21.3   26.6
        [ 3]   13.7  -45.2    8.1

 [ -> ] Multiplicador do indice (3)(1) descoberto: (-0.32)
 [ -> ] Aplicando multiplicador na linha (3)...

 [ -> ] Resultado:
              [1]     [2]     [3]
        [ 1]   42.3   15.2   -9.6
        [ 2]    0.0  -21.3   26.6
        [ 3]    0.0  -50.0    4.8

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= Operacao numero: [2] =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 [ -> ] Multiplicador do indice (3)(2) descoberto: (2.35)
 [ -> ] Aplicando multiplicador na linha (3)...

 [ -> ] Resultado:
              [1]     [2]     [3]
        [ 1]   42.3   15.2   -9.6
        [ 2]    0.0  -21.3   26.6
        [ 3]    0.0    0.0   67.4

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 [ -> ] Determinante da matriz: (60212.8)

 [ <- ] Deseja testar outra matriz? (S/n) >
```
