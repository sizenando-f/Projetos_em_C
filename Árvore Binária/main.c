#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct TreeNode{
  int key;
  struct TreeNode *left;
  struct TreeNode *right;
};

struct s_arq_no{
  int32_t chave:30;
  uint32_t esq:1;
  uint32_t dir:1;
};

void preOrderWay(struct TreeNode *rootNode){
  printf("-> %d ", rootNode->key);

  if(rootNode->left != NULL){
    preOrderWay(rootNode->left);
  }
  if(rootNode->right != NULL){
    preOrderWay(rootNode->right);
  }
}

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

struct TreeNode* findMin(struct TreeNode *node){
  while(node->left != NULL){
    node = node->left;
  }

  return node;
}

struct TreeNode* findMax(struct TreeNode *node){
  while(node->right != NULL){
    node = node->right;
  }

  return node;
}

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

void saveTree(struct TreeNode *rootNode){
  remove("binarytree.bin");
  FILE * fp = fopen("binarytree.bin", "ab");

  if(fp == NULL){
    printf("Erro na abertura do arquivo!\n");
    exit(100);
  }

  preOrderWaySave(rootNode, fp);
  fclose(fp);
}

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

void loadTree(struct TreeNode **rootNode){
  FILE * fp = fopen("binarytree.bin", "rb");

  if(fp == NULL){
    printf("Erro na abertura do arquivo!\n");
    exit(100);
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