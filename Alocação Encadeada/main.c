#include "gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */
#include <stdlib.h>
#include <string.h>

// SEO = Simplesmente Encadeada Sem Nó Cabeça Ordenado
// CSEO = Circular Simplesmente Encadeada Sem Nó Cabeça Ordenado
// DENC = Duplamente Encadeada com Nó Cabeça
// LIFO = Last In First Out (Pilha)
// FIFO = First In FIrst Out (Fila)

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

void insereLIFO(int pilha[], int n, int *topo, int valor){
  if((*topo) != n){
    (*topo) = (*topo) + 1;
    pilha[(*topo)] = valor;
    return;
  }
  printf("A pilha esta cheia!\n");
}

void removeLIFO(int *topo){
  if((*topo) != 0){
    (*topo) = (*topo) - 1;
    return;
  }
  printf("A pilha esta vazia!\n");
}

void insereFIFO(int fila[], int n, int *fim, int valor){
  if((*fim) == n-1){
    printf("Fila esta cheia!\n");
    return;
  } else {
    (*fim) = (*fim) + 1;
    fila[(*fim)] = valor;
  }
}

void removeFIFO(int fila[], int *fim){
  for(int i = 0; i < (*fim); i++){
    fila[i] = fila[i+1];
  }
  (*fim) = (*fim) - 1;
}

void menuSEO(){
	struct Lista *L = NULL;
  	struct Lista *p;
	int esc, valor, cont;
	char num[100] = "";
	do{
		int x1 = 10, x2 = 60, y = 18;
		cont = 0;
		printf(" - Lista Simplesmente Encadeada Ordenada Sem No Cabeca -\n");
		printf("[ 1 ] Insercao\n");
		printf("[ 2 ] Remocao\n");
		printf("[ 3 ] Busca\n");
		printf("[ 4 ] Sair\n");
		printf("Selecione a operacao > ");
		scanf("%d", &esc);
		switch(esc){
			case 1:
				printf("Insira o valor do no >");
				scanf("%d", &valor);
				getchar();
				insereSEO(valor, &L);
				break;
			default:
				break;
		}
		gfx_quit();
		gfx_init(800, 300, "Lista Simplesmente Encadeada Ordenada Sem No Cabeca");
		p = L;
		while(p != NULL){
			sprintf(num, "%d", p->num);
			gfx_rectangle(x1, 10, x2, 40);
			gfx_text(x1+((x2-x1)/2)-2, y, num);
			if(cont){
				gfx_line(x1, 25, x1 - 20, 25);
			}
			gfx_paint();
			x1 = x2 + 20;
			x2 = x1 + 50;
			p = p->prox;
			cont = 1;
		}
	}while(esc != 4);
	gfx_quit();

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

  // Uso do LIFO
  // int n = 10, pilha[n], topo = -1;
  // for(int i = 0; i <= topo; i++){
  //   printf("%d ", pilha[i]);
  // }
  
  // Uso do FIFO
  // int n = 10, fila[n], fim = -1;
  // for(int i = 0; i <= fim; i++){
  //   printf("%d ", fila[i]);
  // }

	int esc;
	printf("		-- MENU --\n");
	printf("[ 1 ] Lista Simplesmente Encadeada Ordenada Sem No Cabeca\n");
	printf("[ 2 ] Lista Circular Simplesmente Encadeada Ordenada Sem No Cabeca\n");
	printf("[ 3 ] Duplamente Encadeada Com No Cabeca\n");
	printf("[ 4 ] Fila\n");
	printf("[ 5 ] Pilha\n");
	printf("Qual estrutura deseja usar? > ");
	scanf("%d", &esc);
	switch(esc){
		case 1:
			menuSEO();
			break;
		default:
			break;
	}

  return 0;
}