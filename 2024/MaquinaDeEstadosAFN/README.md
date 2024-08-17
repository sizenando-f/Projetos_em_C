# Simulação de Máquina de Estados Finitos Não-Determinística (AFN)

Este projeto implementa uma simulação de uma Máquina de Estados Finitos Não-Determinística (AFN) em C. A máquina aceita ou rejeita palavras de acordo com as transições definidas e os estados de aceitação fornecidos pelo usuário.

## Funcionalidades

- **Configuração do Alfabeto:** O usuário define um alfabeto de duas letras.
- **Definição de Estados:** O usuário insere os estados da máquina, o estado inicial, e os estados de aceitação.
- **Função de Transição:** A função de transição é configurada para cada par de estado e letra do alfabeto, incluindo transições epsilon (transições sem consumir caracteres da palavra).
- **Execução da Máquina:** A máquina processa palavras de entrada e determina se são aceitas ou rejeitadas com base nas transições e nos estados de aceitação.

## Como Executar

### Pré-requisitos

- **Compilador C**: GCC ou similar.

### Compilação

Compile o código com o comando:

```bash
gcc -o afn_simulator main.c
```

### Execução

Execute o programa com o comando:

```bash
./afn_simulator
```

### Interação com o Programa

1. **Definir o Alfabeto**: O programa solicitará ao usuário que insira duas letras que compõem o alfabeto da máquina.

2. **Inserir os Estados da Máquina**: O usuário deve inserir todos os estados da máquina, separados por vírgulas (e.g., `q0,q1,q2`).

3. **Definir o Estado Inicial**: O programa solicitará que o usuário insira o estado inicial da máquina.

4. **Definir os Estados de Aceitação**: O usuário deve inserir os estados de aceitação, separados por vírgulas.

5. **Configurar a Função de Transição**: O usuário será solicitado a inserir as transições para cada estado e símbolo do alfabeto, incluindo transições epsilon. Use `-` para indicar a ausência de transição.

6. **Testar Palavras**: O usuário pode inserir palavras para verificar se são aceitas pela máquina. Para sair, digite `sair`.

## Exemplo de Uso

```text
[ <- ] Insira a primeira letra do alfabeto > a
[ <- ] Insira a segunda letra do alfabeto > b
-----------------------------------------------------------------------
[ <- ] Insira os estados da maquina (separe por virgulas) > q0,q1,q2
-----------------------------------------------------------------------
[ <- ] Insira o estado inicial da maquina > q0
-----------------------------------------------------------------------
[ <- ] Insira os estados de aceitacao da maquina (separe por virgulas) > q2
-----------------------------------------------------------------------
[ <- ] Delta(q0, a) > q1
[ <- ] Delta(q0, b) > -
[ <- ] Delta(q0, e) > -
[ <- ] Delta(q1, a) > -
[ <- ] Delta(q1, b) > q2
[ <- ] Delta(q1, e) > -
[ <- ] Delta(q2, a) > -
[ <- ] Delta(q2, b) > -
[ <- ] Delta(q2, e) > -
-----------------------------------------------------------------------
            [# VISAO GERAL #]
[ @ ] Estados da maquina   -> [q0] [q1] [q2]
[ @ ] Alfabeto             -> [a] [b]
[ @ ] Estado inicial       -> [q0]
[ @ ] Estados de aceitacao -> [q2]

  { FUNCAO }
--------------
|   | a    b    e |
--------------
| q0 | q1    -    - |
| q1 | -    q2    - |
| q2 | -    -    - |
--------------

[ <- ] Insira a palavra para ser testada (ou "sair" para sair) > ab
[# RESULTADO #] A palavra foi aceita pela maquina!
```

## Explicação das Funções

### Funções de Entrada

- **`getAlfabeto(char alfabeto[2])`**: Solicita ao usuário as duas letras do alfabeto.
- **`getEstados(char estados[20], int *numEstados)`**: Recebe os estados da máquina.
- **`getEstadoInicial(char estados[20], char *estadoInicial, int numEstados)`**: Define o estado inicial.
- **`getEstadosDeAceitacao(char estados[20], char estadosDeAceitacao[20], int numEstados)`**: Define os estados de aceitação.
- **`getFuncao(char funcao[20][3][20], char estados[20], int numEstados, char alfabeto[2])`**: Recebe a função de transição.

### Funções de Execução

- **`executaAFN(char estadoAtual, char estados[20], char alfabeto[], char estadosDeAceitacao[20], char funcao[20][3][20], char palavra[100], int tamPalavra, int posicao)`**: Processa a palavra na máquina, retornando se é aceita ou rejeitada.

### Funções de Verificação

- **`verificaEstado(char estados[20], char estado, int numEstados)`**: Verifica se um estado existe no conjunto de estados.
- **`verificaPalavra(char palavra[100], char alfabeto[2], int tamPalavra)`**: Verifica se todas as letras da palavra estão no alfabeto.
- **`verificaEstados(char estados[20], char palavra[20], int numEstados, int numEstadosAceitacao)`**: Verifica se os estados de aceitação são válidos.
