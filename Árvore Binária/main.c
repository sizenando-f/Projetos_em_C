#include <unistd.h> /* sleep  */
#include "gfx.h"
#include <stdio.h>  /* printf */
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

/** @brief Monta a ávore binária de forma gráfica.
 * Esse procedimento auxiliar exibe cada elemento da árvore binária em pré-ordem utilizando a biblioteca gfx
 * @param rootNode Cópia do ponteiro para nó raíz da árvore binária
 * @param x Valor em pixel para posição do nó no eixo X
 * @param y Valor em pixel para posição do nó no eixo Y
 * @param control Valor em pixel para controle de distância entre os nós
*/
void preOrderWay(struct TreeNode *rootNode, int x, int y, int control) {
    if (rootNode == NULL) return;

    char num[100] = "";
    sprintf(num, "%d", rootNode->key);

    int nodeWidth = 40;
    int nodeHeight = 30;

    gfx_set_color(137, 86, 36);
    gfx_filled_rectangle(x - nodeWidth / 2, y - nodeHeight / 2, x + nodeWidth / 2, y + nodeHeight / 2);
    gfx_set_color(255, 255, 255);
    gfx_rectangle(x - nodeWidth / 2, y - nodeHeight / 2, x + nodeWidth / 2, y + nodeHeight / 2);
    gfx_text(x - 10, y - 5, num);

    if (rootNode->left != NULL) {
        int leftX = x - control / 2;
        int leftY = y + 70;
        gfx_set_color(137, 86, 36);
        gfx_line(x, y, leftX, leftY);
        preOrderWay(rootNode->left, leftX, leftY, control / 2);
    }
    if (rootNode->right != NULL) {
        int rightX = x + control / 2;
        int rightY = y + 70;
        gfx_set_color(137, 86, 36);
        gfx_line(x, y, rightX, rightY);
        preOrderWay(rootNode->right, rightX, rightY, control / 2);
    }
}

/**
 *  @brief Exibe graficamente a árvore binária.
 *  Esse procedimento inicializa a interface gfx e utiliza o procedimento auxiliar `preOrderWay` para exibir graficamente a árvore binária
 *  @param rootNode Cópia do ponteiro para o nó raíz da árvore binária
*/
void printTree(struct TreeNode *rootNode) {
    gfx_quit();

    gfx_init(1200, 800, "Arvore Binaria");

    int control = 1200 / 4;

    preOrderWay(rootNode, 1200 / 2, 50, control);

    gfx_paint();
}

void preOrderWayHighlight(struct TreeNode *rootNode, int x, int y, int control, int key) {
    if (rootNode == NULL) return;

    char num[100] = "";
    sprintf(num, "%d", rootNode->key);

    int nodeWidth = 40;
    int nodeHeight = 30;
    if(rootNode->key == key){
      gfx_set_color(114, 58, 58);
    } else {
      gfx_set_color(137, 86, 36);
    }

    gfx_filled_rectangle(x - nodeWidth / 2, y - nodeHeight / 2, x + nodeWidth / 2, y + nodeHeight / 2);
    gfx_set_color(255, 255, 255);
    gfx_rectangle(x - nodeWidth / 2, y - nodeHeight / 2, x + nodeWidth / 2, y + nodeHeight / 2);
    gfx_text(x - 10, y - 5, num);

    if (rootNode->left != NULL) {
        int leftX = x - control / 2;
        int leftY = y + 70;
        gfx_set_color(137, 86, 36);
        gfx_line(x, y, leftX, leftY);
        preOrderWayHighlight(rootNode->left, leftX, leftY, control / 2, key);
    }
    if (rootNode->right != NULL) {
        int rightX = x + control / 2;
        int rightY = y + 70;
        gfx_set_color(137, 86, 36);
        gfx_line(x, y, rightX, rightY);
        preOrderWayHighlight(rootNode->right, rightX, rightY, control / 2, key);
    }
}

void printTreeHiglight(struct TreeNode *rootNode, int key) {
    gfx_quit();

    gfx_init(1200, 800, "Arvore Binaria");

    int control = 1200 / 4;

    preOrderWayHighlight(rootNode, 1200 / 2, 50, control, key);

    gfx_paint();
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
int insertTree(int x, struct TreeNode **rootNode){
  struct TreeNode *ptr = (*rootNode);
  int feedback;
  searchTree(x, &ptr, &feedback);
  if(feedback == 1){
    return 0;
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

  return 1;
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
 * @return Nó sucessor do nó passado como referência, caso exista.
 */
struct TreeNode* findSucessor(struct TreeNode *rootNode, int x){
  int feedback;
  struct TreeNode * node = rootNode;
  searchTree(x, &node, &feedback);
  if(feedback == 2 || feedback == 3){
    printf("O no nao existe.\n");
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
 * @return Nó predecessor do nó passado como referência, caso exista.
 */
struct TreeNode* findPredecessor(struct TreeNode *rootNode, int x){
  int feedback;
  struct TreeNode * node = rootNode;
  searchTree(x, &node, &feedback);
  if(feedback == 2 || feedback == 3){
    printf("O no nao existe.\n");
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
    if(root->left == NULL && root->right == NULL){
      free(root);
      return NULL;
    } else if(root->left == NULL){
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
  printf("[ <- ] Insira o nome do arquivo que deseja salvar: ");
  fgets(archiveName, sizeof(archiveName), stdin);
  archiveName[strcspn(archiveName, "\n")] = '\0';

  FILE * fp = fopen(archiveName, "ab");

  if(fp == NULL){
    printf("[ ERRO -># ] Erro na abertura do arquivo!\n");
    exit(100);
  }

  preOrderWaySave(rootNode, fp);
  printf("[ -> ] O arquivo foi salvo com sucesso!\n");
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
  printf("[ <- ] Insira o nome do arquivo que deseja abrir > ");
  fgets(archiveName, sizeof(archiveName), stdin);
  archiveName[strcspn(archiveName, "\n")] = '\0';

  FILE * fp = fopen(archiveName, "rb");///////

  if(fp == NULL){
    printf("[ ERRO -># ] Erro na abertura do arquivo, verifique se ele existe ou se o nome esta correto.\n");
    return;
  } else {
    printf("[ -> ] Arquivo carregado com sucesso!\n");
  }

  rewind(fp);
  preOrderLoad(rootNode, rootNode, NULL, 0, fp);

  fclose(fp);
}

/**
 * @brief Desaloca a árvore binária.
 * Esse procedimento utiliza do percurso pós-ordem para desalocar cada nó existente da ávore e insere NULL nos nós desalocados
 * @param rootNode Ponteiro para nó raiz da árvore binária
*/
void freeTree(struct TreeNode **rootNode){
	if((*rootNode)->left != NULL){
		freeTree(&(*rootNode)->left);
	}
	if((*rootNode)->right != NULL){
		freeTree(&(*rootNode)->right);
	}
	free((*rootNode));
	(*rootNode) = NULL;
}

/**
 * @brief Menu de manipulação da árvore binária.
 * Esse procedimento exibe o menu para ser realizado a inserção ou remoção de um nó na árvore binária.
 * @param rootNode Ponteiro para o nó raíz da árvore binária
*/
void manipulationMenu(struct TreeNode **rootNode){
  int esc, key;
  do{
    printf("--- PAINEL DE CONTROLE ---\n");
    printf("|1. INSERIR NO NA ARVORE |\n");
    printf("|2. REMOVER NO NA ARVORE |\n");
    printf("|3. VOLTAR PARA O MENU   |\n");
    printf("--------------------------\n");
    printf("[ <- ] O que deseja fazer? > ");
    scanf("%d", &esc);
    system("clear");
    switch (esc) {
      case 1:
        printf("[ <- ] Insira a chave do no a ser inserido > ");
        scanf("%d", &key);
        if(insertTree(key, rootNode)){
          printf("[ -> ] Insercao realiza com sucesso!\n");
        } else {
          printf("[ ERRO -># ] Chave invalida, tente novamente...\n");
        }
        printTree(*rootNode);
        break;
      case 2:
        if((*rootNode) == NULL){
          printf("[ ERRO -># ] A arvore esta vazia, insira um no primeiramente.\n");
          break;
        }
        printf("[ <- ] Insira a chave do no a ser removido > ");
        scanf("%d", &key);
        *rootNode = removeNode(*rootNode, key);
        printTree(*rootNode);
        break;
      case 3:
        break;
      default:
        printf("[ #<- ERRO ] Entrada invalida, tente novamente...\n");
        break;
    }
  }while(esc != 3);
}

/**
 * @brief Menu de busca de nós na árvore binária.
 * Esse procedimento exibe o menu para ser realizado a busca da menor ou maior chave, o sucessor e o predecessor, ou a busca de uma chave específica..
 * @param rootNode Cópia do ponteiro para o nó raíz da árvore binária
*/
void searchNodeMenu(struct TreeNode *rootNode){
  if(rootNode == NULL){
    printf("[ ERRO -># ] Crie ou carrege uma arvore binaria antes de acessar esse menu\n");
    return;
  }

  struct TreeNode *temp = rootNode;
  int esc, key, feedback;
  do{
    printf("---- BUSCA BINARIA ----\n");
    printf("|1. CHAVE ESPECIFICA  |\n");
    printf("|2. MENOR CHAVE       |\n");
    printf("|3. MAIOR CHAVE       |\n");
    printf("|4. SUCESSOR          |\n");
    printf("|5. PREDECESSOR       |\n");
    printf("|6. VOLTAR PARA MENU  |\n");
    printf("-----------------------\n");
    printf("[ <- ] O que deseja buscar? > ");
    scanf("%d", &esc);
    system("clear");
    switch (esc) {
      case 1:
        temp = rootNode;
        printf("[ <- ] Entre a chave do no a ser buscado > ");
        scanf("%d", &key);
        searchTree(key, &temp, &feedback);
        printf("\n\n%d\n\n", key);
        if(feedback == 2 || feedback == 3){
          printf("[ ERRO -># ] A chave buscada nao existe, tente novamente...\n");
        } else {
          printTreeHiglight(rootNode, temp->key);
          printf("[ -> ] A chave foi encontrada e destacada graficamente!\n");
        }
        break;
      case 2:
        temp = findMin(rootNode);
        printf("[ -> ] A menor chave descoberta: [%d]\n", temp->key);
        printTreeHiglight(rootNode, temp->key);
        break;
      case 3:
        temp = findMax(rootNode);
        printf("[ -> ] A maior chave descoberta: [%d]\n", temp->key);
        printTreeHiglight(rootNode, temp->key);
        break;
      case 4:
        printf("[ <- ] Voce deseja descobrir o sucessor de qual chave? > ");
        scanf("%d", &key);
        temp = findSucessor(rootNode, key);
        if(temp != NULL){
          printf("[ -> ] Sucessor de [%d]: [%d]\n", key, temp->key);
          printTreeHiglight(rootNode, temp->key);
        } else {
          printf("[ -> ] A chave [%d] nao possui sucessor!\n", key);
        }
        break;
      case 5:
        printf("[ <- ] Voce deseja descobrir o predecessor de qual chave? > ");
        scanf("%d", &key);
        temp = findPredecessor(rootNode, key);
        if(temp != NULL){
          printf("[ -> ] Predecessor de [%d]: [%d]\n", key, temp->key);
          printTreeHiglight(rootNode, temp->key);
        } else {
          printf("[ -> ] A chave [%d] nao possui predecessor!\n", key);
        }
        break;
      case 6:
        break;
      default:
        printf("[ #<- ERRO] Entrada invalida, tente novamente...\n");
        break;
    }
  }while(esc != 6);
}

int main(){
  struct TreeNode *rootNode = NULL;
  int esc;
  do{
    system("clear");
    printf("--- ARVORE BINARIA ---\n");
    printf("|1. MANIPULAR ARVORE |\n");
    printf("|2. BUSCAR NO        |\n");
    printf("|3. SALVAR ARVORE    |\n");
    printf("|4. CARREGAR ARVORE  |\n");
    printf("|5. SAIR             |\n");
    printf("----------------------\n");
    printf("[ <- ] O que deseja fazer? > ");
    scanf("%d", &esc);
    getchar();
    system("clear");
    switch (esc) {
      case 1:
        manipulationMenu(&rootNode);
        break;
      case 2:
        searchNodeMenu(rootNode);
        break;
      case 3:
        if(rootNode == NULL){
          printf("[ ERRO -># ] Primeiro crie uma arvore binaria antes de tentar salvar.\n");
        } else {
          saveTree(rootNode);
        }
        break;
      case 4:
        loadTree(&rootNode);
        printTree(rootNode);
        break;
      case 5:
        gfx_quit();
        freeTree(&rootNode);
        printf("[ -> ] A arvore foi desalocada!\n");
        break;
      default:
        break;
    }
  }while(esc != 5);
  
  return 0;
}