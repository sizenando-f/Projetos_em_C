# Máquina de Estados

## Descrição

Este projeto implementa uma simulação de uma Máquina de Estados determinística. A máquina é configurada pelo usuário com um conjunto de estados, alfabeto, estado inicial, estados de aceitação e uma função de transição. Após a configuração, o programa permite testar palavras para verificar se são aceitas ou não pela máquina.

- **Versão**: 1.2.2
- **Autor**: Sizenando S. França
- **Data de criação**: 19/05/24
- **Última modificação**: 23/05/24

## Funcionalidades

- **Configuração da Máquina**: O usuário insere os estados, alfabeto, estado inicial, estados de aceitação e função de transição.
- **Verificação de Entradas**: O programa verifica se os estados e letras inseridos pertencem ao conjunto definido.
- **Execução da Máquina**: O usuário pode testar palavras para ver se são aceitas pela máquina, exibindo o caminho percorrido e indicando se a palavra foi aceita ou não.
- **Interface de Usuário Intuitiva**: Utiliza limpeza de tela automática de acordo com o sistema operacional para melhor experiência do usuário.

## Bibliotecas Utilizadas

- **stdio.h**: Para funções de entrada e saída padrão.
- **stdlib.h**: Para a função `system()` utilizada na limpeza de tela.
- **string.h**: Para manipulação de strings e funções como `strcpy()` e `strlen()`.

## Compilação e Execução

Para compilar o programa, utilize o seguinte comando:

```bash
gcc -o maquina_de_estados main.c
```

Para executar o programa compilado:

```bash
./maquina_de_estados
```

## Como Usar

1. **Configuração da Máquina**:

   - Insira os estados da máquina separados por vírgulas.
   - Defina o alfabeto da máquina inserindo duas letras.
   - Especifique o estado inicial da máquina.
   - Informe os estados de aceitação separados por vírgulas.
   - Para cada estado e letra do alfabeto, defina a transição utilizando '-' para representar um estado parcial.

2. **Teste de Palavras**:

   - Após configurar a máquina, insira uma palavra para testar se é aceita pela máquina.
   - O programa mostrará o caminho percorrido pela palavra na máquina e indicará se a palavra foi aceita ou não.

3. **Continuação e Encerramento**:
   - Após testar uma palavra, o usuário pode optar por testar outra palavra ou encerrar o programa.

## Exemplo de Uso

```bash
            ==== MAQUINA DE ESTADOS ====
[ <- ] Insira os estados da maquina (separe por virgulas) > A,B,C,D
[ <- ] Insira a primeira letra do alfabeto > a
[ <- ] Insira a segunda letra do alfabeto > b
-----------------------------------------------------------------------
[ <- ] Insira os estados de aceitacao da maquina (separe por virgulas) > C,D
-----------------------------------------------------------------------
[ <- ] Insira o estado inicial da maquina > A
-----------------------------------------------------------------------
[ <- ] Delta(A, a) > B
[ <- ] Delta(A, b) > C
[ <- ] Delta(B, a) > D
[ <- ] Delta(B, b) > -
[ <- ] Delta(C, a) > -
[ <- ] Delta(C, b) > C
[ <- ] Delta(D, a) > -
[ <- ] Delta(D, b) > C
-----------------------------------------------------------------------
            [# VISAO GERAL #]
[ @ ] Estados da maquina   -> [A] [B] [C] [D]
[ @ ] Alfabeto             -> [a] [b]
[ @ ] Estado inicial       -> [A]
[ @ ] Estados de aceitacao -> [C] [D]

  { FUNCAO }
--------------
|   | a    b |
--------------
| A | B |C |
--------------
| B | D |- |
--------------
| C |- |C |
--------------
| D |- |C |
--------------
[ <- ] Deseja continuar? [S/n] > S

[ <- ] Insira a palavra > abab
-----------------------------------------------------------------------
Palavra abab
< Estados de aceitacao [C] [D] >
 -> [A] -> [B] -> [D] -> [C] -> [C]
[ -> ] A maquina terminou em um estado aceitacao, a palavra foi aceita
[ <- ] Deseja tentar outra palavra? [S/n] > n
[ -> ] PROGRAMA ENCERRADO
```

## Contribuições

Contribuições são bem-vindas! Para sugestões de melhorias ou correções, entre em contato com o autor ou abra uma issue no repositório do projeto.
