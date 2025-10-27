# Calculadora Estatística e de Sequências em MIPS Assembly

**Autores:** Sizenando S. França (50575), Alexandre...  
**Data:** 27-10-2025  
**Projeto:** Trabalho da disciplina de Linguagens de Montagem

## 1. Descrição do Projeto

Este projeto é uma calculadora multifuncional desenvolvida em MIPS Assembly, projetada para rodar no simulador MARS. A interação com o usuário é feita através da ferramenta `Digital Lab Sim`, que emula um teclado hexadecimal e um display duplo de sete segmentos.

O programa não utiliza nenhuma pseudo-instrução do MIPS, dependendo de cálculos manuais de endereço e do conjunto de instruções fundamental da arquitetura.

## 2. Funcionalidades Implementadas

Atualmente, o projeto conta com as seguintes funcionalidades:

- **Entrada Numérica:** Permite ao usuário digitar números decimais de até dois dígitos (0-99).
- **Exibição no Display:** Mostra o número atual ou resultados nos dois displays de sete segmentos.
- **Gerenciamento de Pilha:**
  - **Função 'a' (Empilhar):** Adiciona o número atual a uma pilha customizada com capacidade para 10 elementos.
  - **Função 'f' (Limpar):** Zera o número atual, o display e limpa a pilha de dados.
- **Cálculo de Sequência:**
  - **Função 'e' (Fibonacci):** Calcula o n-ésimo termo da sequência de Fibonacci de forma recursiva.
- **Tratamento de Erro:** Exibe a mensagem "EE" no display quando um cálculo resulta em um número maior que 99.

## 3. Interface de Hardware: Digital Lab Sim

O programa se comunica com o hardware simulado através de **Entrada/Saída Mapeada em Memória (Memory-Mapped I/O)**. Endereços de memória específicos são usados para controlar os periféricos.

- **Display de 7 Segmentos (Saída):**

  - `0xFFFF0010`: Controla o display da **direita**.
  - `0xFFFF0011`: Controla o display da **esquerda**.
  - Para acender os segmentos, um _byte_ com o padrão de bits correto é escrito nesses endereços usando a instrução `sb`.

- **Teclado Hexadecimal (Entrada):**
  - `0xFFFF0012`: Controle do teclado (escrita). O programa escreve um byte (`1`, `2`, `4` ou `8`) para ativar uma das quatro linhas do teclado.
  - `0xFFFF0014`: Leitura do teclado. O programa lê um byte deste endereço para verificar se uma tecla foi pressionada. Um valor diferente de zero indica uma tecla pressionada.

## 4. Arquitetura do Código

O código é dividido em duas seções principais: `.data` (dados estáticos) e `.text` (instruções executáveis).

### 4.1. Seção `.data` (Estrutura de Dados)

Esta seção define todas as constantes, tabelas e áreas de memória que o programa utiliza. **A ordem das declarações aqui é CRÍTICA**, pois o código `.text` depende de endereços de memória absolutos calculados manualmente.

- `MAPA_SEGMENTO`: Uma tabela de consulta (lookup table) que mapeia um número (0-9 e o caractere 'E') ao seu padrão de bits correspondente para o display de 7 segmentos.
- `PILHA_ARRAY`: 40 bytes de espaço alocados para nossa pilha customizada, capaz de armazenar 10 números inteiros (palavras de 4 bytes).
- `PILHA_TOPO`: Uma palavra (4 bytes) que funciona como contador e ponteiro, indicando quantos elementos estão atualmente na pilha.
- `CODIGOS_TECLAS`: Tabela com os 16 códigos de hardware (ex: `0x11`, `0x21`) retornados pelo teclado.
- `VALORES_TECLAS`: Tabela com os valores lógicos (0 a 15) correspondentes a cada código de hardware, na mesma ordem de `CODIGOS_TECLAS`.

> **AVISO IMPORTANTE:** Se qualquer dado for adicionado, removido ou tiver seu tamanho alterado na seção `.data`, os endereços absolutos usados no código (`0x1001....`) **PRECISAM ser recalculados e atualizados manualmente**.

### 4.2. Seção `.text` (Lógica do Programa)

#### Convenções de Registradores

Para manter a clareza, o programa utiliza registradores de forma consistente:

| Registrador | Propósito Principal                                                      |
| :---------- | :----------------------------------------------------------------------- |
| `$s0`       | Armazena o valor da linha do teclado sendo varrida (`1`, `2`, `4`, `8`). |
| `$s1`       | Usado como índice em loops, principalmente na busca de teclas.           |
| `$s2`       | Armazena o valor lógico **decodificado** da tecla (0-15).                |
| `$s3`       | Acumulador do número atual sendo digitado pelo usuário.                  |
| `$t0`-`$t9` | Registradores temporários para cálculos e endereços.                     |
| `$a0`       | Usado para passar argumentos para sub-rotinas.                           |
| `$v0`       | Usado para receber o valor de retorno de sub-rotinas.                    |
| `$ra`       | Endereço de retorno, salvo na pilha durante chamadas `jal`.              |
| `$sp`       | Ponteiro para a pilha do sistema, usado em funções recursivas.           |

#### Fluxo Principal

1.  **`main` (Inicialização):** Carrega endereços de hardware e inicializa registradores de estado (`$s0`, `$s3`).
2.  **`main_loop` (Loop de Polling):**
    - O programa entra em um loop infinito que varre o teclado.
    - Ele ativa uma linha por vez escrevendo em `0xFFFF0012`.
    - Lê `0xFFFF0014` para ver se uma tecla foi pressionada.
    - Se nenhuma tecla for pressionada, avança para a próxima linha e repete.
    - Se uma tecla for detectada (`bne $t2, $zero`), o fluxo é desviado para `tecla_pressionada`.

#### Processamento de Entrada

1.  **`aguarda_soltar_tecla` (Debouncing):** Uma vez que uma tecla é detectada, o programa entra em um pequeno loop que espera o valor lido do teclado voltar a ser zero. Isso garante que cada toque físico na tecla seja processado apenas uma vez, evitando múltiplas leituras do mesmo toque.
2.  **`busca_tecla_loop` (Decodificação):** Após a tecla ser solta, o código dela (ex: `0x41`) é comparado com cada entrada da tabela `CODIGOS_TECLAS`. O índice da correspondência é encontrado.
3.  **`tecla_encontrada` (Roteamento):** O índice encontrado é usado para buscar o valor lógico na tabela `VALORES_TECLAS`. O programa então verifica se o valor é um número (0-9) ou uma função (10-15) e desvia o fluxo para `processa_numero` ou `processa_funcao`, respectivamente.

### 4.3. Módulos e Sub-rotinas

- **`processa_numero`:**

  - Recebe um novo dígito em `$s2`.
  - Calcula `novo_valor = (valor_antigo * 10) + novo_digito` e armazena em `$s3`.
  - Inclui uma verificação de "overflow" para impedir a entrada de um terceiro dígito.
  - Chama `atualiza_visor` para exibir o novo número.

- **`atualiza_visor(numero)`:**

  - Recebe um número em `$a0`.
  - Usa a instrução `divu` para separar o número em dezena (quociente) e unidade (resto).
  - Usa os resultados como índices para buscar os padrões de bits na `MAPA_SEGMENTO`.
  - Escreve os padrões nos endereços dos displays esquerdo e direito.

- **`processa_funcao`:**

  - Funciona como um "switch-case". Ele compara o valor da função em `$s2` com os valores conhecidos (10 para 'a', 14 para 'e', 15 para 'f') e desvia para a sub-rotina apropriada.

- **`funcao_a` (Empilhar):**

  - Verifica se a pilha não está cheia (se `PILHA_TOPO < 10`).
  - Calcula o endereço de destino na `PILHA_ARRAY` (`endereço_base + (topo * 4)`).
  - Armazena o número de `$s3` no endereço calculado.
  - Incrementa o valor de `PILHA_TOPO` na memória.
  - Reseta `$s3` e o display para `00`.

- **`funcao_f` (Limpar):**

  - Escreve `0` no endereço de `PILHA_TOPO`.
  - Reseta `$s3` para `0`.
  - Chama `atualiza_visor(0)` para limpar o display.

- **`funcao_e` e `fib_recursivo` (Fibonacci):**

  - `funcao_e` atua como um "wrapper": ela chama a função recursiva e depois lida com o resultado (verifica o overflow e chama `exibe_erro` ou `atualiza_visor`).
  - `fib_recursivo` é a implementação canônica da recursão:
    - **Prólogo:** Abre espaço na pilha do sistema (`$sp`) para salvar `$ra` (endereço de retorno) e outros registradores (`$s0`, `$a0`) que serão modificados.
    - **Casos Base:** Verifica se `n=0` ou `n=1` e retorna os valores `0` ou `1` imediatamente.
    - **Passo Recursivo:** Chama a si mesma para `fib(n-1)`, salva o resultado, e depois chama a si mesma para `fib(n-2)`. Soma os dois resultados.
    - **Epílogo:** Restaura os valores salvos da pilha e retorna usando `jr $ra`.

- **`exibe_erro`:**
  - Sub-rotina dedicada a exibir "EE" nos displays. Ela busca o padrão para 'E' no `MAPA_SEGMENTO` e o escreve em ambos os visores.

## 5. Como Compilar e Executar

1.  Abra o arquivo `main.asm` no simulador **MARS**.
2.  Vá até o menu **Settings** e certifique-se de que a opção **"Permit extended (pseudo) instructions and formats"** esteja **DESMARCADA**. O código foi escrito para ser independente desta funcionalidade.
3.  No menu **Tools**, selecione **Digital Lab Sim** e clique em **Connect to MIPS**.
4.  Monte o código clicando em **Assemble** (ou `F3`).
5.  Execute o programa clicando em **Run** (ou `F5`). A interação é feita pelo teclado virtual do Digital Lab Sim.
