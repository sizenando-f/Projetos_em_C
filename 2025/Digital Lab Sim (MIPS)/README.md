# Projeto Digital Lab Sim - Implementação em Assembly MIPS

## Visão Geral

Este projeto consiste em uma implementação abrangente em Assembly MIPS desenvolvida para o simulador Digital Lab Sim. O sistema oferece uma suite completa de funcionalidades matemáticas e de manipulação de dados, acessíveis através de uma interface de teclado matricial com resultados exibidos em display de 7 segmentos. O programa integra operações de pilha, cálculos estatísticos avançados e sequências matemáticas complexas em uma arquitetura coesa e eficiente.

## Índice

1. [Funcionalidades Principais](#funcionalidades-principais)
2. [Arquitetura do Sistema](#arquitetura-do-sistema)
3. [Fluxo de Execução Detalhado](#fluxo-de-execução-detalhado)
4. [Algoritmos e Implementações Matemáticas](#algoritmos-e-implementações-matemáticas)
5. [Interface de Hardware e Usuário](#interface-de-hardware-e-usuário)
6. [Estruturas de Dados e Gerenciamento de Memória](#estruturas-de-dados-e-gerenciamento-de-memória)
7. [Sistema de Tratamento de Erros e Validações](#sistema-de-tratamento-de-erros-e-validações)
8. [Registradores e Convenções de Uso](#registradores-e-convenções-de-uso)
9. [Exemplos de Casos de Uso](#exemplos-de-casos-de-uso)
10. [Considerações Técnicas e Limitações](#considerações-técnicas-e-limitações)

## Funcionalidades Principais

### Sistema de Entrada Numérica

**Teclas 0-9**: Implementa um sistema completo de entrada numérica com as seguintes características:
- Buffer de entrada com capacidade para números de 0 a 99
- Validação em tempo real para evitar estouro de buffer
- Atualização imediata do display durante a digitação
- Mecanismo de reset automático ao detectar tentativa de entrada de terceiro dígito

### Sistema de Gerenciamento de Pilha

**Tecla A (Empilhamento)**: Gerencia uma estrutura de dados do tipo pilha com as seguintes propriedades:
- Capacidade para 10 elementos de 32 bits cada
- Controle preciso do topo através de variável dedicada
- Verificação de limites para prevenir estouro negativo e positivo
- Armazenamento em array com alocação estática de 40 bytes
- Operação atômica de empilhamento com preservação do estado do sistema

### Módulo de Cálculos Estatísticos

**Tecla B (Média Aritmética)**: Implementa cálculo de média com as seguintes capacidades:
- Suporte para médias inteiras e decimais
- Algoritmo de arredondamento para uma casa decimal
- Cálculo: (soma × 10 + N/2) / N para precisão decimal
- Validação de exibição para valores entre 0 e 99
- Tratamento especial para pilha vazia (retorna 0)

**Tecla C (Desvio Padrão Amostral)**: Calcula estatística descritiva avançada:
- Requer mínimo de 2 elementos na pilha para cálculo válido
- Implementa fórmula de variância amostral: σ = √[Σ(xᵢ - μ)² / (N-1)]
- Processo em múltiplos estágios com precisão decimal
- Uso de algoritmo de raiz quadrada personalizado
- Exibição inteligente (inteira ou decimal conforme necessário)

### Módulo de Sequências Matemáticas

**Tecla D (Sequência de Van Eck)**: Implementa sequência matemática complexa:
- Definição: Para n ≥ 1, a(n) = número de steps desde a última ocorrência de a(n-1)
- Implementação recursiva com múltiplos casos base
- Busca retrospectiva na sequência para determinar valores
- Limitação de exibição para valores menores que 100

**Tecla E (Sequência de Fibonacci)**: Implementa a clássica sequência recursiva:
- Casos base: fib(0) = 0, fib(1) = 1
- Recursão: fib(n) = fib(n-1) + fib(n-2)
- Controle de profundidade recursiva através do stack pointer
- Validação de resultado para exibição (0-99)

### Sistema de Controle e Utilidades

**Tecla F (Limpeza Total)**: Função de manutenção do sistema:
- Reset completo do estado da pilha (topo = 0)
- Limpeza do buffer de entrada corrente
- Reinicialização do display para estado inicial (00)
- Preparação do sistema para novo ciclo de operações

## Arquitetura do Sistema

### Mapeamento de Hardware

O sistema utiliza endereços de memória mapeados para periféricos conforme especificação do Digital Lab Sim:

| Componente | Endereço | Modo | Função Específica |
|------------|----------|------|-------------------|
| `VISOR_D` | 0xFFFF0010 | Escrita | Controla display direito (dígito das unidades) |
| `VISOR_E` | 0xFFFF0011 | Escrita | Controla display esquerdo (dígito das dezenas) |
| `CHAVE_CONTROLE` | 0xFFFF0012 | Escrita | Ativa linhas do teclado matricial para varredura |
| `CONTADOR_CONTROLE` | 0xFFFF0013 | Escrita | Configuração de contadores de hardware |
| `CHAVE_DADO` | 0xFFFF0014 | Leitura | Leitura das colunas do teclado matricial |

### Organização de Memória

A aplicação utiliza uma organização de memória bem definida com as seguintes seções:

**.data (Segmento de Dados)**
- Tabelas de consulta para display (MAPA_SEGMENTO)
- Estrutura de pilha (PILHA_ARRAY) com 10 posições
- Variáveis de controle (PILHA_TOPO)
- Tabelas de mapeamento teclado-valor (CODIGOS_TECLAS, VALORES_TECLAS)

**.text (Segmento de Código)**
- Rotina principal de inicialização e loop
- Subrotinas de tratamento de teclado
- Implementações de algoritmos matemáticos
- Funções de atualização de display

## Fluxo de Execução Detalhado

### Inicialização do Sistema (main)

```asm
main:
    lui $t0, 0xFFFF         # Carrega parte alta do endereço de controle
    ori $t0, $t0, 0x0012    # Completa endereço para CHAVE_CONTROLE
    lui $t1, 0xFFFF         # Carrega parte alta do endereço de dados  
    ori $t1, $t1, 0x0014    # Completa endereço para CHAVE_DADO
    addiu $s0, $zero, 1     # Inicializa varredura na linha 1
    addiu $s3, $zero, 0     # Inicializa buffer de entrada com 0
```

### Loop Principal de Varredura (main_loop)

O sistema implementa um algoritmo de varredura não-bloqueante:

1. **Ativação de Linha**: Escreve o padrão de bits correspondente à linha atual em CHAVE_CONTROLE
2. **Leitura de Colunas**: Lê o estado das colunas através de CHAVE_DADO
3. **Detecção de Tecla**: Verifica se alguma coluna está ativa (valor ≠ 0)
4. **Transição de Linha**: Avança para próxima linha através de deslocamento bit-a-bit
5. **Reset Cíclico**: Reinicia varredura após linha 8 (padrão 1000 → 0001)

### Processamento de Interrupção de Teclado (tecla_pressionada)

Quando uma tecla é detectada, o sistema executa:

1. **Debouncing**: Aguarda liberação física da tecla
2. **Decodificação**: Converte código de hardware para valor lógico
3. **Classificação**: Identifica se é dígito numérico ou função
4. **Roteamento**: Encaminha para handler específico

### Pipeline de Processamento Numérico (processa_numero)

```asm
processa_numero:
    sltiu $t3, $s3, 10          # Verifica se número atual < 10
    beq $t3, $zero, estouro_buffer_entrada  # Se ≥10, trata estouro
    addiu $t3, $zero, 10        # Prepara multiplicador decimal
    multu $s3, $t3              # Multiplica número atual por 10
    mflo $s3                    # Recupera resultado
    addu $s3, $s3, $s2          # Adiciona novo dígito
    add $a0, $s3, $zero         # Prepara argumento para display
    jal atualiza_visor          # Atualiza interface
```

## Algoritmos e Implementações Matemáticas

### Algoritmo de Raiz Quadrada Inteira (raiz_quadrada)

Implementa o método de Newton-Raphson para cálculo de raiz quadrada:

```pseudocode
function raiz_quadrada(n):
    if n == 0:
        return 0
    
    x = n
    last_x = 0
    
    while x ≠ last_x:
        last_x = x
        x = (x + n/x) / 2
        if x == 0:
            x = 1  # Prevenção de divisão por zero
    
    return x
```

### Algoritmo Recursivo de Van Eck (van_eck_recursivo)

Implementa a complexa sequência de Van Eck:

```pseudocode
function van_eck(n):
    if n == 0:
        return 0
    
    anterior = van_eck(n-1)
    j = -1
    
    for i from n-2 down to 0:
        if van_eck(i) == anterior:
            j = i
            break
    
    if j == -1:
        return 0
    else:
        return (n-1) - j
```

### Algoritmo de Desvio Padrão Amostral

Processo multi-estágio com precisão decimal:

```pseudocode
function calcular_desvio_padrao():
    N = numero_elementos_pilha
    
    if N < 2:
        return 0.0
    
    # Estágio 1: Cálculo da média com precisão decimal
    soma = Σ elemento[i] para i=0 até N-1
    media_10 = (soma × 10) / N  # Mantém uma casa decimal
    
    # Estágio 2: Soma dos quadrados das diferenças
    soma_quadrados = 0
    for each elemento xi in pilha:
        diff = (xi × 10) - media_10  # Diferença em escala decimal
        soma_quadrados += diff × diff
    
    # Estágio 3: Variância amostral com arredondamento
    variancia_100 = (soma_quadrados + (N-1)/2) / (N-1)
    
    # Estágio 4: Raiz quadrada e ajuste final
    desvio_10 = raiz_quadrada(variancia_100)
    return desvio_10  # Representa desvio_padrao × 10
```

## Interface de Hardware e Usuário

### Protocolo de Comunicação com Display

O sistema utiliza dois displays de 7 segmentos independentes:

**Display Esquerdo (VISOR_E)**: Controla dígito das dezenas
- Endereço: 0xFFFF0011
- Bit 7 (0x80) ativa ponto decimal quando necessário

**Display Direito (VISOR_D)**: Controla dígito das unidades  
- Endereço: 0xFFFF0010
- Sem ponto decimal

**Tabela de Mapeamento de Segmentos**:
```
0: 0x3F (0111111)    5: 0x6D (1101101)
1: 0x06 (0000110)    6: 0x7D (1111101) 
2: 0x5B (1011011)    7: 0x07 (0000111)
3: 0x4F (1001111)    8: 0x7F (1111111)
4: 0x66 (1100110)    9: 0x6F (1101111)
E: 0x79 (1111001)   Ponto: 0x80 (10000000)
```

### Sistema de Varredura de Teclado Matricial

**Organização Física do Teclado**:
```
    [1] [2] [3] [A]    Linha 1: 0x01
    [4] [5] [6] [B]    Linha 2: 0x02  
    [7] [8] [9] [C]    Linha 3: 0x04
    [0] [F] [E] [D]    Linha 4: 0x08
```

**Protocolo de Varredura**:
1. Ativar linha escrevendo padrão de bits em CHAVE_CONTROLE
2. Ler padrão de bits das colunas em CHAVE_DADO
3. Combinar linha e coluna para identificar tecla única
4. Mapear código hardware para valor lógico através de tabela

## Estruturas de Dados e Gerenciamento de Memória

### Implementação da Pilha

**Estrutura**:
```asm
PILHA_ARRAY:    .space 40    # 10 elementos × 4 bytes cada
PILHA_TOPO:     .word 0      # Índice do próximo elemento (0-10)
```

**Operações**:
- **Empilhamento**: PILHA_ARRAY[PILHA_TOPO] = valor; PILHA_TOPO++
- **Consulta**: Acesso direto via índice para cálculos
- **Capacidade**: Limitada a 10 elementos com verificação de estouro

### Tabelas de Mapeamento

**CODIGOS_TECLAS**: Mapeia combinações linha-coluna para identificadores:
```asm
.byte 0x11, 0x21, 0x41, 0x81  # 1, 0, 2, 3
.byte 0x12, 0x22, 0x42, 0x82  # 4, 5, 6, 7  
.byte 0x14, 0x24, 0x44, 0x84  # 8, 9, A, B
.byte 0x18, 0x28, 0x48, 0x88  # C, D, E, F
```

**VALORES_TECLAS**: Mapeia identificadores para valores lógicos:
```asm
.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
```

## Sistema de Tratamento de Erros e Validações

### Categorias de Erro Tratadas

**Estouro de Buffer de Entrada**:
- Condição: Tentativa de inserir terceiro dígito (valor ≥ 10)
- Ação: Reset automático do buffer para 0
- Objetivo: Manter consistência da interface

**Estouro de Pilha**:
- Condição: Tentativa de empilhar com PILHA_TOPO ≥ 10
- Ação: Operação ignorada e buffer resetado
- Objetivo: Preservar integridade dos dados

**Valores de Exibição Excedidos**:
- Condição: Resultado de cálculo ≥ 100
- Ação: Exibição de "EE" em ambos displays
- Objetivo: Indicar claramente limitação do hardware

**Condições Matemáticas Inválidas**:
- Desvio padrão com N < 2: Retorna 0.0
- Pilha vazia em cálculos: Retorna 0
- Fibonacci/Van Eck com entrada inválida: Tratamento específico

### Protocolo de Exibição de Erro

A exibição "EE" é reservada para condições de erro críticas:
- Ambos displays mostram padrão 0x79 (letra E)
- Buffer de entrada é automaticamente resetado
- Estado da pilha permanece inalterado
- Sistema permanece responsivo a novas entradas

## Registradores e Convenções de Uso

### Registradores de Propósito Específico

| Registrador | Tipo | Função Principal | Preservação entre Chamadas |
|-------------|------|------------------|---------------------------|
| `$s0` | Salvo | Linha atual de varredura do teclado | Preservado |
| `$s1` | Salvo | Índice para busca em tabela de teclas | Preservado |
| `$s2` | Salvo | Valor lógico da tecla pressionada | Preservado |
| `$s3` | Salvo | Buffer de entrada numérica atual | Preservado |
| `$s4` | Salvo | Número de elementos na pilha (N) | Preservado |
| `$s5` | Salvo | Acumulador para cálculos de soma | Preservado |
| `$s6` | Salvo | Média × 10 para cálculos de precisão | Preservado |
| `$s7` | Salvo | Soma dos quadrados para desvio padrão | Preservado |

### Convenções de Chamada de Função

**Funções com Argumentos**:
- `$a0` a `$a3`: Argumentos de entrada
- `$v0` a `$v1`: Valores de retorno

**Registradores Temporários**:
- `$t0` a `$t9`: Uso livre sem preservação
- `$s0` a `$s7`: Devem ser preservados pelo caller

**Protocolo de Stack para Funções Recursivas**:
```asm
funcao_recursiva:
    addiu $sp, $sp, -frame_size  # Aloca espaço na pilha
    sw $ra, offset_ra($sp)       # Salva endereço de retorno
    sw $s0, offset_s0($sp)       # Salva registradores salvos
    # ... corpo da função ...
    lw $ra, offset_ra($sp)       # Restaura endereço de retorno
    lw $s0, offset_s0($sp)       # Restaura registradores salvos
    addiu $sp, $sp, frame_size   # Libera espaço na pilha
    jr $ra                       # Retorna ao caller
```

## Exemplos de Casos de Uso

### Caso 1: Cálculo Estatístico Completo

**Objetivo**: Calcular média e desvio padrão de conjunto de dados

**Operações**:
```
Entrada: 25 [A] 35 [A] 45 [A] 
Processo: Empilha três valores (25, 35, 45)

Ação: [B] (Média)
Cálculo: (25 + 35 + 45) / 3 = 35
Display: "35"

Ação: [C] (Desvio Padrão)
Cálculo: 
  Média = 35
  Diferenças: -10, 0, +10
  Quadrados: 100, 0, 100
  Soma quadrados = 200
  Variância = 200 / 2 = 100
  Desvio padrão = √100 = 10
Display: "10"
```

### Caso 2: Trabalho com Números Decimais

**Objetivo**: Demonstrar precisão decimal em cálculos

**Operações**:
```
Entrada: 5 [A] 6 [A]
Processo: Empilha valores 5 e 6

Ação: [B] (Média)
Cálculo: (5 + 6) / 2 = 5.5
Processamento interno:
  Soma × 10 = (5+6) × 10 = 110
  Adiciona termo arredondamento: 110 + 1 = 111
  Divide por N: 111 / 2 = 55.5 → truncado para 55
Display: "5.5" (5 com ponto no esquerdo, 5 no direito)
```

### Caso 3: Sequências Matemáticas

**Objetivo**: Calcular termos de sequências complexas

**Operações**:
```
Entrada: 1 [0] (Forma número 10)
Ação: [E] (Fibonacci)
Cálculo: fib(10) = 55
Display: "55"

Entrada: 5 
Ação: [D] (Van Eck)
Cálculo: 
  van_eck(1) = 0
  van_eck(2) = 0
  van_eck(3) = 1
  van_eck(4) = 0
  van_eck(5) = 2
Display: "2"
```

## Considerações Técnicas e Limitações

### Limitações de Hardware

**Display de 7 Segmentos**:
- Capacidade máxima: 2 dígitos (0-99)
- Sem suporte para números negativos
- Formato decimal limitado a X.Y (0.0-9.9)

**Teclado Matricial**:
- Varredura sequencial pode introduzir latência
- Detecção depende de timing preciso de ativação/leitura
- Sem suporte para múltiplas teclas pressionadas simultaneamente

**Memória**:
- Pilha limitada a 10 elementos
- Alocação estática para todas as estruturas de dados
- Sem gerenciamento dinâmico de memória

### Otimizações Implementadas

**Eficiência de Varredura**:
- Algoritmo não-bloqueante permite responsividade
- Transição eficiente entre linhas via deslocamento bit-a-bit
- Reset circular sem overhead de verificação complexa

**Cálculos Matemáticos**:
- Uso de aritmética inteira para todas as operações
- Multiplicação por 10 para simular precisão decimal
- Algoritmos otimizados para evitar operações custosas

**Gerenciamento de Recursos**:
- Uso conservativo de registradores salvos
- Protocolo consistente de preservação de estado
- Alocação precisa de espaço de stack para recursão
