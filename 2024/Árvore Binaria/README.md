# Árvore Binária Visualizada Graficamente

Este programa em C utiliza a biblioteca `gfx.h` para visualizar graficamente uma árvore binária. Ele oferece funcionalidades para inserir nós na árvore, remover nós, buscar nós específicos, encontrar o sucessor e predecessor de um nó, além de salvar e carregar a árvore em arquivos binários.

## Requisitos

Para compilar e executar este programa, são necessárias as seguintes bibliotecas e ferramentas:

- **gcc**: compilador C para compilar o código.
- **gfx.h**: biblioteca gráfica para desenhar elementos na tela.
- **unistd.h, stdio.h, stdlib.h, stdint.h, string.h**: bibliotecas padrão do C para funcionalidades como manipulação de strings, alocação de memória, operações de arquivo, etc.

## Compilação

Para compilar o programa, utilize o seguinte comando:

```bash
gcc main.c gfx.c -o arvore_binaria -lm
```

## Execução

Após compilar, execute o programa com:

```bash
./arvore_binaria
```

## Funcionalidades

### 1. Inserir Nó na Árvore

Permite inserir um novo nó com uma chave específica na árvore binária. Se a chave já existir, a inserção não será realizada.

### 2. Remover Nó da Árvore

Remove um nó específico da árvore binária, mantendo a estrutura de árvore binária válida.

### 3. Buscar Nó na Árvore

Busca por um nó específico na árvore binária e destaca visualmente o nó encontrado.

### 4. Encontrar Menor Chave

Encontra e destaca visualmente o nó com a menor chave na árvore.

### 5. Encontrar Maior Chave

Encontra e destaca visualmente o nó com a maior chave na árvore.

### 6. Encontrar Sucessor de um Nó

Encontra e destaca visualmente o sucessor de um nó específico na árvore.

### 7. Encontrar Predecessor de um Nó

Encontra e destaca visualmente o predecessor de um nó específico na árvore.

### 8. Salvar Árvore em Arquivo

Salva a estrutura atual da árvore binária em um arquivo binário para uso posterior.

### 9. Carregar Árvore de Arquivo

Carrega uma estrutura de árvore binária previamente salva de um arquivo binário.

### 10. Visualizar Árvore

Exibe visualmente a estrutura atual da árvore binária na tela, utilizando a biblioteca gráfica `gfx.h`.

### 11. Limpar Árvore da Memória

Desaloca todos os nós da árvore binária da memória.

## Estruturas Utilizadas

- **struct TreeNode**: Define a estrutura básica de um nó na árvore binária, contendo uma chave inteira e ponteiros para os nós da esquerda e direita.
- **struct s_arq_no**: Estrutura utilizada para salvar e carregar a árvore binária em arquivos binários, otimizando o espaço com campos compactos para a chave e indicações de filhos.

## Como Usar

Ao iniciar o programa, você terá acesso a um menu interativo que oferece todas as funcionalidades mencionadas acima. Basta seguir as instruções na linha de comando para interagir com a árvore binária de forma gráfica e funcional.
