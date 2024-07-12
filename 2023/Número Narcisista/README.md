# Programa NUM3R0 N4RC1S1ST4

## Descrição

Este programa em C permite armazenar e manipular números narcisistas em uma lista encadeada. Um número narcisista é aquele cuja soma das potências de seus algarismos é igual ao próprio número. O programa oferece opções para armazenar números dentro de um intervalo específico, excluir números da lista, mostrar os números narcisistas armazenados e encerrar o programa.

## Funcionalidades

- **Armazenar Números:** Permite ao usuário inserir um valor final e encontra todos os números narcisistas entre 10 e o valor fornecido. Os números são armazenados em uma lista encadeada.
- **Excluir Número:** Permite ao usuário remover um número específico da lista de números narcisistas, se estiver presente.
- **Mostrar Números Narcisistas:** Exibe todos os números narcisistas atualmente armazenados na lista.
- **Encerrar Programa:** Encerra a execução do programa, liberando a memória utilizada pela lista encadeada.

## Bibliotecas Utilizadas

- `<stdio.h>`: Para entrada e saída de dados.
- `<stdlib.h>`: Para a função `system()` e alocação de memória dinâmica.
- `<math.h>`: Para a função `pow()` utilizada no cálculo das potências dos algarismos.
- `<unistd.h>`: Para a função `sleep()` que pausa a execução por um número específico de segundos.
- `<myList.h>`: Uma biblioteca personalizada `myList` utilizada para manipular a lista encadeada de números narcisistas.

## Compilação

Para compilar o programa em um ambiente Unix/Linux, utilize o seguinte comando, que inclui a biblioteca matemática `-lm` necessária para a função `pow()`:

```bash
cc -o app main.c myList.a -lm
```

Certifique-se de que o arquivo `myList.a` contém a implementação da lista encadeada conforme utilizada no programa.

## Como Utilizar

1. **Execução:** Após compilar o programa, execute-o.
2. **Menu de Opções:** Escolha uma das opções disponíveis para armazenar números narcisistas, excluir números da lista, mostrar os números armazenados ou encerrar o programa.
3. **Entrada de Dados:** Siga as instruções para inserir valores quando solicitado.
4. **Saída:** O programa exibirá os resultados conforme as operações realizadas.

## Observações

Este programa foi desenvolvido como parte de um trabalho prático para demonstrar habilidades em estruturas de dados (lista encadeada), manipulação de entrada e saída de dados em C, uso de funções matemáticas e controle de fluxo.
