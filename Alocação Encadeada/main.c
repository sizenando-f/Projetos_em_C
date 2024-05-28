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

// Estrutura básica de uma lista
struct Lista{
  int num;
  struct Lista *prox;
};

// Estrutura básica da lista duplamente encadeada
struct ListaDup{
  int num;
  struct ListaDup *ant;
  struct ListaDup *prox;
};

// pont retorna NULL se não encontrou ou retorna o ponteiro do nó encontrado
void buscaSEO(int x, struct Lista **L, struct Lista **ant, struct Lista **pont){
  *ant = NULL;
  *pont = NULL;
  struct Lista *ptr = (*L);
  while(ptr != NULL){
    if(ptr->num < x){
      *ant = ptr;
      ptr = ptr->prox;
    } else if(ptr->num == x){
      *pont = ptr;
      return;
    } else {
		*pont = ptr;
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
  if(ant == NULL){
  	ptr->num = x;
  	ptr->prox = pont;
	(*L) = ptr;
	return;
  }
  ptr->num = x;
  ptr->prox = ant->prox;
  ant->prox = ptr;
}

// Remove nó da lista simplesmente encadeada sem nó cabeça ordenado
void removeSEO(struct Lista **inicio, int num){
	if((*inicio) == NULL) return;
  struct Lista *ptr = (*inicio);
  struct Lista *ant = NULL;

  while(ptr != NULL){
    if(ptr->num == num){
      if(ant == NULL){
        (*inicio) = ptr->prox;
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

// Retorna nó anterior do valor encontrado na lista circular e o ponteiro do valor
void buscaCSEO(int x, struct Lista **L, struct Lista **ant, struct Lista **pont) {
    (*ant) = NULL;
    (*pont) = NULL;
    struct Lista *ptr = (*L);

    do {
        if (ptr->num < x) {
            (*ant) = ptr;
            ptr = ptr->prox;
        } else if (ptr->num == x) {
            (*pont) = ptr;
            return;
        } else {
            (*pont) = ptr;
            ptr = (*L);
        }
    } while (ptr != (*L));
}

// Insere um novo nó na lista circular
void insereCSEO(struct Lista **inicio, int num) {
    struct Lista *no = (struct Lista *) malloc(sizeof(struct Lista));
    no->num = num;

    if ((*inicio) == NULL) {
        (*inicio) = no;
        no->prox = (*inicio);
        return;
    }

    struct Lista *ant, *pont;
    buscaCSEO(num, inicio, &ant, &pont);

    if (ant == NULL) {
        no->prox = (*inicio);
        struct Lista *temp = (*inicio);
        while (temp->prox != (*inicio)) {
            temp = temp->prox;
        }
        temp->prox = no;
        (*inicio) = no;
    } else {
		no->prox = ant->prox;
        ant->prox = no;
    }
}

// Remove o nó da lista circular
void removeCSEO(struct Lista **inicio, int num){
	if((*inicio) == NULL) return;
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

// Inicializa lista com nó cabeça
void inicializaDENC(struct ListaDup **L){
  (*L) = (struct ListaDup *) malloc(sizeof(struct ListaDup));
  (*L)->ant = NULL;
  (*L)->prox = NULL;
  (*L)->num = 0;
}

// Retorna nó do ponteiro com o valor encontrado
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

// Insere nó na lista duplamente encadeada com nó cabeça
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

// Remove nó da lista duplamente encadeada com nó cabeça
void removeDENC(struct ListaDup **L, int num){
	if((*L) == NULL) return;
  struct ListaDup *ptr = (*L)->prox, *temp;
  while(ptr != NULL){
    if(ptr->num == num){
      temp = ptr;
      ptr->ant->prox  = ptr->prox;
      free(temp);
    }
    ptr = ptr->prox;
  }
}

// Insere valor na pilha
void insereLIFO(int pilha[], int n, int *topo, int valor){
  if((*topo) != n){
    (*topo) = (*topo) + 1;
    pilha[(*topo)] = valor;
    return;
  }
  printf("A pilha esta cheia!\n");
}

// Remove valor na pilha
void removeLIFO(int *topo){
	if((*topo) != -1){
		(*topo) = (*topo) - 1;
		return;
	}
	printf("A pilha esta vazia!\n");
}

// Insere valor na fila
void insereFIFO(int fila[], int n, int *fim, int valor){
  if((*fim) == n-1){
    printf("Fila esta cheia!\n");
    return;
  } else {
    (*fim) = (*fim) + 1;
    fila[(*fim)] = valor;
  }
}

// Remove valor na fila
void removeFIFO(int fila[], int *fim){
	if((*fim) != -1){
		for(int i = 0; i < (*fim); i++){
		fila[i] = fila[i+1];
		}
		(*fim) = (*fim) - 1;
		return;
	}
	printf("A fila esta vazia!\n");
}

// Retorna qual nó contém o valor procurado na lista duplamente encadeada
int retornaIndiceDENC(struct ListaDup **L, int valor){
	if((*L)->prox == NULL) return 0;
	int indice = 0;
	struct ListaDup *p = (*L)->prox;
	while(p != NULL){
		if(p->num == valor){
			return indice + 2;
		}
		indice++;
		p = p->prox;
	}
	return 0;
}

// Retorna qual nó contém o valor procurado na lista simplesmente encadeada
int retornaIndice(struct Lista **L, int valor){
	if((*L) == NULL) return 0;
	int indice = 0;
	struct Lista *p = (*L);
	while(p != NULL){
		if(p->num == valor){
			return indice + 1;
		}
		indice++;
		p = p->prox;
	}
	return 0;
}

// Retorna qual nó contém o valor procurado na lista circular
int retornaIndiceCSEO(struct Lista **L, int valor){
	if((*L) == NULL) return 0;
	int indice = 0;
	struct Lista *p = (*L);
	do{
		if(p->num == valor){
			return indice + 1;
		}
		indice++;
		p = p->prox;
	}while(p != (*L));
	return 0;
}

// Desaloca todos os nós da lista duplamente encadeada
void desalocaDENC(struct ListaDup **L){
	if((*L) == NULL) return;
	struct ListaDup *p = (*L), *temp;
	while(p != NULL){
		temp = p->prox;
		free(p);
		p = temp;
	}
}

// Desaloca todos os nós da lista simplesmente encadeada
void desaloca(struct Lista **L){
	if((*L) == NULL) return;
	struct Lista *p = (*L), *temp;
	while(p != NULL){
		temp = p->prox;
		free(p);
		p = temp;
	}
}

// Desaloca todos os nós da lista circular
void desalocaCSEO(struct Lista **L){
	if((*L) == NULL) return;
	struct Lista *p = (*L), *temp;
	do{
		temp = p;
		p = p->prox;
		free(temp);
	}while(p != (*L));
}

// Menu de opções para manusear a fila com exibição gráfica
void menuFIFO(){
	int n = 10, fila[n], fim = -1;
	int esc, valor;
	char num[100] = "";
	do{
		int x1 = 10, x2 = 60, y = 18;
		printf(" - Fila -\n");
		printf("[ 1 ] Insercao\n");
		printf("[ 2 ] Remocao\n");
		printf("[ 3 ] Sair\n");
		printf("Selecione a operacao > ");
		scanf("%d", &esc);
		switch(esc){
			case 1:
				printf("Insira o valor do no > ");
				scanf("%d", &valor);
				getchar();
				insereFIFO(fila, n, &fim, valor);
				break;
			case 2:
				removeFIFO(fila, &fim);
				break;
			case 3:
				printf("Programa encerrado\n");
				gfx_quit();
				return;
			default:
				printf("Opcao invalida, tente novamente!\n");
				break;
		}
		gfx_quit();
		gfx_init(800, 300, "Fila");
		for(int i = 0; i <= fim; i++){
			sprintf(num, "%d", fila[i]);
			gfx_rectangle(x1, 10, x2, 40);
			gfx_text(x1+((x2-x1)/2)-2, y, num);
			gfx_paint();
			x1 = x2;
			x2 = x1 + 50;
		}
	}while(esc != 4);
}

// Menu de opções para manusear a pilha com exibição gráfica
void menuLIFO(){
	int n = 10, pilha[n], topo = -1;
	int esc, valor;
	char num[100] = "";
	do{
		int x1 = 10, x2 = 60, y = 18;
		printf(" - Pilha -\n");
		printf("[ 1 ] Insercao\n");
		printf("[ 2 ] Remocao\n");
		printf("[ 3 ] Sair\n");
		printf("Selecione a operacao > ");
		scanf("%d", &esc);
		switch(esc){
			case 1:
				printf("Insira o valor do no > ");
				scanf("%d", &valor);
				getchar();
				insereLIFO(pilha, n, &topo, valor);
				break;
			case 2:
				removeLIFO(&topo);
				break;
			case 3:
				printf("Programa encerrado\n");
				gfx_quit();
				return;
			default:
				printf("Opcao invalida, tente novamente!\n");
				break;
		}
		gfx_quit();
		gfx_init(800, 300, "Pilha");
		for(int i = 0; i <= topo; i++){
			sprintf(num, "%d", pilha[i]);
			gfx_rectangle(x1, 10, x2, 40);
			gfx_text(x1+((x2-x1)/2)-2, y, num);
			gfx_paint();
			x1 = x2;
			x2 = x1 + 50;
		}
	}while(esc != 4);
}

// Menu de opções para manusear a lista duplamente encadeada com exibição gráfica
void menuDENC(){
	struct ListaDup *L = NULL;
  	inicializaDENC(&L);
  	struct ListaDup *p;
	int esc, valor, cont, temp;
	char num[100] = "";
	do{
		int x1 = 10, x2 = 60, y = 18;
		cont = 0;
		printf(" - Lista Duplamente Encadeada Com No Cabeca -\n");
		printf("[ 1 ] Insercao\n");
		printf("[ 2 ] Remocao\n");
		printf("[ 3 ] Busca\n");
		printf("[ 4 ] Sair\n");
		printf("Selecione a operacao > ");
		scanf("%d", &esc);
		switch(esc){
			case 1:
				printf("Insira o valor do no > ");
				scanf("%d", &valor);
				getchar();
				insereDENC(&L, valor);
				break;
			case 2:
				printf("Insira o valor que deseja remover > ");
				scanf("%d", &valor);
				getchar();
				removeDENC(&L, valor);
				break;
			case 3:
				printf("Qual valor deseja buscar? > ");
				scanf("%d", &valor);
				getchar();
				temp = retornaIndiceDENC(&L, valor);
				if(temp){
					printf("Valor encontrado em no [%d]\n", temp);
				} else {
					printf("Valor nao encontrado na lista!\n");
				}
				break;
			case 4:
				printf("Programa encerrado, a lista foi desalocada!\n");
				desalocaDENC(&L);
				gfx_quit();
				return;
			default:
				printf("Opcao invalida, tente novamente!\n");
				break;
		}
		gfx_quit();
		gfx_init(800, 300, "Lista Duplamente Encadeada Com No Cabeca");
		p = L;
		while(p != NULL){
			sprintf(num, "%d", p->num);
			gfx_rectangle(x1, 10, x2, 40);
			gfx_text(x1+((x2-x1)/2)-2, y, num);
			if(cont){
				gfx_line(x1, 25, x1 - 20, 25);
				gfx_line(x1-7, 20, x1, 25);
				gfx_line(x1-7, 30, x1, 25);
				gfx_line(x1-13, 20, x1-20, 25);
				gfx_line(x1-13, 30, x1-20, 25);

			}
			gfx_paint();
			x1 = x2 + 20;
			x2 = x1 + 50;
			p = p->prox;
			cont = 1;
		}
	}while(esc != 4);
}

// Menu de opções para manusear a lista circular com exibição gráfica
void menuCSEO(){
	struct Lista *L = NULL;
  	struct Lista *p;
	int esc, valor, cont, temp;
	int iniX = 0, fimX = 0;
	char num[100] = "";
	do{
		int x1 = 10, x2 = 60, y = 18;
		cont = 0;
		printf(" - Lista Circular Simplesmente Encadeada Ordenada Sem No Cabeca -\n");
		printf("[ 1 ] Insercao\n");
		printf("[ 2 ] Remocao\n");
		printf("[ 3 ] Busca\n");
		printf("[ 4 ] Sair\n");
		printf("Selecione a operacao > ");
		scanf("%d", &esc);
		switch(esc){
			case 1:
				printf("Insira o valor do no > ");
				scanf("%d", &valor);
				getchar();
				insereCSEO(&L, valor);
				break;
			case 2:
				printf("Insira o valor que deseja remover > ");
				scanf("%d", &valor);
				getchar();
				removeCSEO(&L, valor);
				break;
			case 3:
				printf("Qual valor deseja buscar? > ");
				scanf("%d", &valor);
				getchar();
				temp = retornaIndiceCSEO(&L, valor);
				if(temp){
					printf("Valor encontrado em no [%d]\n", temp);
				} else {
					printf("Valor nao encontrado na lista!\n");
				}
				break;
			case 4:
				printf("Programa encerrado, a lista foi desalocada!\n");
				desalocaCSEO(&L);
				gfx_quit();
				return;
			default:
				printf("Opcao invalida, tente novamente!\n");
				break;
		}
		gfx_quit();
		gfx_init(800, 300, "Lista Circular Simplesmente Encadeada Ordenada Sem No Cabeca");
		p = L;

		iniX = x1+((x2-x1)/2)-2;
		gfx_line(iniX, 40, iniX, 60);
		gfx_line(iniX, 40, iniX+7, 47);
		gfx_line(iniX, 40, iniX-7, 47);
		do{
			sprintf(num, "%d", p->num);
			gfx_rectangle(x1, 10, x2, 40);
			gfx_text(x1+((x2-x1)/2)-2, y, num);
			if(p->prox == L){
				fimX = x1+((x2-x1)/2)-2;
				gfx_line(fimX, 40, fimX, 60);
				gfx_line(iniX, 60, fimX, 60);
			}
			if(cont){
				gfx_line(x1, 25, x1 - 20, 25);
				gfx_line(x1-7, 20, x1, 25);
				gfx_line(x1-7, 30, x1, 25);
			}
			gfx_paint();
			x1 = x2 + 20;
			x2 = x1 + 50;
			p = p->prox;
			cont = 1;
		}while(p != L);
	}while(esc != 4);
}

// Menu de opções para manusear a lista simplesmente encadeada com exibição gráfica
void menuSEO(){
	struct Lista *L = NULL;
  	struct Lista *p;
	int esc, valor, cont, temp;
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
				printf("Insira o valor do no > ");
				scanf("%d", &valor);
				getchar();
				insereSEO(valor, &L);
				break;
			case 2:
				printf("Insira o valor que deseja remover > ");
				scanf("%d", &valor);
				getchar();
				removeSEO(&L, valor);
				break;
			case 3:
				printf("Qual valor deseja buscar? > ");
				scanf("%d", &valor);
				getchar();
				temp = retornaIndice(&L, valor);
				if(temp){
					printf("Valor encontrado em no [%d]\n", temp);
				} else {
					printf("Valor nao encontrado na lista!\n");
				}
				break;
			case 4:
				printf("Programa encerrado, a lista foi desalocada!\n");
				desaloca(&L);
				gfx_quit();
				return;
			default:
				printf("Opcao invalida, tente novamente!\n");
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
				gfx_line(x1-7, 20, x1, 25);
				gfx_line(x1-7, 30, x1, 25);
			}
			gfx_paint();
			x1 = x2 + 20;
			x2 = x1 + 50;
			p = p->prox;
			cont = 1;
		}
		
	}while(esc != 4);
}

int main(){
	int esc;
	printf("		-- MENU --\n");
	printf("[ 1 ] Lista Simplesmente Encadeada Ordenada Sem No Cabeca\n");
	printf("[ 2 ] Lista Circular Simplesmente Encadeada Ordenada Sem No Cabeca\n");
	printf("[ 3 ] Duplamente Encadeada Com No Cabeca\n");
	printf("[ 4 ] Pilha\n");
	printf("[ 5 ] Fila\n");
	do{
		printf("Qual estrutura deseja usar? > ");
		scanf("%d", &esc);
		switch(esc){
			case 1:
				menuSEO();
				break;
			case 2:
				menuCSEO();
				break;
			case 3:
				menuDENC();
				break;
			case 4:
				menuLIFO();
				break;
			case 5:
				menuFIFO();
				break;
			default:
				printf("Opcao invalida, tente novamente!\n");
				break;
		}
	}while(esc < 1 || esc > 5);
  return 0;
}