# Calculadora de Equações Quadráticas

Este programa em C permite calcular as raízes de uma equação quadrática (ax² + bx + c = 0) fornecendo os valores de a, b e c.

#### Funcionalidades

1. **Calcula Delta**

   - A função `calculaDelta(int a, int b, int c)` calcula o delta da equação quadrática utilizando a fórmula:
     ```
     delta = b² - 4ac
     ```
   - Mostra passo a passo o cálculo do delta e determina se existem raízes reais distintas (delta > 0), raízes reais e iguais (delta = 0) ou raízes complexas conjugadas (delta < 0).

2. **Calcula Raízes**
   - A função `calcularQuadratica(int a, int b, int delta)` utiliza a fórmula quadrática para calcular as raízes da equação:
     ```
     x1,2 = (-b ± sqrt(delta)) / 2a
     ```
   - Mostra o cálculo detalhado de x1 e x2, considerando os diferentes casos de delta.

#### Utilização

Para utilizar o programa:

1. Execute o programa e insira os valores de a, b e c quando solicitado.
2. O programa calculará o delta da equação.
3. Em seguida, calculará as raízes da equação e as exibirá na tela.

#### Requisitos

- Compilador C (por exemplo, GCC) instalado no sistema.
- Ambiente de desenvolvimento C adequado (como Code::Blocks, Dev-C++, Visual Studio Code com extensão C/C++, etc.).

#### Instruções de Compilação e Execução

1. **Compilação:**

   ```
   gcc nome_do_arquivo.c -o nome_do_executavel
   ```

   Substitua `nome_do_arquivo.c` pelo nome do arquivo onde está o código e `nome_do_executavel` pelo nome desejado para o executável.

2. **Execução:**
   ```
   ./nome_do_executavel
   ```
   Substitua `nome_do_executavel` pelo nome do executável gerado na etapa de compilação.

#### Exemplo de Uso

Ao executar o programa, você será solicitado a inserir os valores de a, b e c da equação quadrática. O programa então calculará o delta e as raízes, mostrando todos os passos do cálculo na tela.

#### Observações

- Certifique-se de fornecer valores adequados para a, b e c para obter resultados precisos.
- O programa faz uso das bibliotecas padrão `stdlib.h`, `stdio.h`, `ctype.h` e `math.h`.
