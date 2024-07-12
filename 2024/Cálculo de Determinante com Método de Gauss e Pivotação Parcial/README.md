# Gerador de Matriz Aleatória e Cálculo do Determinante

Este programa em C gera uma matriz quadrada com valores aleatórios ou definidos pelo usuário e calcula seu determinante. A matriz pode ter dimensão de até 10x10.

## Funcionalidades

1. **Geração de Números Aleatórios:**

   - A função `numAleatorio(double min, double max)` gera números aleatórios do tipo double entre um valor mínimo e máximo especificado.

2. **Inicialização da Matriz:**

   - A função `defineMatriz(double matriz[10][10], unsigned *ordem)` permite ao usuário definir a matriz manualmente ou gerar valores aleatórios automaticamente.

3. **Exibição da Matriz:**

   - A função `exibeMatriz(double matriz[10][10], unsigned ordem)` exibe a matriz no console.

4. **Transformação em Matriz Superior:**

   - A função `transformaMatrizSuperior(double matriz[10][10], unsigned ordem, unsigned *trocas)` transforma a matriz em uma matriz triangular superior usando operações de troca de linhas e multiplicação.

5. **Cálculo do Determinante:**
   - A função `achaDeterminante(double matriz[10][10], unsigned ordem, unsigned trocas)` calcula o determinante da matriz triangular superior resultante.

## Como Usar

### Compilação e Execução

1. **Compilação:**

   - Compile o código usando um compilador C. Por exemplo, com `gcc`:
     ```bash
     gcc -o matriz matriz.c
     ```

2. **Execução:**
   - Execute o programa:
     ```bash
     ./matriz
     ```

### Passo a Passo

1. **Definição da Matriz:**

   - O usuário escolhe a dimensão da matriz (entre 1 e 10).
   - O usuário escolhe se deseja preencher a matriz manualmente ou automaticamente.

2. **Exibição da Matriz:**

   - A matriz gerada é exibida no console.

3. **Transformação e Cálculo:**

   - O programa transforma a matriz em uma matriz triangular superior.
   - Exibe cada operação de troca de linha e aplicação de multiplicador.
   - Calcula e exibe o determinante da matriz.

4. **Repetição:**
   - O usuário pode optar por testar outra matriz.

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
