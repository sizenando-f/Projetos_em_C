#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


void pegaTexto(char fraseOriginal[], char texto[][82], int codigo, char palavraAntiga[], char palavraNova[]){
  char * frase = strdup(fraseOriginal);
  int fraseTam = strlen(frase)/75;
  int existePalavra = 0;
  for (int i = 0; i < fraseTam; i++){
    strcpy(texto[i], "");
  }
  system("cls");
  if(codigo == 0){
    char * palavra = strtok(frase, " ");
    strcpy(texto[0], palavra);
    strcat(texto[0], " ");

    palavra = strtok(NULL, " ");

    for (int i = 0; i < fraseTam+2; i++){
      int check = 0;
      while(check == 0 && palavra != NULL){
        strcat(texto[i], palavra);
        
        if(strlen(texto[i]) > 80){
          check = 1;
          char * ponteiro = strrchr(texto[i], ' ');
          *ponteiro = '\0';
          strcpy(texto[i + 1], palavra);
        }

        if(check){
          strcat(texto[i+1], " ");
        } else {
          strcat(texto[i], " ");
        }

        palavra = strtok(NULL, " ");
      }
    }
  } else if(codigo == 1){
    int control = 0;
    char * palavra = strtok(frase, " ");

    if(strcmp(palavraAntiga, palavra) == 0){
      strcpy(texto[0], palavraNova);
      control = 1;
      existePalavra = 1;
    } else {
      strcpy(texto[0], palavra);
    }

    strcat(texto[0], " ");

    palavra = strtok(NULL, " ");

    for (int i = 0; i < fraseTam+2; i++){
      int check = 0;
      while(check == 0 && palavra != NULL){
        if((strcmp(palavraAntiga, palavra) == 0) && (control == 0)){
          strcat(texto[i], palavraNova);
          control = 1;
          existePalavra = 1;
        } else {
          strcat(texto[i], palavra);
        }
        
        if(strlen(texto[i]) > 80){
          check = 1;
          char * ponteiro = strrchr(texto[i], ' ');
          *ponteiro = '\0';
          if((strcmp(palavraAntiga, palavra) == 0) && (control == 0)){
            strcpy(texto[i + 1], palavraNova);
            control = 1;
            existePalavra = 1;
          } else {
            strcpy(texto[i + 1], palavra);
          }
        }

        if(check){
          strcat(texto[i+1], " ");
        } else {
          strcat(texto[i], " ");
        }

        palavra = strtok(NULL, " ");
      }
    }
    if(existePalavra){
        printf("ACAO REALIZADA COM SUCESSO!\n");
      } else {
        printf("A PALAVRA NAO EXISTE!\n");
      }
      system("pause");
  } else if(codigo == 2){
    char * palavra = strtok(frase, " ");

    if(strcmp(palavraAntiga, palavra) == 0){
      strcpy(texto[0], palavraNova);
      existePalavra = 1;
    } else {
      strcpy(texto[0], palavra);
    }

    strcat(texto[0], " ");

    palavra = strtok(NULL, " ");

    for (int i = 0; i < fraseTam+2; i++){
      int check = 0;
      while(check == 0 && palavra != NULL){
        if((strcmp(palavraAntiga, palavra) == 0)){
          strcat(texto[i], palavraNova);
          existePalavra = 1;
        } else {
          strcat(texto[i], palavra);
        }
        
        if(strlen(texto[i]) > 80){
          check = 1;
          char * ponteiro = strrchr(texto[i], ' ');
          *ponteiro = '\0';
          if((strcmp(palavraAntiga, palavra) == 0)){
            strcpy(texto[i + 1], palavraNova);
            existePalavra = 1;
          } else {
            strcpy(texto[i + 1], palavra);
          }
        }

        if(check){
          strcat(texto[i+1], " ");
        } else {
          strcat(texto[i], " ");
        }

        palavra = strtok(NULL, " ");
      }
    }
    if(existePalavra){
        printf("ACAO REALIZADA COM SUCESSO!\n");
      } else {
        printf("A PALAVRA NAO EXISTE!\n");
      }
      system("pause");
  }
  
}

void imprimeTexto(char texto[][82], int fraseTam, int *alinhamento){
    int espaco = 80;
    system("cls");
    switch (*alinhamento){
      case 0:
        for (int i = 0; i < fraseTam; i++){
          printf("%s\n", texto[i]);
        }
        break;
      case 1:
        for (int i = 0; i < fraseTam; i++){
          printf("%*s\n", espaco, texto[i]);
        }
        break;
      case 2: {
        for (int i = 0; i < fraseTam; i++){
          int espacosLaterais = (espaco - strlen(texto[i])) / 2;
          printf("%*s%s%*s\n", espacosLaterais, "", texto[i], espacosLaterais, "");
        }
        break;
      }
      case 3: {
        for (int i = 0; i < fraseTam; i++){
          char * frase = strdup(texto[i]);
          char * palavra = strtok(frase, " ");
          int espacosLaterais = (espaco - strlen(texto[i]));

          fputs(palavra, stdout);
          fputs(" ", stdout);
          while(palavra != NULL){
            if(espacosLaterais > 0){
              fputs(" ", stdout);
              espacosLaterais--;
            }
            palavra = strtok(NULL, " ");
            fputs(palavra, stdout);
            fputs(" ", stdout);
          }
          printf("\n");
        }
      }
        break;
      default:
        break;
    }
    system("pause");
}

void encontraPalavra(char texto[][82], char palavra[], int fraseTam){
  char temp[50][82];
  int cont = 0;
  for(int i = 0; i < fraseTam; i++){
    strcpy(temp[i], texto[i]);
  }

  for (int i = 0; i < fraseTam; i++) {
    char *linha = strdup(temp[i]); // Copia a linha para uma variável temporária
    char *palavraAtual = strtok(linha, " ,().-");
    int coluna = 0;

    while (palavraAtual != NULL) {
        if (strcmp(palavraAtual, palavra) == 0) {
            printf("Palavra encontrada na linha %d\n", i + 1);
            printf("Na coluna %d\n", coluna);
            cont++;
        }
        coluna += strlen(palavraAtual) + 1; // Avança a coluna para o início da próxima palavra
        palavraAtual = strtok(NULL, " ,().-");
    }

    free(linha); // Libera a memória alocada pela strdup
  }
  if(!cont){
    printf("PALAVRA NAO EXISTE!\n");
  }
  system("pause");
}

void transformaEmFrase(char texto[][82], char frase[], int fraseTam){
  strcpy(frase, "");

  for (int i = 0; i < fraseTam; i++){
    char * temp = strdup(texto[i]);
    char * palavra = strtok(temp, " ");
    while(palavra != NULL){
      strcat(frase, palavra);
      strcat(frase, " ");
      palavra = strtok(NULL, " ");
    }
  }
}

void menuSubstituir(char texto[][82], int fraseTam){
  int esc;
  char palavraAntiga[30], palavraNova[30];
  char frase[4000];
  while((esc < 1 || esc > 3) || (esc != 3)){
    system("cls");
    printf("------------ # SUBSTITUIR PALAVRA # ------------\n");
    printf("1) SUBSTITUIR PRIMEIRA OCORRENCIA \n");
    printf("2) SUBSTITUIR TODAS OCORRENCIAS \n");
    printf("3) VOLTAR \n");
    printf("---------------------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        printf("# SUBSTITUIR PRIMEIRA OCORRENCIA #\n");
        printf("PALAVRA QUE DESEJA SUBSTITUIR: ");
        scanf("%s", palavraAntiga);
        printf("PALAVRA NOVA: ");
        scanf("%s", palavraNova);
        transformaEmFrase(texto, frase, fraseTam);
        pegaTexto(frase, texto, 1, palavraAntiga, palavraNova);
        break;
      case 2:
        printf("# SUBSTITUIR TODAS OCORRENCIAS #\n");
        printf("PALAVRA QUE DESEJA SUBSTITUIR: ");
        scanf("%s", palavraAntiga);
        printf("PALAVRA NOVA: ");
        scanf("%s", palavraNova);
        transformaEmFrase(texto, frase, fraseTam);
        pegaTexto(frase, texto, 2, palavraAntiga, palavraNova);
        break;
      case 3:
        break;
      default:
        printf("ESCOLHA INVALIDA!\n");
        system("pause");
        break;
    }
  }

}

void paraMaiusculo(char texto[][82], int fraseTam){
  for (int i = 0; i < fraseTam; i++){
    for (int j = 0; j < 82; j++){
      texto[i][j] = toupper(texto[i][j]);
    }
  }
}

void paraMinusculo(char texto[][82], int fraseTam){
  for (int i = 0; i < fraseTam; i++){
    for (int j = 0; j < 82; j++){
      texto[i][j] = tolower(texto[i][j]);
    }
  }
}

void primeiraParaMaiusculo(char texto[][82], int fraseTam){
  for (int i = 0; i < fraseTam; i++){
    texto[i][0] = toupper(texto[i][0]);
  }
}

void menuCaixa(char texto[][82], int fraseTam){
  int esc;
  while((esc < 1 || esc > 4) || (esc != 4)){
    system("cls");
    printf("------------ # MODIFICAR CAIXA # ------------\n");
    printf("1) TODOS CARACACTERES EM MAIUSCULO \n");
    printf("2) TODOS OS CARACTERES EM MINUSCULO \n");
    printf("3) CARACTERES INICIAL DAS FRASES EM MAIUSCULO \n");
    printf("4) VOLTAR\n");
    printf("---------------------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    system("cls");
    switch (esc){
      case 1:
        paraMaiusculo(texto, fraseTam);
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
        break;
      case 2:
        paraMinusculo(texto, fraseTam);
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
        break;
      case 3:
        primeiraParaMaiusculo(texto, fraseTam);
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
        break;
      case 4:
      break;
      default:
        printf("OPCAO INVALIDA!\n");
        system("pause");
        break;
    }
  }
}

void menuAlinhamento(int *alinhamento){
  int esc;
  while((esc < 1 || esc > 5) || (esc != 5)){
    system("cls");
    printf("------------ # MODIFICAR CAIXA # ------------\n");
    printf("|1) ALINHAR TEXTO À ESQUERDA                |\n");
    printf("|2) ALINHAR TEXTO À DIREITA                 |\n");
    printf("|3) CENTRALIZAR TEXTO                       |\n");
    printf("|4) JUSTIFICAR TEXTO                        |\n");
    printf("|5) VOLTAR                                  |\n");
    printf("---------------------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    system("cls");
    switch (esc){
      case 1:
        *alinhamento = 0;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
        break;
      case 2:
        *alinhamento = 1;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
        break;
      case 3:
        *alinhamento = 2;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
        break;
      case 4:
        *alinhamento = 3;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        system("pause");
      case 5:
        break;
      default:
        printf("OPCAO INVALIDA!\n");
        system("pause");
        break;
    }
    
  }
}

void menuPrincipal(char texto[][82], int fraseTam){
  int esc;
  static int alinhamento = 0;
  char palavra[20];
  while((esc < 1 || esc > 6) || (esc != 6)){
    system("cls");
    printf("-------- # EDITOR DE TEXTO # --------\n");
    printf("|1) IMPRIMIR TEXTO FORMATADO        |\n");
    printf("|2) ENCONTRAR PALAVRA               |\n");
    printf("|3) OPCOES DE SUBSTITUICAO          |\n");
    printf("|4) OPCOES DE CAIXA                 |\n");
    printf("|5) OPCOES DE ALINHAMENTO           |\n");
    printf("|6) ENCERRAR PROGRAMA               |\n");
    printf("-------------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    switch(esc){
      case 1:
        imprimeTexto(texto, fraseTam, &alinhamento);
        break;
      case 2:
        printf("QUAL PALAVRA DESEJA ENCONTRAR? -> ");
        scanf("%s", palavra);
        encontraPalavra(texto, palavra, fraseTam);
        break;
      case 3:
        menuSubstituir(texto, fraseTam);
        break;
      case 4:
        menuCaixa(texto, fraseTam);
        break;
      case 5:
        menuAlinhamento(&alinhamento);
        break;
      case 6:
        break;
      default:
        printf("OPCAO INVALIDA!\n");
        system("pause");
        break;
    }

    
  }
}

int main(){
  char frase[] = "William Henry Gates III KBE GCIH (Seattle, 28 de outubro de 1955) mais conhecido como Bill Gates, e um magnata, empresario, diretor executivo, investidor, filantropo e autor americano, que ficou conhecido por fundar junto com Paul Allen a Microsoft a maior e mais conhecida empresa de software do mundo em termos de valor de mercado. Gates ocupa atualmente o cargo de presidente nao-executivo da Microsoft alem de ser classificado regularmente como a pessoa mais rica do mundo, posicao ocupada por ele de 1995 a 2007, 2009, e de 2014 a 2017. E um dos pioneiros na revolucao do computador pessoal. Gates nasceu em uma familia de classe media de Seattle. Seu pai, William H. Gates, era advogado de grandes empresas, e sua mae, Mary Maxwell Gates, foi professora da Universidade de Washington e diretora de bancos. Bill Gates e as suas duas irmas, Kristanne e Libby, frequentaram as melhores escolas particulares de sua cidade natal, e Bill tambem participou do Movimento Escoteiro ainda quando jovem. Bill Gates, foi admitido na prestigiosa Universidade Harvard, (conseguindo 1590 SATs dos 1600 possiveis) mas abandonou os cursos de Matematica e Direito no terceiro ano para dedicar-se a Microsoft. Trabalhou na Taito com o desenvolvimento de software basico para maquinas de jogos eletronicos (fliperamas) ate seus 16 anos. Tambem trabalhou como pesquisador visitante na University of Massachusetts at Amherst, UMASS, Estados Unidos, quando com 17 anos, desenvolveu junto com Paul Allen um software para leitura de fitas magneticas, com informacoes de trafego de veiculos, em um chip Intel 8008. Com esse produto, Gates e Allen criaram uma empresa, a Traf-o-Data, porem os clientes desistiram do negocio quando descobriram a idade dos donos. Enquanto estudavam em Harvard, os jovens desenvolveram um interpretador da linguagem BASIC para um dos primeiros computadores pessoais a serem lancado nos Estados Unidos - o Altair 8800. Apos um modesto sucesso na comercializa��o deste produto, Gates e Allen fundaram a Microsoft, uma das primeiras empresas no mundo focadas exclusivamente no mercado de programas para computadores pessoais ou PCs. Gates adquiriu ao longo dos anos uma fama de visionario (apostou no mercado de software na epoca em que o hardware era considerado muito mais valioso) e de negociador agressivo, chegando muitas vezes a ser acusado por concorrentes da Microsoft de utilizar praticas comerciais desleais. Nos anos 1980, a IBM, lider no mercado de grandes computadores, resolveu entrar no mercado da microinformatica com o PC, porem faltava o Sistema Operacional. Para isso, fechou contrato com a recem-criada Microsoft. Todavia, a Microsoft nao possuia o software ainda. O jovem Bill Gates foi a uma pequena empresa que havia desenvolvido o sistema para o processador da Intel e decidiu compra-lo, pagou cerca de US$ 50 mil, personalizou o programa e vendeu-o por US$ 8 milhoes, mantendo a licenca do produto. Este viria a ser o MS-DOS. Fonte:";
  char texto[100][82];
  int fraseTam = strlen(frase)/75;
  pegaTexto(frase, texto, 0, "", "");

  menuPrincipal(texto, fraseTam);
  return 0;
}