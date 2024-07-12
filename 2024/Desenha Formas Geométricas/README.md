# Desenho Recursivo de Figuras Geométricas

Este programa em C utiliza recursão para desenhar figuras geométricas (círculos e quadrados) na tela utilizando a biblioteca `gfx.h`.

## Funcionalidades

- **Desenho de Círculo**: Utiliza recursão para desenhar círculos concêntricos.
- **Desenho de Quadrado**: Utiliza recursão para desenhar quadrados concêntricos.
- **Seleção do Usuário**: O usuário escolhe qual figura deseja desenhar (círculo ou quadrado) e o nível de recursão.
- **Limitação de Recursão**: Restringe o nível máximo de recursão de 0 a 10 para evitar desenhos excessivamente complexos.
- **Interface Gráfica Simples**: Utiliza a biblioteca `gfx.h` para manipular gráficos simples na tela.
- **Tempo de Exibição**: Após desenhar a figura, espera por 5 segundos antes de fechar a janela.

## Bibliotecas Utilizadas

- **gfx.h**: Biblioteca gráfica simples para manipulação de gráficos.
- **stdio.h**: Para funções de entrada e saída.
- **unistd.h**: Para utilizar a função `sleep` para pausas.

## Compilação e Execução

Para compilar o programa, é necessário garantir que a biblioteca `gfx.h` esteja configurada corretamente no sistema. Utilize o seguinte comando:

```bash
gcc -o desenho_recursivo desenho_recursivo.c -lgfx -lm
```

Para executar o programa:

```bash
./desenho_recursivo
```

## Como Usar

1. O programa exibirá um menu com as opções:

   - 1: Desenhar Círculo
   - 2: Desenhar Quadrado
   - 3: Sair do Programa

2. Insira o número correspondente à figura que deseja desenhar.
3. Insira o nível de recursão desejado (de 0 a 10). Níveis mais altos aumentam a complexidade do desenho.
4. Após desenhar a figura, a janela será exibida por 5 segundos antes de fechar automaticamente.

## Exemplos de Uso

```bash
[ 1 ] Circulo
[ 2 ] Quadrado
O que deseja desenhar? > 1
Insira o nivel de recursao (0 - 10) > 3

Desenhando círculos concêntricos...

...

[ 1 ] Circulo
[ 2 ] Quadrado
O que deseja desenhar? > 2
Insira o nivel de recursao (0 - 10) > 2

Desenhando quadrados concêntricos...

...

[ 1 ] Circulo
[ 2 ] Quadrado
O que deseja desenhar? > 3
Programa encerrado!
```
