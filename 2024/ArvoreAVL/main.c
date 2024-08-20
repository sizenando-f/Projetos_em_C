#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct s_no {
    int32_t chave:28;
    int32_t bal:2;
    int32_t reservado:2; /*Não usar*/
    struct s_no *esq;
    struct s_no *dir;
};

struct s_arq_no{
  int32_t chave:28;
  int32_t bal:2;
  uint32_t esq:1;
  uint32_t dir:1;
};

// ! warning relacionado ao tamanho de bits entre x e ptr->chave
void inicioNo(struct s_no **ptr, int x) {
    (*ptr) = (struct s_no*) malloc(sizeof(struct s_no));
    (*ptr)->chave = (int32_t)x;
    (*ptr)->dir = NULL;
    (*ptr)->esq = NULL;
    (*ptr)->bal = 0;
}

void caso1(struct s_no **pt, int *h) {
    struct s_no *ptu = (*pt)->esq;
    struct s_no *ptv;

    if (ptu->bal == -1) {
        (*pt)->esq = ptu->dir;
        ptu->dir = (*pt);
        (*pt)->bal = 0;
        (*pt) = ptu;
    } else {
        ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = (*pt);
        if (ptv->bal == -1) {
            (*pt)->bal = 1;
        } else {
            (*pt)->bal = 0;
        }
        if (ptv->bal == 1) {
            ptu->bal = -1;
        } else {
            ptu->bal = 0;
        }
        (*pt) = ptv;
    }
    (*pt)->bal = 0;
    *h = 0;
}

void caso2(struct s_no **pt, int *h) {
    struct s_no *ptu = (*pt)->dir;
    struct s_no *ptv;

    if (ptu->bal == 1) {
        (*pt)->dir = ptu->esq;
        ptu->esq = (*pt);
        (*pt)->bal = 0;
        (*pt) = ptu;
    } else {
        ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = (*pt);
        if (ptv->bal == 1) {
            (*pt)->bal = -1;
        } else {
            (*pt)->bal = 0;
        }
        if (ptv->bal == -1) {
            ptu->bal = 1;
        } else {
            ptu->bal = 0;
        }
        (*pt) = ptv;
    }
    (*pt)->bal = 0;
    *h = 0;
}

void insAVL(int x, struct s_no **ptr, int *h) {
    if ((*ptr) == NULL) {
        inicioNo(&(*ptr), x);
        *h = 1;
        return;
    } else if (x == (*ptr)->chave) return;

    if (x < (*ptr)->chave) {
        insAVL(x, &(*ptr)->esq, h);
        if (*h) {
            switch ((*ptr)->bal) {
            case 1:
                (*ptr)->bal = 0; *h = 0;
                break;
            case 0:
                (*ptr)->bal = -1;
                break;
            case -1:
                caso1(&(*ptr), h);  // Rebalanceamento
                break;
            }
        }
    } else {
        insAVL(x, &(*ptr)->dir, h);
        if (*h) {
            switch ((*ptr)->bal) {
            case -1:
                (*ptr)->bal = 0; *h = 0;
                break;
            case 0:
                (*ptr)->bal = 1;
                break;
            case 1:
                caso2(&(*ptr), h);  // Rebalanceamento
                break;
            }
        }
    }
}

void desaloca(struct s_no **raiz) {
    if ((*raiz)->esq != NULL) {
        desaloca(&(*raiz)->esq);
    }
    if ((*raiz)->dir != NULL) {
        desaloca(&(*raiz)->dir);
    }
    free((*raiz));
    (*raiz) = NULL;
}

void exibe(struct s_no *raiz) {
    if (raiz != NULL) {
        printf("%d -> ", raiz->chave);
        exibe(raiz->esq);
        exibe(raiz->dir);
    }
}

void removeAVL(int x, struct s_no **ptr, int *h) {
    if (*ptr == NULL) {
        return;
    } else if (x < (*ptr)->chave) {
        removeAVL(x, &(*ptr)->esq, h);
        if (*h) {
            switch ((*ptr)->bal) {
                case -1:
                    (*ptr)->bal = 0;
                    break;
                case 0:
                    (*ptr)->bal = 1;
                    *h = 0;
                    break;
                case 1:
                    caso2(ptr, h);  // Rebalanceamento
                    break;
            }
        }
    } else if (x > (*ptr)->chave) {
        removeAVL(x, &(*ptr)->dir, h);
        if (*h) {
            switch ((*ptr)->bal) {
                case 1:
                    (*ptr)->bal = 0;
                    break;
                case 0:
                    (*ptr)->bal = -1;
                    *h = 0;
                    break;
                case -1:
                    caso1(ptr, h);  // Rebalanceamento
                    break;
            }
        }
    } else {  // Encontrou o nó a ser removido
        struct s_no *aux;
        if ((*ptr)->esq == NULL) {
            aux = *ptr;
            *ptr = (*ptr)->dir;
            free(aux);
            *h = 1;
        } else if ((*ptr)->dir == NULL) {
            aux = *ptr;
            *ptr = (*ptr)->esq;
            free(aux);
            *h = 1;
        } else {  // Nó com dois filhos
            aux = (*ptr)->dir;
            while (aux->esq != NULL) {
                aux = aux->esq;
            }
            (*ptr)->chave = aux->chave;
            removeAVL(aux->chave, &(*ptr)->dir, h);
            if (*h) {
                switch ((*ptr)->bal) {
                    case 1:
                        (*ptr)->bal = 0;
                        break;
                    case 0:
                        (*ptr)->bal = -1;
                        *h = 0;
                        break;
                    case -1:
                        caso1(ptr, h);  // Rebalanceamento
                        break;
                }
            }
        }
    }
}

void preOrderSave(struct s_no *raiz, FILE *fp){
  if(raiz == NULL) return;

  struct s_arq_no no;
  no.chave = raiz->chave;
  no.esq = raiz->esq != NULL ? 1 : 0;
  no.dir = raiz->dir != NULL ? 1 : 0;
  no.bal = raiz->bal;

  fwrite(&no, sizeof(no), 1, fp);

  if(raiz->esq != NULL){
    preOrderSave(raiz->esq, fp);
  }
  if(raiz->dir != NULL){
    preOrderSave(raiz->dir, fp);
  }
}

void saveTree(struct s_no *raiz){
  char nome_arquivo[100];
  printf("[ <- ] Insira o nome do arquivo que deseja salvar > ");
  fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
  nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';

  FILE * fp = fopen(nome_arquivo, "ab");

  if(fp == NULL){
    printf("[ ERRO -># ] Erro ao salvar o arquivo, tente novamente...\n");
    return;
  }

  preOrderSave(raiz, fp);
  printf("[ -> ] O arquivo foi salvo com sucesso!\n");
  fclose(fp);
}

// ! Checar
void preOrderLoad(struct s_no **raizPerm, struct s_no **raiz, struct s_no *noAnterior, int lado, FILE *fp){
  struct s_arq_no noSalvo;
  if(fread(&noSalvo, sizeof(noSalvo), 1, fp) > 0){
    struct s_no *no = (struct s_no*) malloc(sizeof(struct s_no));
    no->chave = noSalvo.chave;
    no->bal = noSalvo.bal;
    no->esq = NULL;
    no->dir = NULL;
    if(lado == 0){
      (*raizPerm) = no;
    } else if(lado == 1){
      noAnterior->esq = no;
    } else if(lado == 2){
      noAnterior->dir = no;
    }

    (*raiz) = no;
    if(noSalvo.esq == 1){
      preOrderLoad(raizPerm, &no->esq, no, 1, fp);
    }
    if(noSalvo.dir == 1){
      preOrderLoad(raizPerm, &no->dir, no, 2, fp);
    }
  }
}

// ! Checar
void loadTree(struct s_no **raiz){
  char nome_arquivo[100];
  printf("[ <- ] Insira o nome do arquivo que deseja abrir > ");
  fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
  nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';

  FILE * fp = fopen(nome_arquivo, "rb");

  if(fp == NULL){
    printf("[ ERRO -># ] Erro na abertura do arquivo, verifique se ele existe ou se o nome esta correto.\n");
    return;
  } else {
    printf("[ -> ] Arquivo carregado com sucesso!\n");
  }

  rewind(fp);
  preOrderLoad(raiz, raiz, NULL, 0, fp);

  fclose(fp);
}


int main() {
    struct s_no *raiz = NULL;
    int h = 0;

    // loadTree(&raiz);

    // printf("Inserindo 10...\n");
    // insAVL(10, &raiz, &h);

    // printf("Inserindo 2...\n");
    // insAVL(2, &raiz, &h);

    // printf("Inserindo 5...\n");
    // insAVL(5, &raiz, &h);

    // printf("Removendo 10...\n");
    // removeAVL(10, &raiz, &h);

    // printf("Removendo 5...\n");
    // removeAVL(5, &raiz, &h);

    // printf("Removendo 2...\n");
    // removeAVL(2, &raiz, &h);

    printf("Arvore AVL: \n");
    exibe(raiz);
    printf("\n");


    desaloca(&raiz);

    return 0;
}
