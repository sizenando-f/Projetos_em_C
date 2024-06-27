#include "gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */

// Desenha o círculo
void desenhaCirculo(int x, int y, int raiox, int raioy, int n){
	if(n < 0) return;

	int novo_raiox = (raiox/2);
	int novo_raioy = (raiox/2);
	int novo_n = n - 1;

	gfx_ellipse(x, y, raiox, raioy);

	desenhaCirculo(x + novo_raiox*0.71, y + novo_raioy*0.71, novo_raiox, novo_raioy, novo_n);
	desenhaCirculo(x + novo_raiox*0.71, y - novo_raioy*0.71, novo_raiox, novo_raioy, novo_n);
	desenhaCirculo(x - novo_raiox*0.71, y + novo_raioy*0.71, novo_raiox, novo_raioy, novo_n);
	desenhaCirculo(x - novo_raiox*0.71, y - novo_raioy*0.71, novo_raiox, novo_raioy, novo_n);

}

// Desenha o quadrado
void desenhaQuadrado(int xEsq, int ySup, int xDir, int yInf, int n){
	if(n < 0) return;

	int metade = (yInf + ySup)/2;
	int metadey = (yInf - ySup)/2;
	int metadex = (xDir - xEsq)/2;
	int metadex2 = (xEsq + xDir)/2;

	//Quadrado esquerdo
	int Enovo_xEsq = (metadex2-(metadex*2));
	int Enovo_xDir = xEsq;
	int Enovo_ySup = metade-((metade-ySup)/2);
	int Enovo_yInf = metade+((metade-ySup)/2);

	//Quadrado direito
	int Dnovo_xEsq = xDir;
	int Dnovo_xDir = xEsq+(metadex*3);
	int Dnovo_ySup = metade-((metade-ySup)/2);
	int Dnovo_yInf = metade+((metade-ySup)/2);

	//Quadrado superior
	int Snovo_yInf = ySup;
	int Snovo_ySup = metade-(metadey*2);
	int Snovo_xEsq = metadex2-((metadex2-xEsq)/2);
	int Snovo_xDir = metadex2+((metadex2-xEsq)/2);

	//Quadrado inferior
	int Inovo_yInf = metade+(metadey*2);
	int Inovo_ySup = yInf;
	int Inovo_xEsq = metadex2-((metadex2-xEsq)/2);
	int Inovo_xDir = metadex2+((metadex2-xEsq)/2);
	
	
	desenhaQuadrado(Dnovo_xEsq, Dnovo_ySup, Dnovo_xDir, Dnovo_yInf, n-1);
	desenhaQuadrado(Enovo_xEsq, Enovo_ySup, Enovo_xDir, Enovo_yInf, n-1);
	desenhaQuadrado(Inovo_xEsq, Inovo_ySup, Inovo_xDir, Inovo_yInf, n-1);
	desenhaQuadrado(Snovo_xEsq, Snovo_ySup, Snovo_xDir, Snovo_yInf, n-1);
	

	gfx_set_color(255, 255, 255);
	gfx_rectangle(xEsq, ySup, xDir, yInf);
	gfx_set_color(0, 0, 0);
	gfx_filled_rectangle(xEsq+1, ySup+1, xDir-1, yInf-1);
	
}

int main()
{
	int esc, n;
	printf("[ 1 ] Circulo\n");
	printf("[ 2 ] Quadrado\n");
	printf("O que deseja desenhar? >");
	scanf("%d", &esc);
	switch(esc){
		case 1:
			do{
				printf("Insira o nivel de recursao (0 - 10) >");
				scanf("%d", &n);
				if(n < 0 || n > 10){
					printf("ERRO: Entrada de nivel invalida!\n");
				}
			}while(n < 0 || n > 10);
			gfx_init(600, 600, "Recursividade");
			desenhaCirculo(300, 300, 290, 290, n);
			gfx_paint();
			sleep(5);
			gfx_quit();
			break;
		case 2:
			do{
				printf("Insira o nivel de recursao (0 - 10) >");
				scanf("%d", &n);
				if(n < 0 || n > 10){
					printf("ERRO: Entrada de nivel invalida!\n");
				}
			}while(n < 0 || n > 10);
			gfx_init(600, 600, "Recursividade");
			desenhaQuadrado(200, 200, 400, 400, n);
			gfx_paint();
			sleep(5);
			gfx_quit();
			break;
		case 3:
			printf("Programa encerrado!\n");
			gfx_quit();
			break;
		default:
			printf("ERRO: Escolha invalida, tente novamente!\n");
			gfx_quit();
			break;
	}

	return 0;
}
