#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_MAX 256

// Estrutura do nó da árvore de Huffman
struct No{
  char dado;
  unsigned frequencia;
  struct No *esq, *dir;
};

// Estrutura do heap mínimo
struct HeapMinimo{
  unsigned tamanho;
  unsigned capacidade;
  struct No **array;
};

/**
 * @brief Cria novo nó. 
 * 
 * Insere o caractere e sua frequência. Inicializa os nós direito e esquerdo como NULL
 * @param dado Caractere a ser guardado
 * @param frequencia Frequência do caractere
 * @return struct No*: Ponteiro para nó criado
 */
struct No* novoNo(char dado, unsigned frequencia){
  struct No* no = (struct No*) malloc(sizeof(struct No));
  no->dado = dado;
  no->frequencia = frequencia;
  no->esq = no->dir = NULL;
  return no;
}

/**
 * @brief Cria um heap mínimo.
 * 
 * Inicializa tamanho com 0. Aloca dinamicamente um vetor com o tamanho da capacidade informada.
 * @param capacidade Tamanho do heap
 * @return struct HeapMinimo*: Ponteiro para a heap criada
 */
struct HeapMinimo* criaHeapMinimo(unsigned capacidade){
  struct HeapMinimo* heap = (struct HeapMinimo*) malloc(sizeof(struct HeapMinimo));
  heap->tamanho = 0;
  heap->capacidade = capacidade;
  heap->array = (struct No**) malloc(heap->capacidade * sizeof(struct No*));
  return heap;
}

/**
 * @brief Troca dois nós
 * 
 * @param no1 Endereço para primeiro nó
 * @param no2 Endereço para segundo nó
 */
void trocaNos(struct No** no1, struct No** no2){
  struct No* temp = *no1;
  *no1 = *no2;
  *no2 = temp;
}

/**
 * @brief Mantém a propriedade de um heap mínimo
 * 
 * Compara qual o nó com menor frequência e altera os nós comparados caso positivo
 * @param heapMinimo Heap para ser ajustada
 * @param indice Índice do nó a ser operado
 */
void corrigeHeap(struct HeapMinimo* heapMinimo, int indice){
  int menor = indice;
  unsigned filhoEsquerdo = 2 * indice + 1;
  unsigned filhoDireito = 2 * indice + 2;

  if(filhoEsquerdo < heapMinimo->tamanho && heapMinimo->array[filhoEsquerdo]->frequencia < heapMinimo->array[menor]->frequencia){
    menor = filhoEsquerdo;
  }

  if(filhoDireito < heapMinimo->tamanho && heapMinimo->array[filhoDireito]->frequencia < heapMinimo->array[menor]->frequencia){
    menor = filhoDireito;
  }

  if(menor != indice){
    trocaNos(&heapMinimo->array[menor], &heapMinimo->array[indice]);
    corrigeHeap(heapMinimo, menor);
  }
}

/**
 * @brief Verifica se o tamanho da heap é um
 * 
 * @param heapMinimo Heap a ser verificado
 * @return 1: Tamanho igual a um. 0: Tamanho diferente de um
 */
int tamanhoUm(struct HeapMinimo* heapMinimo){
  return (heapMinimo->tamanho == 1);
}

/**
 * @brief Remove o nó com menor frequência da heap
 * 
 * @param heapMinimo 
 * @return struct No*: Ponteiro para o nó extraído
 */
struct No* extraiMinimo(struct HeapMinimo* heapMinimo){
  struct No* noExtraido = heapMinimo->array[0];
  heapMinimo->array[0] = heapMinimo->array[heapMinimo->tamanho - 1];
  --heapMinimo->tamanho;

  corrigeHeap(heapMinimo, 0);
  return noExtraido;
}

/**
 * @brief Insere nó na heap.
 * 
 * Aumenta o tamanho da heap, insere o nó no final da heap e "sobe" o nó até que a frequência do nó inserido seja maior que seu pai ou que acabe a heap.
 * @param heapMinimo Heap onde será inserido o nó
 * @param no Nó a ser inserido
 */
void insereHeapMinimo(struct HeapMinimo* heapMinimo, struct No* no){
  ++heapMinimo->tamanho;
  int i = heapMinimo->tamanho - 1;
  
  // (i - 1) / 2 = índice do nó pai no índice i
  while(i && no->frequencia < heapMinimo->array[(i - 1) / 2]->frequencia){
    heapMinimo->array[i] = heapMinimo->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }

  heapMinimo->array[i] = no;
}

/**
 * @brief Constrói heap a partir dos dados
 * 
 * @param heapMinimo Heap a ser construído
 */
void constroiHeapMinimo(struct HeapMinimo* heapMinimo){
  int n = heapMinimo->tamanho - 1;
  for(int i = (n - 1) / 2; i >= 0; --i){
    corrigeHeap(heapMinimo, i);
  }
}

/**
 * @brief Verifica se nó é folha
 * 
 * @param no Nó a ser verificado
 * @return 1: É nó folha. 0: Não é nó folha
 */
int eFolha(struct No* no){
  return !(no->esq) && !(no->dir);
}

/**
 * @brief Cria e constrói um heap mínimo.
 * 
 * Cria um heap e insere em seu vetor nós contendo seu caractere e sua frequência respectiva. Constrói o heap com os dados inseridos.
 * @param dados Vetor de caracteres
 * @param frequencia Frequência de cada caractere respectivo
 * @param tamanho Tamanho do vetor
 * @return struct HeapMinimo*: Ponteiro para heap criada
 */
struct HeapMinimo* criaEConstroiHeapMinimo(char dados[], int frequencia[], int tamanho){
  struct HeapMinimo* heapMinimo = criaHeapMinimo(tamanho);

  for(int i = 0; i < tamanho; i++){
    heapMinimo->array[i] = novoNo(dados[i], frequencia[i]);
  }

  heapMinimo->tamanho = tamanho;
  constroiHeapMinimo(heapMinimo);
  return heapMinimo;
}

/**
 * @brief Contrói a árvore de Huffman
 * 
 * Cria e constrói o heap minimo e cria nós interno contendo a soma das frequências dos nós com menores frequências do heap.
 * @param dados Vetor dos caracteres
 * @param frequencia Frequência de cada caractere respectivo
 * @param tamanho Tamanho do vetor
 * @return struct No*: Ponteiro para raíz da árvore de Huffman
 */
struct No* constroiArvoreDeHuffman(char dados[], int frequencia[], int tamanho){
  struct No *esq, *dir, *interno;
  struct HeapMinimo *heapMinimo = criaEConstroiHeapMinimo(dados, frequencia, tamanho);

  while(!tamanhoUm(heapMinimo)){
    esq = extraiMinimo(heapMinimo);
    dir = extraiMinimo(heapMinimo);
    interno = novoNo('-', esq->frequencia + dir->frequencia);
    interno->esq = esq;
    interno->dir = dir;
    insereHeapMinimo(heapMinimo, interno);
  }

  return extraiMinimo(heapMinimo);
}

/**
 * @brief Armazena o código de Huffman em um vetor.
 * 
 * Se percorrer o caminho da esquerda adiciona 0, caso contrário adiciona 1 ao vetor que armazena o caminho percorrido. No final se for um nó folha, armazena esse caminho como um código binário para representar esse caractere
 * @param raiz Nó raíz da árvore
 * @param caminho Vetor para armazenar o caminho percorrido
 * @param topo Índice atual do vetor
 * @param codigos Códigos binarios respectivos para cada caractere
 * @param tamCodigo Tamanho de cada código
 */
void armazenaCodigo(struct No* raiz, int caminho[], int topo, int codigos[][TAMANHO_MAX], int* tamCodigo){
  if(raiz->esq){
    caminho[topo] = 0;
    armazenaCodigo(raiz->esq, caminho, topo + 1, codigos, tamCodigo);
  }

  if(raiz->dir){
    caminho[topo] = 1;
    armazenaCodigo(raiz->dir, caminho, topo + 1, codigos, tamCodigo);
  }

  if(eFolha(raiz)){
    int indice = (unsigned char)raiz->dado;
    tamCodigo[indice] = topo;
    for(int i = 0; i < topo; i++){
      codigos[indice][i] = caminho[i];
    }
  }
}

/**
 * @brief Grava a árvore de Huffman no arquivo
 * 
 * É usado 1 para indicar que é uma folha e então é seguido pelo caractere respectivo, e usado 0 para representar um nó interno.
 * @param raiz Raíz da árvore de Huffman 
 * @param arquivoSaida Arquivo onde será gravado a árvore
 */
void escreveArvore(struct No* raiz, FILE* arquivoSaida){
  if(!raiz){
    return;
  }

  if(eFolha(raiz)){
    fputc('1', arquivoSaida);
    fputc(raiz->dado, arquivoSaida);
  } else {
    fputc('0', arquivoSaida);
  }

  escreveArvore(raiz->esq, arquivoSaida);
  escreveArvore(raiz->dir, arquivoSaida);
}

/**
 * @brief Lê árvore de Huffman do arquivo
 * 
 * Se for folha '1', apenas cria o novo nó com seu caractere, se for nó inteno '0', insere de forma recursiva seus nós filhos.
 * @param arquivoEntrada Arquivo onde será feito a leitura
 * @return struct No*: Ponteiro para raíz da árvore de Huffman
 */
struct No* leArvore(FILE* arquivoEntrada){
  int bit = fgetc(arquivoEntrada);

  if(bit == '1'){
    char dado = fgetc(arquivoEntrada);
    return novoNo(dado, 0);
  } else {
    struct No* no = novoNo('-', 0);
    no->esq = leArvore(arquivoEntrada);
    no->dir = leArvore(arquivoEntrada);
    return no;
  }
}

/**
 * @brief Comprime o arquivo desejado.
 * 
 * Abre o arquivo recebido. Calcula a frequência de caracteres. Constrói a árvore de Huffman, gera os códigos e grava a árvore gerada no arquivo. Grava os dados compactados.
 * @param nomeArquivoEntrada Nome do arquivo a ser compactado
 */
void comprimirArquivo(const char* nomeArquivoEntrada){
  FILE *arquivoEntrada = fopen(nomeArquivoEntrada, "rb");
  if(!arquivoEntrada){
    printf("Erro ao abrir o arquivo de entrada!\n");
    return;
  }

  // Calcula a frequência de cada caractere usando seu valor ASCII como índice no vetor
  int frequencia[256] = {0};
  int c;
  while((c = fgetc(arquivoEntrada)) != EOF){
    frequencia[(unsigned char)c]++;
  }

  char dado[256];
  int listaDeFrequencia[256];
  int tamanho = 0;
  for(int i = 0; i < 256; i++){
    if(frequencia[i] > 0){
      dado[tamanho] = i;
      listaDeFrequencia[tamanho] = frequencia[i];
      tamanho++;
    }
  }

  struct No* raiz = constroiArvoreDeHuffman(dado, listaDeFrequencia, tamanho);

  int codigos[256][TAMANHO_MAX], tamCodigo[256] = {0};
  int caminho[TAMANHO_MAX];
  armazenaCodigo(raiz, caminho, 0, codigos, tamCodigo);

  char nomeArquivoSaida[256];
  snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s.cmp", nomeArquivoEntrada);
  FILE* arquivoSaida = fopen(nomeArquivoSaida, "wb");

  if(!arquivoSaida){
    printf("Erro ao criar o arquivo se saída!\n");
    fclose(arquivoEntrada);
    return;
  }

  // Escreve a árvore de Huffman no arquivo
  escreveArvore(raiz, arquivoSaida);

  // Retorna no ínicio do arquivo para gravar os dados compactados
  rewind(arquivoEntrada);

  int bitBuffer, bitContador = 0;
  while((c = fgetc(arquivoEntrada)) != EOF){
    unsigned char caractere = (unsigned char)c;
    for(int i = 0; i < tamCodigo[caractere]; i++){
      bitBuffer = (bitBuffer << 1) | codigos[caractere][i]; // Cria um 'espaço' no buffer e insere '1' se necessário
      bitContador++;

      // Quando completa 1 byte, insere no arquivo de saída
      if(bitContador == 8){ 
        fputc(bitBuffer, arquivoSaida);
        bitBuffer = 0;
        bitContador = 0;
      }
    }
  }

  // Para caso sobrar bits no buffer, grava o último byte
  if(bitContador > 0){
    bitBuffer <<= (8 - bitContador); // Insere zeros na direita do byte
    fputc(bitBuffer, arquivoSaida);
  }

  fclose(arquivoEntrada);
  fclose(arquivoSaida);

  printf("Arquivo '%s' foi compactado com sucesso!\n", nomeArquivoSaida);
}

/**
 * @brief Descomprime o arquivo desejado
 * 
 * Lê a árvore de Huffman navegando por ela ao ler 0 (esquerda) e 1 (direita) e, ao alcançar um nó folha, grava o caractere no arquivo de saída. Lendo byte por byte indo do bit mais signficativo para o menos significativo. 
 * @param nomeArquivoEntrada Nome do arquivo para ser descomprimido
 */
void descomprimeArquivo(const char* nomeArquivoEntrada){
  FILE* arquivoEntrada = fopen(nomeArquivoEntrada, "rb");
  if(!arquivoEntrada){
    printf("Erro ao abrir o arquivo de entrada!\n");
    return;
  }

  struct No* raiz = leArvore(arquivoEntrada);
  struct No* noAtual = raiz;

  char nomeArquivoSaida[256];
  snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s.dcmp", nomeArquivoEntrada);
  FILE* arquivoSaida = fopen(nomeArquivoSaida, "wb");

  if(!arquivoSaida){
    printf("Erro ao criar o arquivo de saida!\n");
    fclose(arquivoEntrada);
    return;
  }

  int byte;
  while((byte = fgetc(arquivoEntrada)) != EOF){
    for(int i = 7; i >= 0; i--){
      int bit = (byte >> i) & 1; // Pega o bit mais significativo

      if(bit == 0){
        noAtual = noAtual->esq;
      } else {
        noAtual = noAtual->dir;
      }

      // Grava o caractere no arquivo se for folha
      if(eFolha(noAtual)){
        fputc(noAtual->dado, arquivoSaida);
        noAtual = raiz; // Reinicia o caminho na árvore
      }
    }
  }

  fclose(arquivoEntrada);
  fclose(arquivoSaida);

  printf("O arquivo '%s' foi decompactado com sucesso!\n", nomeArquivoSaida);
}

int main(int argc, char *argv[]){
  if(argc != 3){
    printf("Uso: %s <c|d> <arquivo>\n", argv[0]);
    return 1;
  }

  if(strcmp(argv[1], "c") == 0){
    printf("Comprimindo arquivo...\n");
    comprimirArquivo(argv[2]);
  } else if(strcmp(argv[1], "d") == 0){
    printf("Descomprimindo arquivo...\n");
    descomprimeArquivo(argv[2]);
  } else{
    printf("Opcao invalida. Use 'c' para compactar ou 'd' para descompactar.\n");
  }

  return 0;
}