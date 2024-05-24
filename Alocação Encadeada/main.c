#include <stdio.h>
#include <stdlib.h>

// SEO = Simplesmente Encadeada Sem Nó Cabeça Ordenado
// CSEO = Circular Simplesmente Encadeada Sem Nó Cabeça Ordenado
// DENC = Duplamente Encadeada com Nó Cabeça

struct Lista{
  int num;
  struct Lista *prox;
};

struct ListaDup{
  int num;
  struct ListaDup *ant;
  struct ListaDup *prox;
};

// pont retorna NULL se não encontrou ou retorna o ponteiro do nó encontrado
void buscaSEO(int x, struct Lista **L, struct Lista **ant, struct Lista **pont){
  *ant = NULL;
  *pont = NULL;
  struct Lista *ptr = *L;
  while(ptr != NULL){
    if(ptr->num < x){
      *ant = ptr;
      ptr = ptr->prox;
    } else if(ptr->num == x){
      *pont = ptr;
      return;
    } else {
      ptr = NULL;
    }
  }
}

// Insere nó na lista simplesmente encadeada sem nó cabeça ordenado
void insereSEO(int x, struct Lista **L){
  struct Lista *ptr = malloc(sizeof(struct Lista));
  if((*L) == NULL){
    ptr->num = x;
    ptr->prox = *L;
    *L = ptr;
    return;
  }

  struct Lista *ant, *pont;
  buscaSEO(x, L, &ant, &pont);
  ptr->num = x;
  ptr->prox = ant->prox;
  ant->prox = ptr;
}

// Remove nó da lista simplesmente encadeada sem nó cabeça ordenado
void removeSEO(struct Lista **inicio, int num){
  struct Lista *ptr = *inicio;
  struct Lista *ant = NULL;

  while(ptr != NULL){
    if(ptr->num == num){
      if(ant == NULL){
        *inicio = ptr->prox;
      } else {
        ant->prox = ptr->prox;
      }
      free(ptr);
      return;
    }
    ant = ptr;
    ptr = ptr->prox;
  }
}

void buscaCSEO(int x, struct Lista **L, struct Lista **ant, struct Lista **pont){
  *ant = NULL;
  *pont = NULL;
  struct Lista *ptr = *L;

  do{
    if(ptr->num < x){
      *ant = ptr;
      ptr = ptr->prox;
    } else if(ptr->num == x){
      *pont = ptr;
      return;
    } else {
      ptr = (*L);
    }
  }while(ptr != (*L));
}

void insereCSEO(struct Lista **inicio, int num){
  struct Lista *no = (struct Lista *) malloc(sizeof(struct Lista));
  no->num = num;
  if((*inicio) == NULL){
    (*inicio) = no;
    no->prox = (*inicio);
    return;
  }

  struct Lista *ant, *pont;
  buscaCSEO(num, inicio, &ant, &pont);

  if(ant == NULL){
    no->prox = (*inicio);
    (*inicio)->prox = no;
    return;
  }

  no->prox = ant->prox;
  ant->prox = no;
}

void removeCSEO(struct Lista **inicio, int num){
  struct Lista *ptr = *inicio;
  struct Lista *ant = NULL;

  do{
    if(ptr->num == num){
      if(ant == NULL){
        struct Lista *temp = (*inicio);
        while(temp->prox != (*inicio)){
          temp = temp->prox;
        }
        *inicio = ptr->prox;
        temp->prox = (*inicio);
      } else {
        ant->prox = ptr->prox;
      }
      free(ptr);
      return;
    }
    ant = ptr;
    ptr = ptr->prox;
  }while(ptr != (*inicio));
}

void inicializaDENC(struct ListaDup **L){
  (*L) = (struct ListaDup *) malloc(sizeof(struct ListaDup));
  (*L)->ant = NULL;
  (*L)->prox = NULL;
  (*L)->num = 0;
}

void buscaDENC(struct ListaDup **L, int x, struct ListaDup **pont){
  struct ListaDup *ptr = (*L)->prox;
  while(ptr != NULL){
    if(ptr->num == x){
      (*pont) = ptr;
      return;
    }
    ptr = ptr->prox;
  }
}

void insereDENC(struct ListaDup **L, int x){
  struct ListaDup *ptr = (*L)->prox;
  
  if(ptr == NULL){
    struct ListaDup *p = (struct ListaDup *) malloc(sizeof(struct ListaDup));
    p->ant = (*L);
    p->num = x;
    p->prox = NULL;
    (*L)->prox = p;
    return;
  }

  while(ptr->prox != NULL){
    ptr = ptr->prox;
  }

  struct ListaDup *p = (struct ListaDup *) malloc(sizeof(struct ListaDup));
  p->ant = ptr;
  p->num = x;
  p->prox = NULL;
  ptr->prox = p;
}

void removeDENC(struct ListaDup **L, int num){
  struct ListaDup *ptr = (*L)->prox, *temp;
  while(ptr != NULL){
    if(ptr->num == num){
      temp = ptr;
      ptr->ant->prox  = ptr->prox;
      free(temp);
      return;
    }
    ptr = ptr->prox;
  }
}


int main(){
  // Uso da SEO
  // struct Lista *L = NULL;
  // struct Lista *p;
  // p = L;
  // while(p != NULL){
  //   printf("%d ", p->num);
  //   p = p->prox;
  // }

  // Uso do CSEO
  // struct Lista *L = NULL;
  // struct Lista *p;
  // p = L;
  // do{
  //   printf("%d ", p->num);
  //   p = p->prox;
  // }while(p != L);

  // Uso do DENC
  // struct ListaDup *L = NULL;
  // inicializaDENC(&L);
  // struct ListaDup *p;
  // p = L->prox;
  // while(p != NULL){
  //   printf("%d ", p->num);
  //   p = p->prox;
  // }

  return 0;
}