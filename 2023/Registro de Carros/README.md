# Gestão de Registros de Carros

## Descrição

O programa permite realizar diversas operações de gestão e análise de registros de carros armazenados em um arquivo binário `carro.dbf`. Ele oferece funcionalidades como contagem de registros, ordenação por placa, exibição de registros ordenados, contagem de carros por tipo de combustível e contagem de carros por tipo de opcional.

## Funcionalidades

1. **Contagem de Registros:** Informa a quantidade de registros presentes no arquivo `carro.dbf`.
2. **Ordenação de Registros:** Ordena os registros por placa em ordem crescente e armazena-os em um novo arquivo `carro.ord`.
3. **Exibição de Registros Ordenados:** Mostra os registros ordenados pelo arquivo `carro.ord`.
4. **Quantidade de Carros por Combustível:** Informa quantos carros existem para cada tipo de combustível encontrado nos registros.
5. **Quantidade de Carros por Opcional:** Informa quantos carros possuem cada um dos opcionais especificados.

## Bibliotecas Utilizadas

- `<stdio.h>`: Para entrada e saída de dados.
- `<stdlib.h>`: Para as funções `system()` e alocação de memória dinâmica.
- `<string.h>`: Para manipulação de strings e comparações.
- `<math.h>`: Não é utilizada diretamente neste programa.

## Compilação e Execução

Para compilar o programa, utilize o compilador C padrão. Um exemplo de compilação no ambiente Unix/Linux seria:

```bash
cc -o registro main.c
```

Após compilar, execute o programa com:

```bash
./registro
```

## Como Utilizar

1. **Menu de Opções:** Ao iniciar o programa, escolha uma das opções disponíveis no menu.
2. **Operações:** Siga as instruções fornecidas para cada opção escolhida.
3. **Saída:** O programa exibirá os resultados na tela conforme solicitado.

## Observações

Este programa foi desenvolvido para fins educacionais e demonstra o uso de estruturas de dados em C para manipulação de arquivos binários, ordenação e análise de dados. Certifique-se de que o arquivo `carro.dbf` esteja presente e contenha os dados necessários para o funcionamento correto do programa.
