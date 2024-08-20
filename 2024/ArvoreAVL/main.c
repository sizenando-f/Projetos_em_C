#include <stdio.h>
#include <stdlib.h>

struct No {
    int chave;
    int bal;
    struct No *esq;
    struct No *dir;
};

void inicioNo(struct No **ptr, int x) {
    (*ptr) = (struct No*) malloc(sizeof(struct No));
    (*ptr)->chave = x;
    (*ptr)->dir = NULL;
    (*ptr)->esq = NULL;
    (*ptr)->bal = 0;
}

void caso1(struct No **pt, int *h) {
    struct No *ptu = (*pt)->esq;
    struct No *ptv;

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

void caso2(struct No **pt, int *h) {
    struct No *ptu = (*pt)->dir;
    struct No *ptv;

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

void insAVL(int x, struct No **ptr, int *h) {
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

void desaloca(struct No **raiz) {
    if ((*raiz)->esq != NULL) {
        desaloca(&(*raiz)->esq);
    }
    if ((*raiz)->dir != NULL) {
        desaloca(&(*raiz)->dir);
    }
    free((*raiz));
    (*raiz) = NULL;
}

void exibe(struct No *raiz) {
    if (raiz != NULL) {
        printf("%d -> ", raiz->chave);
        exibe(raiz->esq);
        exibe(raiz->dir);
    }
}

void removeAVL(int x, struct No **ptr, int *h) {
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
        struct No *aux;
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


int main() {
    struct No *raiz = NULL;
    int h = 0;

    printf("Inserindo 10...\n");
    insAVL(10, &raiz, &h);

    printf("Inserindo 2...\n");
    insAVL(2, &raiz, &h);

    printf("Inserindo 5...\n");
    insAVL(5, &raiz, &h);

    printf("Removendo 10...\n");
    removeAVL(10, &raiz, &h);

    printf("Removendo 5...\n");
    removeAVL(5, &raiz, &h);

    printf("Removendo 2...\n");
    removeAVL(2, &raiz, &h);

    printf("Arvore AVL: \n");
    exibe(raiz);
    printf("\n");

    desaloca(&raiz);

    return 0;
}
