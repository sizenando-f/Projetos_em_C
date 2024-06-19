#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Estrura básica da árvore binária
struct TreeNode{
  int key;
  struct TreeNode *left;
  struct TreeNode *right;
};

// Estrutura de carregamento e escrita da árvore binária
struct s_arq_no{
  int32_t chave:30;
  uint32_t esq:1;
  uint32_t dir:1;
};

/** @brief Exibe a árvore em pré-ordem
 * Esse procedimento exibe cada elemento da árvore binária em pré-ordem
 * @param rootNode Cópia do ponteiro para nó raíz da árvore binária
*/
void preOrderWay(struct TreeNode *rootNode){
  printf("-> %d ", rootNode->key);

  if(rootNode->left != NULL){
    preOrderWay(rootNode->left);
  }
  if(rootNode->right != NULL){
    preOrderWay(rootNode->right);
  }
}

/**
 * @brief Salva a árvore binária em um arquivo.
 * Esse procedimento auxiliar grava a árvore em pré-ordem, montando um nó em estrutura `s_arq_no` e gravando no arquivo desejado,
 * retornando nulo caso não exista uma árvore para ser salva
 * @param rootNode Cópia do ponteiro para o nó raiz da árvore binária
 * @param fp Arquivo já aberto onde será salvo a árvore 
*/
void preOrderWaySave(struct TreeNode *rootNode, FILE *fp){
  if(rootNode == NULL) return;

  struct s_arq_no node;
  node.chave = (rootNode)->key;
  node.esq = (rootNode->left != NULL) ? 1 : 0;
  node.dir = (rootNode->right != NULL) ? 1 : 0;


  fwrite(&node, sizeof(node), 1, fp);

  if(rootNode->left != NULL){
    preOrderWaySave(rootNode->left, fp);
  }
  if(rootNode->right != NULL){
    preOrderWaySave(rootNode->right, fp);
  }
}

/** @brief Procura por um nó na árvore binária.\n
 * Esse procedimento além de procurar por um nó, armazena em `feedback` como a árvore pode ser tratada.
 * - 0: Árvore vazia
 * - 1: Nó encontrado
 * - 2: Subárvore esquerda está vazia
 * - 3: Subárvore direita está vazia
 * @param x Valor da chave do nó a ser encontrado
 * @param ptr Ponteiro para a raíz da árvore
 * @param feedback Variável onde será armazenado o status da árvore
 * @returns Ponteiro apontado para nó encontrado e feedback da árvore
*/
void searchTree(int x, struct TreeNode **ptr, int *feedback){
  if((*ptr) == NULL){
    *feedback = 0;
  } else if(x == (*ptr)->key){
    *feedback = 1;
  } else if(x < (*ptr)->key) {
    if((*ptr)->left == NULL){
      *feedback = 2;
    } else {
      (*ptr) = (*ptr)->left;
      searchTree(x, ptr, feedback);
    }
  } else if((*ptr)->right == NULL){
    *feedback = 3;
  } else {
    (*ptr) = (*ptr)->right;
    searchTree(x, ptr, feedback);
  }
}

/** @brief Insere um nó na árvore binária.
 * Esse procedimento procura pelo nó se ele já existe, caso contrário, um novo nó será alocado com a chave desejada,
 * recebendo inicialmente os ponteiros esquerdo e direitos como nulo, sendo colocado na subárvore vazia de acordo com o feedback.
 * @param x Valor da chave do nó a ser inserido
 * @param rootNode Ponteiro para nó raíz da árvore binária
*/
void insertTree(int x, struct TreeNode **rootNode){
  struct TreeNode *ptr = (*rootNode);
  int feedback;
  searchTree(x, &ptr, &feedback);
  if(feedback == 1){
    printf("Insercao invalida\n");
    return;
  }
  struct TreeNode *pt1 = (struct TreeNode*) malloc(sizeof(struct TreeNode));
  pt1->key = x;
  pt1->left = NULL;
  pt1->right = NULL;
  if(feedback == 0){
    (*rootNode) = pt1;
  } else if(feedback == 2){
    ptr->left = pt1;
  } else {
    ptr->right = pt1;
  }
}

/**
 * @brief Encontra o menor nó da árvore.
 * Essa função avança para as subárvores esquerdas até não existir mais nenhuma.
 * @param node Cópia do ponteiro para o nó inicial da procura
 * @return Menor nó encontrado
 */
struct TreeNode* findMin(struct TreeNode *node){
  while(node->left != NULL){
    node = node->left;
  }

  return node;
}

/**
 * @brief Encontra o maior nó da árvore.
 * Essa função avança para as subárvores direitas até não existir mais nenhuma.
 * @param node Cópia do ponteiro para o nó inicial da procura
 * @return Maior nó encontrado
 */
struct TreeNode* findMax(struct TreeNode *node){
  while(node->right != NULL){
    node = node->right;
  }

  return node;
}

/**
 * @brief Encontra o sucessor de um nó desejado.
 * Essa função realiza a procura pelo sucessor desse nó, caso não exista, será printado que não existe. Caso exista, será feita
 * operações consecutivas até que o sucessor seja encontrado, retornando NULL caso não haja um sucessor.
 * @param rootNode Cópia do ponteiro para o nó raiz da árvore
 * @param x Chave do nó a ser usado como referência
 * @return Sucessor do nó passado como referência, caso exista.
 */
struct TreeNode* findSucessor(struct TreeNode *rootNode, int x){
  int feedback;
  struct TreeNode * node = rootNode;
  searchTree(x, &node, &feedback);
  if(feedback == 2 || feedback == 3){
    printf("O no nao existe. ");
    return NULL;
  }

  if(node->right != NULL){
    struct TreeNode *temp = findMin(node->right);
    return temp;
  }

  struct TreeNode * sucessor = NULL;
  while(rootNode != NULL){
    if(x < rootNode->key){
      sucessor = rootNode;
      rootNode = rootNode->left;
    } else if(x > rootNode->key){
      rootNode = rootNode->right;
    } else {
      break;
    }
  }
  return sucessor;
}

/**
 * @brief Encontra o predecessor de um nó desejado.
 * Essa função realiza a procura pelo predecessor desse nó, caso não exista, será printado que não existe. Caso exista, será feita
 * operações consecutivas até que o predecessor seja encontrado, retornando NULL caso não haja um predecessor.
 * @param rootNode Cópia do ponteiro para o nó raiz da árvore
 * @param x Chave do nó a ser usado como referência
 * @return Predecessor do nó passado como referência, caso exista.
 */
struct TreeNode* findPredecessor(struct TreeNode *rootNode, int x){
  int feedback;
  struct TreeNode * node = rootNode;
  searchTree(x, &node, &feedback);
  if(feedback == 2 || feedback == 3){
    printf("O no nao existe. ");
    return NULL;
  }

  if(node->left != NULL){
    struct TreeNode * temp = findMax(node->left);
    return temp;
  }

  struct TreeNode * predecessor = NULL;
  while(rootNode != NULL){
    if(x < rootNode->key){
      rootNode = rootNode->left;
    } else if(x > rootNode->key){
      predecessor = rootNode;
      rootNode = rootNode->right;
    } else {
      break;
    }
  }

  return predecessor;
}

/**
 * @brief Remove um nó da árvore binária.
 * Essa função remove um nó da árvore, caso ele exista, através de chamadas recursivas.
 * @param root Cópia do ponteiro para o nó raiz da árvore
 * @param key Chave do nó a ser excluído
 */
struct TreeNode* removeNode(struct TreeNode *root, int key){
  if(root == NULL) return NULL;

  if(key < root->key){
    root->left = removeNode(root->left, key);
  } else if (key > root->key){
    root->right = removeNode(root->right, key);
  } else {
    if(root->left == NULL){
      struct TreeNode *temp = root->right;
      free(root);
      return temp;
    } else if(root->right == NULL){
      struct TreeNode *temp = root->left;
      free(root);
      return temp;
    }

    struct TreeNode *temp = findMin(root->right);

    root->key = temp->key;

    root->right = removeNode(root->right, temp->key);
  }

  return root;
}

/**
 * @brief Salva a árvore binária.
 * Esse procedimento salva a árvore binária com o nome que o usuário desejar. Chamando uma função auxiliar `preOrderWaySave` para
 * gravar cada nó em pré-ordem
 * @param rootNode Cópia do nó raíz da árvore binária
 */
void saveTree(struct TreeNode *rootNode){
  char archiveName[100];
  printf("Insira o nome do arquivo que deseja salvar: ");
  fgets(archiveName, sizeof(archiveName), stdin);
  archiveName[strcspn(archiveName, "\n")] = '\0';

  FILE * fp = fopen(archiveName, "ab");

  if(fp == NULL){
    printf("Erro na abertura do arquivo!\n");
    exit(100);
  }

  preOrderWaySave(rootNode, fp);
  fclose(fp);
}

/**
 * @brief Carrega uma árvore binária em pré-ordem.
 * Esse procedimento carrega todos os nós em formato `s_arq_no` e converte para `TreeNode`, alocando nós e recriando toda a árvore
 * binária em pré-ordem cada um com seu respectivos filhos.
 * @param rootNodePerm Ponteiro para nó raiz que serve apenas para armazenar o primeiro nó da árvore, o nó raíz
 * @param rootNode Ponteiro referente ao nó atual da alocação
 * @param pastNode Ponteiro para o nó da chamada anterior
 * @param side Contém o valor 0, 1 ou 2 para saber em qual subárvore o novo nó será colocado, sendo 0 = nó raiz, 1 = lado esquerdo, 2 = lado direito
 * @param fp Arquivo já aberto da árvore a ser carregada
 */
void preOrderLoad(struct TreeNode **rootNodePerm, struct TreeNode **rootNode, struct TreeNode *pastNode, int side, FILE *fp){
  struct s_arq_no savedNode;
  if(fread(&savedNode, sizeof(savedNode), 1, fp) > 0){
    struct TreeNode *node = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    node->key = savedNode.chave;
    node->left = NULL;
    node->right = NULL;
    if(side == 0){
      (*rootNodePerm) = node;
    } else if(side == 1){
      pastNode->left = node;
    } else if(side == 2){
      pastNode->right = node;
    }

    (*rootNode) = node;
    if(savedNode.esq == 1){
      preOrderLoad(rootNodePerm, &node->left, node, 1, fp);
    }
    if(savedNode.dir == 1){
      preOrderLoad(rootNodePerm, &node->right, node, 2, fp);
    }
  }

}

/**
 * @brief Carrega uma árvore binária.
 * Esse procedimento chama a função auxiliar `preOrderLoad` para estruturar uma árvore binária já salva caso ela exista.
 * @param rootNode Ponteiro para receber o nó raíz da árvore carregada
 */
void loadTree(struct TreeNode **rootNode){
  char archiveName[100];
  printf("Insira o nome do arquivo que deseja abrir: ");
  fgets(archiveName, sizeof(archiveName), stdin);
  archiveName[strcspn(archiveName, "\n")] = '\0';

  FILE * fp = fopen(archiveName, "rb");

  if(fp == NULL){
    printf("Erro na abertura do arquivo!\n");
    return;
  } else {
    printf("Arquivo carregado com sucesso!\n");
  }

  rewind(fp);
  preOrderLoad(rootNode, rootNode, NULL, 0, fp);

  fclose(fp);
}

int main(){
  struct TreeNode *ptr = NULL;
  loadTree(&ptr);
  preOrderWay(ptr);

  return 0;
}