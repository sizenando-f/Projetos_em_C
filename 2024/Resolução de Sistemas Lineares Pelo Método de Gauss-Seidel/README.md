# Resolvendo Sistemas Lineares com Gauss Seidel

## Descrição

Este projeto implementa a solução de sistemas de equações lineares utilizando o método de Gauss-Seidel. O programa permite ao usuário definir uma matriz de coeficientes e um vetor de termos independentes, tanto manualmente quanto automaticamente, e encontra a solução do sistema de acordo com a precisão desejada pelo usuário.

## Funcionalidades

1. **Geração de números aleatórios**: Função auxiliar para gerar números aleatórios em um intervalo especificado.
2. **Criação de matriz aleatória**: Inicializa uma matriz com valores aleatórios.
3. **Exibição de matriz**: Exibe a matriz e os termos independentes no console.
4. **Definição de matriz**: Permite ao usuário definir manualmente ou gerar automaticamente a matriz e os termos independentes.
5. **Inicialização de solução**: Permite ao usuário definir a solução inicial.
6. **Solução do sistema**: Aplica o método de Gauss-Seidel para encontrar a solução do sistema.
7. **Cálculo de precisão**: Compara duas soluções consecutivas para verificar a precisão da solução.

## Estrutura do Código

### Funções

- **numAleatorio(double min, double max)**

  - Gera um número aleatório do tipo `double` entre `min` e `max`.

- **matrizAleatoria(double matriz[10][10], unsigned ordem, double termoInd[])**

  - Inicializa uma matriz e um vetor de termos independentes com valores aleatórios.

- **exibeMatriz(double matriz[10][10], unsigned ordem, double termoInd[])**

  - Exibe a matriz e os termos independentes no console.

- **defineMatriz(double matriz[10][10], unsigned \*ordem, double termoInd[])**

  - Permite ao usuário definir a matriz e os termos independentes manualmente ou automaticamente.

- **inicializaSolucao(double solucao[10], unsigned int ordem)**

  - Permite ao usuário definir a solução inicial.

- **solucionaSistema(double matriz[10][10], double termoInd[10], double solucao[10], double solucaoPost[10], unsigned ordem)**

  - Aplica o método de Gauss-Seidel para encontrar a solução do sistema.

- **descobrePrecisao(double solucaoAnt[10], double solucaoPost[10], unsigned ordem)**
  - Calcula a precisão entre duas soluções consecutivas.

### Função Principal

- **main()**
  - Inicializa variáveis, define a matriz e os termos independentes, recebe a precisão desejada, e itera até encontrar a solução com a precisão especificada.

## Compilação e Execução

### Pré-requisitos

- Compilador C (como `gcc`)

### Compilação

Para compilar o programa, use o seguinte comando no terminal:

```sh
gcc -o sistema_linear main.c
```

### Execução

Para executar o programa, use o seguinte comando no terminal:

```sh
./sistema_linear
```

## Uso

1. Ao executar o programa, será solicitado ao usuário definir a ordem do sistema linear (entre 1 e 10).
2. O usuário pode escolher preencher a matriz manualmente ou automaticamente.
3. Se escolhido manualmente, o usuário deve inserir os coeficientes da matriz e os termos independentes.
4. O usuário define a precisão desejada.
5. O programa irá iterar até encontrar a solução com a precisão especificada, exibindo cada iteração no console.

## Observações

- O programa está limitado a matrizes de no máximo 10x10 devido à definição estática dos arrays.
- O método de Gauss-Seidel requer uma boa escolha de solução inicial para garantir a convergência, que pode não ocorrer para todas as matrizes.
