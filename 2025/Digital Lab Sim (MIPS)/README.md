# Calculadora Estatística e de Sequências em MIPS Assembly

![Linguagem](https://img.shields.io/badge/Linguagem-MIPS%20Assembly-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-MARS%204.5-orange.svg)
![Status](https://img.shields.io/badge/Status-Concluído-brightgreen.svg)

Uma calculadora multifuncional desenvolvida puramente em MIPS Assembly, projetada para rodar no simulador MARS. O projeto utiliza a ferramenta `Digital Lab Sim` para emular a interação com hardware de baixo nível, como um teclado hexadecimal e um display duplo de sete segmentos.

O desenvolvimento seguiu a rigorosa restrição de **não utilizar nenhuma pseudo-instrução**, exigindo a manipulação direta de endereços de memória e o uso exclusivo do conjunto de instruções fundamental da arquitetura MIPS.

![Screenshot da Calculadora em Ação](https://imgur.com/a/aypTnqq)

---

## Tabela de Conteúdos

1.  [Funcionalidades](#-funcionalidades)
2.  [Arquitetura e Implementação](#-arquitetura-e-implementação)
    - [Interface de Hardware (Digital Lab Sim)](#interface-de-hardware-digital-lab-sim)
    - [Estrutura de Dados (`.data`)](#estrutura-de-dados-data)
    - [Fluxo de Execução](#fluxo-de-execução)
    - [Algoritmos Notáveis](#algoritmos-notáveis)
3.  [Como Executar](#-como-executar)
4.  [Autores](#-autores)

---

##  Funcionalidades

A calculadora oferece um conjunto de funcionalidades estatísticas e de cálculo de sequências, operando sobre um conjunto de dados armazenado em uma pilha interna.

| Tecla | Função | Descrição |
| :---: | :--- | :--- |
| **0-9** | **Entrada Numérica** | Permite a digitação de números decimais de até dois dígitos (0-99). |
| **a** | **Acumulador de Pilha** | Adiciona o número digitado a uma pilha interna com capacidade para 10 elementos. |
| **b** | **Média Aritmética** | Calcula a média dos elementos na pilha, com suporte para resultados decimais arredondados (formato `X.Y`). |
| **c** | **Desvio Padrão** | Calcula o desvio padrão populacional dos elementos na pilha, com suporte para resultados decimais. |
| **d** | **Sequência de Van Eck** | Calcula o *n*-ésimo termo da sequência de Van Eck de forma recursiva. |
| **e** | **Sequência de Fibonacci** | Calcula o *n*-ésimo termo da sequência de Fibonacci de forma recursiva. |
| **f** | **Limpeza Total (Clear)**| Zera o display, o buffer de entrada e esvazia completamente a pilha de dados. |
| **EE** | **Exibição de Erro** | Mensagem mostrada para resultados que excedem a capacidade de exibição. |

---

##  Arquitetura e Implementação

### Interface de Hardware (Digital Lab Sim)

A comunicação com o hardware simulado é feita através de **Entrada/Saída Mapeada em Memória (Memory-Mapped I/O)**. Endereços de memória específicos são usados para ler e escrever bytes de controle e dados.

| Componente | Endereço | Operação | Descrição |
| :--- | :--- | :--- | :--- |
| **Display Direito** | `0xFFFF0010` | Escrita (`sb`) | Controla os 8 segmentos (7 + ponto) do display da direita. |
| **Display Esquerdo** | `0xFFFF0011` | Escrita (`sb`) | Controla os 8 segmentos (7 + ponto) do display da esquerda. |
| **Controle Teclado** | `0xFFFF0012` | Escrita (`sb`) | Ativa uma das 4 linhas do teclado para varredura (`1`, `2`, `4`, `8`). |
| **Leitura Teclado** | `0xFFFF0014` | Leitura (`lb`) | Retorna o código da tecla pressionada na linha ativa. |

### Estrutura de Dados (`.data`)

A seção `.data` é a fundação do programa, onde todas as estruturas e constantes são alocadas na memória.

> **AVISO IMPORTANTE PARA DESENVOLVEDORES:** O código depende de **endereços de memória absolutos** calculados manualmente (ex: `0x1001004C`). Qualquer alteração na ordem ou tamanho dos elementos na seção `.data` exigirá o recálculo e a atualização manual desses endereços no código da seção `.text`.

-   **`MAPA_SEGMENTO`**: Uma tabela de consulta (lookup table) essencial que mapeia um número (0-9 e o caractere 'E') ao seu padrão de bits correspondente para o display de 7 segmentos.
-   **`PILHA_ARRAY`**: 40 bytes de espaço (`.space 40`) alocados para nossa pilha customizada, capaz de armazenar 10 números inteiros (palavras de 4 bytes).
-   **`PILHA_TOPO`**: Uma palavra (`.word`) que funciona como contador, indicando quantos elementos estão atualmente na pilha.
-   **`CODIGOS_TECLAS` / `VALORES_TECLAS`**: Duas tabelas que trabalham em conjunto para decodificar a entrada do teclado. A primeira armazena os códigos de hardware brutos, e a segunda, os valores lógicos (0-15) na mesma ordem.

### Fluxo de Execução

A espinha dorsal do programa é um loop infinito que implementa a técnica de **polling** para a leitura do teclado.

1.  **Inicialização (`main`):** Carrega os endereços de hardware e zera os registradores de estado, como o buffer de entrada (`$s3`).
2.  **Loop Principal (`loop_principal`):**
    - Ativa uma linha do teclado por vez (valores `1`, `2`, `4`, `8`).
    - Lê o registrador de dados. Se for zero, passa para a próxima linha.
    - Se for diferente de zero, desvia para a rotina de processamento (`tecla_pressionada`).
3.  **Debouncing (`aguarda_soltar_tecla`):** Uma vez que uma tecla é detectada, o programa entra em um sub-loop que espera o usuário soltar a tecla. Isso garante que cada toque físico na tecla seja processado apenas uma vez, evitando múltiplas leituras indesejadas.
4.  **Decodificação (`busca_tecla_loop`):** O código de hardware da tecla (ex: `0x41`) é comparado com cada entrada da tabela `CODIGOS_TECLAS`. O índice da correspondência é encontrado.
5.  **Roteamento (`tecla_encontrada`):** O índice encontrado é usado para obter o valor lógico na tabela `VALORES_TECLAS`. O programa então desvia para `processa_numero` (se o valor for < 10) ou `processa_funcao` (se for >= 10).

### Algoritmos Notáveis

-   **Aritmética de Ponto Fixo:** Para lidar com resultados decimais sem o uso de hardware de ponto flutuante, o programa utiliza técnicas de ponto fixo.
    -   **Média:** A soma total é multiplicada por 10 antes da divisão para preservar uma casa decimal. A fórmula `(soma * 10 + N / 2) / N` é usada para garantir o arredondamento correto.
    -   **Desvio Padrão:** Os cálculos de diferença são feitos com a média e os valores multiplicados por 10. Isso resulta em um somatório dos quadrados e uma variância que estão implicitamente multiplicados por 100, preservando a precisão necessária para o cálculo da raiz quadrada.

-   **Raiz Quadrada (`raiz_quadrada`):** Implementada usando o **Método Babilônico**, um algoritmo iterativo eficiente que converge para a raiz quadrada inteira de um número. A lógica `x = (x + n / x) / 2` é repetida até que a estimativa `x` se estabilize.

-   **Recursão (`fib_recursivo`, `van_eck_recursivo`):** As funções de Fibonacci e Van Eck foram implementadas de forma recursiva, conforme exigido pela especificação do trabalho. Ambas as funções utilizam a pilha do sistema (`$sp`) para salvar o estado (endereço de retorno e registradores) a cada chamada, garantindo que o fluxo de execução retorne corretamente. A implementação de Van Eck, sendo duplamente recursiva, demonstra o gerenciamento de um estado de pilha mais complexo e possui um custo computacional elevado para valores de *n* maiores.

---

##  Como Executar

1.  Abra o simulador **MARS 4.5**.
2.  No menu `Settings`, certifique-se de que a opção **"Permit extended (pseudo) instructions and formats"** esteja **DESMARCADA**. O código foi escrito para ser independente desta funcionalidade.
3.  Carregue o arquivo `main.asm` (`File -> Open`).
4.  No menu `Tools`, selecione **Digital Lab Sim** e clique em **Connect to MIPS**.
5.  Monte o código clicando em **Assemble** (`F3`).
6.  Execute o programa clicando em **Run** (`F5`). A interação é feita através do teclado virtual do Digital Lab Sim.

---

##  Autores

* **Sizenando S. França**
* **Alexandre**
