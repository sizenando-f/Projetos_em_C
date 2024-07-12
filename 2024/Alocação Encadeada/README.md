# Alocação Encadeada

Este código contém estruturas de dados básicas e suas operações correspondentes. Cada estrutura de dados é manipulada através de menus interativos que permitem inserção, remoção, busca e visualização dos elementos, além de uma representação gráfica simples usando a biblioteca `gfx.h`.

## Estruturas Implementadas

### Lista Simplesmente Encadeada Ordenada Sem Nó Cabeça (SEO)

Uma lista simplesmente encadeada onde os elementos são inseridos mantendo a ordem crescente.

- **Funções Implementadas:**
  - `void insereSEO(int x, struct Lista **L)`: Insere um elemento na lista mantendo a ordem.
  - `void removeSEO(struct Lista **inicio, int num)`: Remove um elemento da lista.
  - `int retornaIndice(struct Lista **L, int valor)`: Retorna o índice do elemento na lista.
  - Outras funções auxiliares para busca e manipulação.

### Lista Circular Simplesmente Encadeada Ordenada Sem Nó Cabeça (CSEO)

Uma lista circular simplesmente encadeada onde os elementos são inseridos mantendo a ordem crescente.

- **Funções Implementadas:**
  - `void insereCSEO(struct Lista **inicio, int num)`: Insere um elemento na lista mantendo a ordem.
  - `void removeCSEO(struct Lista **inicio, int num)`: Remove um elemento da lista.
  - `int retornaIndiceCSEO(struct Lista **L, int valor)`: Retorna o índice do elemento na lista.
  - Outras funções auxiliares para busca e manipulação.

### Lista Duplamente Encadeada Com Nó Cabeça (DENC)

Uma lista duplamente encadeada com nó cabeça onde os elementos são manipulados de forma similar às outras listas.

- **Funções Implementadas:**
  - `void inicializaDENC(struct ListaDup **L)`: Inicializa a lista duplamente encadeada com nó cabeça.
  - `void insereDENC(struct ListaDup **L, int x)`: Insere um elemento no final da lista.
  - `void removeDENC(struct ListaDup **L, int num)`: Remove um elemento da lista.
  - Funções auxiliares para busca, desalocação e manipulação.

### Pilha (LIFO - Last In First Out)

Uma implementação básica de pilha usando um array em C.

- **Funções Implementadas:**
  - `void insereLIFO(int pilha[], int n, int *topo, int valor)`: Insere um elemento na pilha.
  - `void removeLIFO(int *topo)`: Remove um elemento da pilha.

### Fila (FIFO - First In First Out)

Uma implementação básica de fila usando um array em C.

- **Funções Implementadas:**
  - `void insereFIFO(int fila[], int n, int *fim, int valor)`: Insere um elemento na fila.
  - `void removeFIFO(int fila[], int *fim)`: Remove um elemento da fila.

## Como Executar

1. Compile o código utilizando um compilador C compatível.
2. Execute o programa gerado.
3. Escolha a estrutura de dados desejada no menu inicial.
4. Interaja com o menu para realizar operações como inserção, remoção, busca e visualização dos elementos.

## Requisitos

- **gcc** ou outro compilador C compatível.
- **gfx.h**: Biblioteca gráfica para visualização simples dos elementos inseridos nas estruturas.
