// Editor de Texto
// Descrição: Manipulação de um texto pré-definido, busca, substituição, tamanho e alinhamento de texto
// Autor: Sizenando S. França
// Última modificação: 26-09-23

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Procedimento que organiza o texto na matriz
void pegaTexto(char fraseOriginal[], char texto[][82], int codigo, char palavraAntiga[], char palavraNova[]){
  char * frase = strdup(fraseOriginal); // Duplica frase
  int fraseTam = strlen(frase)/75;  // Descobre quantas linhas terá o texto
  int existePalavra = 0;  // Variável controle para verificar se palavra a ser alterada existe

  // Inicializa matriz do texto
  for (int i = 0; i < fraseTam; i++){ 
    strcpy(texto[i], "");
  }

  system("cls");

  if(codigo == 0){  // 0 = Procedimento padrão de inserção de palavras no texto
    char * palavra = strtok(frase, " ");  // Pega primeira palavra da frase
    strcpy(texto[0], palavra);  // Insere na primeria linha do texto
    strcat(texto[0], " ");  // Concatena um espaço

    palavra = strtok(NULL, " ");  // Prossegue para próxima palavra

    // Itera por todas as linhas
    for (int i = 0; i < fraseTam+2; i++){
      int check = 0;  // Variável controle para descobrir se a linha já possui 80 caracteres
      while(check == 0 && palavra != NULL){ // Repete enquanto houver palavra na frase e a linha não tiver 80 caracteres
        strcat(texto[i], palavra);  // Concatena palavra no texto
        
        if(strlen(texto[i]) > 80){  // Verifica se linha possui mais de 80 caracteres
          check = 1;  // Variável controla se torna verdadeira
          char * ponteiro = strrchr(texto[i], ' '); // Ponteiro para o último espaço da linha
          *ponteiro = '\0'; // Inserção de um finalizador de string no espaço
          strcpy(texto[i + 1], palavra);  // Insere palavra na próxima linha
        }

        // Concatena espaço de acordo com a linha disponível 
        if(check){
          strcat(texto[i+1], " ");
        } else {
          strcat(texto[i], " ");
        }

        palavra = strtok(NULL, " ");  // Passa para próxima palavra
      }
    }
  } else if(codigo == 1){ // 1 = Procedimento de inserção de palavras no texto alterando uma palavra
    int control = 0;  // Variável controle para saber se palavra já foi alterada
    char * palavra = strtok(frase, " ");  // Primeira palavra da frase
    
    if(strcmp(palavraAntiga, palavra) == 0){  // Verifica se palavra a ser alterada é igual a palavra atual
      strcpy(texto[0], palavraNova);  // Insere para palavra nova
      control = 1;  // Palavra alterada se torna verdadeiro
      existePalavra = 1;  // Palavra existe
    } else {
      strcpy(texto[0], palavra);  // Se a palavra não for igual, continua normalmente a inserção de palavras
    }

    strcat(texto[0], " ");  // Concatena espaço

    palavra = strtok(NULL, " ");  // Passa para próxima palavra

    for (int i = 0; i < fraseTam+2; i++){
      int check = 0; // Variável controle para descobrir se a linha já possui 80 caracteres
      while(check == 0 && palavra != NULL){ // Repete enquanto houver palavra na frase e a linha não tiver 80 caracteres
        if((strcmp(palavraAntiga, palavra) == 0) && (control == 0)){  // Verifica se palavra a ser alterada é igual a palavra atual
          strcat(texto[i], palavraNova);  // Insere para palavra nova
          control = 1;  // Palavra alterada se torna verdadeiro
          existePalavra = 1;  // Palavra existe
        } else {
          strcat(texto[i], palavra);   // Se a palavra não for igual, continua normalmente a inserção de palavras
        }
        
        if(strlen(texto[i]) > 80){
          check = 1;  // Variável controla se torna verdadeira
          char * ponteiro = strrchr(texto[i], ' '); // Ponteiro para o último espaço da linha
          *ponteiro = '\0'; // Inserção de um finalizador de string no espaço
          if((strcmp(palavraAntiga, palavra) == 0) && (control == 0)){  // Verifica se palavra a ser alterada é igual a palavra atual e que a palavra ainda não foi alterada
            strcpy(texto[i + 1], palavraNova);  // Insere para palavra nova
            control = 1;   // Palavra alterada se torna verdadeiro
            existePalavra = 1;  // Palavra existe
          } else { 
            strcpy(texto[i + 1], palavra);   // Se a palavra não for igual, continua normalmente a inserção de palavras
          }
        }

        // Concatena espaço de acordo com a linha disponível
        if(check){
          strcat(texto[i+1], " ");
        } else {
          strcat(texto[i], " ");
        }

        palavra = strtok(NULL, " ");  // Passa para próxima palavra
      }
    }

    // FeedBack de se a palava foi encontrada ou não
    if(existePalavra){
        printf("---------------------------\n");
        printf("ACAO REALIZADA COM SUCESSO!\n");
        printf("---------------------------\n");
      } else {
        printf("----------------------\n");
        printf("A PALAVRA NAO EXISTE!\n");
        printf("----------------------\n");
      }
      system("pause");
  } else if(codigo == 2){ // 2 = Procedimento de inserção alterando todas as palavras escolhidas
    char * palavra = strtok(frase, " ");  // Primeira palavra da frase

    if(strcmp(palavraAntiga, palavra) == 0){  // Verifica se palavra a ser alterada é igual a palavra atual
      strcpy(texto[0], palavraNova);  // Insere palavra nova
      existePalavra = 1;  // Variável controle de palavra existente se torna verdadeira
    } else {
      strcpy(texto[0], palavra);  // Se a palavra não for igual, continua normalmente a inserção de palavras
    }

    strcat(texto[0], " ");  // Concatena um espaço

    palavra = strtok(NULL, " ");  // Passa para próxima palavra

    // Itera por todas as linhas
    for (int i = 0; i < fraseTam+2; i++){
      int check = 0;  // Variável para saber se linha já possui 80 caracteres
      while(check == 0 && palavra != NULL){ // Repete enquanto houver palavra na frase e a linha não tiver 80 caracteres
        if((strcmp(palavraAntiga, palavra) == 0)){  // Verifica se palavra a ser alterada é igual a palavra atual
          strcat(texto[i], palavraNova);  // Concatena palavra nova
          existePalavra = 1;  // Variável controle de palavra existente se torna verdadeira
        } else {
          strcat(texto[i], palavra);  // Se a palavra não for igual, continua normalmente a inserção de palavras
        }
        
        if(strlen(texto[i]) > 80){  // Verifica se linha possui mais de 80 caracteres
          check = 1;  // Variável se torna verdadeira
          char * ponteiro = strrchr(texto[i], ' '); // Ponteiro para úlitmo espaço da linha
          *ponteiro = '\0'; // Insere finalizador de string no espaço
          if((strcmp(palavraAntiga, palavra) == 0)){  // Verifica se palavra a ser alterada é igual a palavra atual
            strcpy(texto[i + 1], palavraNova);  // Insere palavra nova na próxima linha
            existePalavra = 1;  // Variável controle de palavra existente se torna verdadeira
          } else {
            strcpy(texto[i + 1], palavra);   // Se a palavra não for igual, continua normalmente a inserção de palavras na próxima linha
          }
        }

        // Concatena espaço de acordo com a linha disponível
        if(check){
          strcat(texto[i+1], " ");
        } else {
          strcat(texto[i], " ");
        }

        palavra = strtok(NULL, " ");  // Passa para próxima palavra
      }
    }

    // FeedBack se a palavra foi encontrada ou não
    if(existePalavra){
        printf("---------------------------\n");
        printf("ACAO REALIZADA COM SUCESSO!\n");
        printf("---------------------------\n");
      } else {
        printf("----------------------\n");
        printf("A PALAVRA NAO EXISTE!\n");
        printf("----------------------\n");
      }
      system("pause");
  }

  free(frase); // Libera a memória da strdup
}

// Procedimento que existe texot ao usuário
void imprimeTexto(char texto[][82], int fraseTam, int *alinhamento){
    int espaco = 80;  // Espaço de visualização do texto
    system("cls");
    printf("--------------------------------------------------------------------------------\n");
    switch (*alinhamento){
      case 0: // 0 = Visualização à esquerda 
        for (int i = 0; i < fraseTam; i++){
          printf("%s\n", texto[i]);
        }
        break;
      case 1: // 1 = Visualização à direita
        for (int i = 0; i < fraseTam; i++){
          printf("%*s\n", espaco, texto[i]);  // Faz texto ocupar 80 "casas"
        }
        break;
      case 2: { // 2 = Visualização centralizada
        for (int i = 0; i < fraseTam; i++){
          int espacosLaterais = (espaco - strlen(texto[i])) / 2;  // Descobre espaços laterais dividindo por 2 os espaços restantes da linha
          printf("%*s%s%*s\n", espacosLaterais, "", texto[i], espacosLaterais, ""); // Exibe texto com os espaços laterais
        }
        break;
      }
      case 3: { // 3 = Visualização justificada
        // Itera por todas as linhas
        for (int i = 0; i < fraseTam; i++){
          char * frase = strdup(texto[i]);  // Duplica linha do texto
          char * palavra = strtok(frase, " ");  // Pega primeira palavra da linha
          int espacosLaterais = (espaco - strlen(texto[i]));  // Descobre espaços em branco da linha

          fputs(palavra, stdout); // Exibe palavra atual
          fputs(" ", stdout); // Exibe espaço
          while(palavra != NULL){ // Repete enquanto houver palavras na linha
            if(espacosLaterais > 0){  // Se houver espaços vaziod
              fputs(" ", stdout); // Insere espaço a mais
              espacosLaterais--;  // Reduz espaços vazios
            }
            palavra = strtok(NULL, " ");  // Prossegue para próxima palavra
            fputs(palavra, stdout); // Insere próxima palavra
            fputs(" ", stdout); // Insere outro espaço
          }
          printf("\n");
          free(frase); // Libera a memória da strdup
        }
      }
        break;
      default:
        break;
    }
    printf("--------------------------------------------------------------------------------\n");
    system("pause");
}

// Procedimento que encontra uma palavra recebida 
void encontraPalavra(char texto[][82], char palavra[], int fraseTam){
  char temp[50][82];
  int cont = 0;
  int contaPalavra = 0;

  // Insere cada linha do texto em uma matriz temporária
  for(int i = 0; i < fraseTam; i++){
    strcpy(temp[i], texto[i]);
  }
  system("cls");
  printf("   # PALAVRA: %s #\n", palavra);

  for (int i = 0; i < fraseTam; i++) {
    char *linha = strdup(temp[i]); // Copia a linha para uma variável temporária
    char *palavraAtual = strtok(linha, " ,().:"); // Pega primeira palavra da linha
    int coluna = 0; // Inicializa coluna
    int contPalavra = 0;
    while (palavraAtual != NULL) {  // Repete enquanto houver palavra na linha
        contPalavra++;
        if (strcmp(palavraAtual, palavra) == 0) { // Se a palavra atual for igual a palavra recebida
            printf("--------------------------------------\n");
            printf("|           ENCONTRADA EM:           |\n");
            printf("--------------------------------------\n");
            printf("  LINHA: %d COLUNA: %d (palavra n.%d)\n", i+1, coluna+1, contPalavra);
            printf("--------------------------------------\n\n");
            cont++;
            contaPalavra++;
        }
        coluna += strlen(palavraAtual) + 1; // Avança a coluna para o início da próxima palavra
        palavraAtual = strtok(NULL, " ,().:");  // Avança para próxima palavra
    }

    free(linha); // Libera a memória da strdup
  }

  // Feedback caso palavra não for encontrada
  if(!cont){
    printf("\n-------- ERRO --------\n");
    printf("| PALAVRA NAO EXISTE |\n");
    printf("----------------------\n\n");
  } else {
    printf("   ----------- RESUMO -----------\n");
    printf("   | PALAVRA ENCONTRADA %d VEZES |\n", contaPalavra);
    printf("   ------------------------------\n");
  }
  system("pause");
}

// Procedimento que transforma texto de várias linhas em uma frase
void transformaEmFrase(char texto[][82], char frase[], int fraseTam){
  strcpy(frase, "");  // Inicializa frase vazia

  // Itera por todas as linhas
  for (int i = 0; i < fraseTam; i++){
    char * temp = strdup(texto[i]); // Duplica linha do texto
    char * palavra = strtok(temp, " "); // Pega primeira palavra da linha
    while(palavra != NULL){ // Repete enquanto houver palavra na linha
      strcat(frase, palavra); // Concatena palavra na frase
      strcat(frase, " "); // Concatena um espaço
      palavra = strtok(NULL, " ");  // Passa para próxima palavra
    }
    free(temp); // Libera a memória da strdup
  }
}

// Menu de substituição de palavra
void menuSubstituir(char texto[][82], int fraseTam){
  int esc;
  char palavraAntiga[30], palavraNova[30];
  char frase[4000];
  while((esc < 1 || esc > 3) || (esc != 3)){
    system("cls");
    printf("------ # SUBSTITUIR PALAVRA # ------\n");
    printf("|1) SUBSTITUIR PRIMEIRA OCORRENCIA |\n");
    printf("|2) SUBSTITUIR TODAS OCORRENCIAS   |\n");
    printf("|3) VOLTAR                         |\n");
    printf("------------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    while(getchar() != '\n'); // Limpa o buffer
    system("cls");
    switch (esc){
      case 1:
        printf("#   SUBSTITUIR PRIMEIRA OCORRENCIA   #\n");
        printf("--------------------------------------\n");
        printf("PALAVRA QUE DESEJA SUBSTITUIR: ");
        scanf("%s", palavraAntiga);
        printf("--------------------------------------\n");
        printf("PALAVRA NOVA: ");
        scanf("%s", palavraNova);
        transformaEmFrase(texto, frase, fraseTam);
        pegaTexto(frase, texto, 1, palavraAntiga, palavraNova);
        break;
      case 2:
        printf("#   SUBSTITUIR TODAS OCORRENCIAS   #\n");
        printf("------------------------------------\n");
        printf("PALAVRA QUE DESEJA SUBSTITUIR: ");
        scanf("%s", palavraAntiga);
        printf("------------------------------------\n");
        printf("PALAVRA NOVA: ");
        scanf("%s", palavraNova);
        transformaEmFrase(texto, frase, fraseTam);
        pegaTexto(frase, texto, 2, palavraAntiga, palavraNova);
        break;
      case 3:
        break;
      default:
        printf("-------------------------\n");
        printf("|ERROR: ENTRADA INVALIDA|\n");
        printf("-------------------------\n");
        break;
    }
  }

}

// Procedimento que transforma todas as letras em maiúsculo
void paraMaiusculo(char texto[][82], int fraseTam){
  for (int i = 0; i < fraseTam; i++){
    for (int j = 0; j < 82; j++){
      texto[i][j] = toupper(texto[i][j]);
    }
  }
}

// Procedimento que transforma todas as letras em minúsculo
void paraMinusculo(char texto[][82], int fraseTam){
  for (int i = 0; i < fraseTam; i++){
    for (int j = 0; j < 82; j++){
      texto[i][j] = tolower(texto[i][j]);
    }
  }
}

// Procedimento que transforma todas as letras iniciais das linhas em maiúsculas
void primeiraParaMaiusculo(char texto[][82], int fraseTam){
  for (int i = 0; i < fraseTam; i++){
    texto[i][0] = toupper(texto[i][0]);
  }
}

// Menu de alteração do tamanho da caixa das palavras
void menuCaixa(char texto[][82], int fraseTam){
  int esc;
  while((esc < 1 || esc > 4) || (esc != 4)){
    system("cls");
    printf("------------------ # MODIFICAR CAIXA # ------------------\n");
    printf("|1) DEIXAR TODOS CARACACTERES EM MAIUSCULO              |\n");
    printf("|2) DEIXAR TODOS OS CARACTERES EM MINUSCULO             |\n");
    printf("|3) DEIXAR CARACTERES INICIAIS DAS FRASES EM MAIUSCULO  |\n");
    printf("|4) VOLTAR                                              |\n");
    printf("---------------------------------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    while(getchar() != '\n');
    system("cls");
    printf("  ---------------------------\n");
    switch (esc){
      case 1:
        paraMaiusculo(texto, fraseTam);
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
        break;
      case 2:
        paraMinusculo(texto, fraseTam);
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
        break;
      case 3:
        paraMinusculo(texto, fraseTam);
        primeiraParaMaiusculo(texto, fraseTam);
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
        break;
      case 4:
      break;
      default:
        printf("-------------------------\n");
        printf("|ERROR: ENTRADA INVALIDA|\n");
        printf("-------------------------\n");
        system("pause");
        break;
    }
  }
}

// Menu de alteração do alinhamento do texto
void menuAlinhamento(int *alinhamento){
  int esc;
  while((esc < 1 || esc > 5) || (esc != 5)){
    system("cls");
    printf("------- # MODIFICAR CAIXA # -------\n");
    printf("|1) ALINHAR TEXTO A ESQUERDA      |\n");
    printf("|2) ALINHAR TEXTO A DIREITA       |\n");
    printf("|3) CENTRALIZAR TEXTO             |\n");
    printf("|4) JUSTIFICAR TEXTO              |\n");
    printf("|5) VOLTAR                        |\n");
    printf("-----------------------------------\n");
    printf("SUA ESCOLHA: ");
    scanf("%d", &esc);
    while(getchar() != '\n');
    system("cls");
    printf("  ---------------------------\n");
    switch (esc){
      case 1:
        *alinhamento = 0;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
        break;
      case 2:
        *alinhamento = 1;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
        break;
      case 3:
        *alinhamento = 2;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
        break;
      case 4:
        *alinhamento = 3;
        printf("OPERACAO REALIZADA COM SUCESSO!\n");
        printf("  ---------------------------\n");
        system("pause");
      case 5:
        break;
      default:
        printf("-------------------------\n");
        printf("|ERROR: ENTRADA INVALIDA|\n");
        printf("-------------------------\n");
        break;
    }
  }
}

// Menu principal
void menuPrincipal(char texto[][82], int fraseTam){
  int esc;
  static int alinhamento = 0; // Variável permanente
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
    while (getchar() != '\n');
    system("cls");
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
        printf("-------------------------\n");
        printf("|ERROR: ENTRADA INVALIDA|\n");
        printf("-------------------------\n");
        system("pause");
        break;
    }
  }
}

int main(){
  char frase[] = "William Henry Gates III KBE GCIH (Seattle, 28 de outubro de 1955) mais conhecido como Bill Gates,\
  e um magnata, empresario, diretor executivo, investidor, filantropo e autor americano, que ficou conhecido por \
  fundar junto com Paul Allen a Microsoft a maior e mais conhecida empresa de software do mundo em termos de valor \
  de mercado. Gates ocupa atualmente o cargo de presidente nao-executivo da Microsoft alem de ser classificado \
  regularmente como a pessoa mais rica do mundo, posicao ocupada por ele de 1995 a 2007, 2009, e de 2014 a 2017. E \
  um dos pioneiros na revolucao do computador pessoal. Gates nasceu em uma familia de classe media de Seattle. Seu \
  pai, William H. Gates, era advogado de grandes empresas, e sua mae, Mary Maxwell Gates, foi professora da \
  Universidade de Washington e diretora de bancos. Bill Gates e as suas duas irmas, Kristanne e Libby, \
  frequentaram as melhores escolas particulares de sua cidade natal, e Bill tambem participou do \
  Movimento Escoteiro ainda quando jovem. Bill Gates, foi admitido na prestigiosa Universidade \
  Harvard, (conseguindo 1590 SATs dos 1600 possiveis) mas abandonou os cursos de Matematica e \
  Direito no terceiro ano para dedicar-se a Microsoft. Trabalhou na Taito com o desenvolvimento de \
  software basico para maquinas de jogos eletronicos (fliperamas) ate seus 16 anos. Tambem \
  trabalhou como pesquisador visitante na University of Massachusetts at Amherst, UMASS, Estados Unidos, quando \
  com 17 anos, desenvolveu junto com Paul Allen um software para leitura de fitas magneticas, com informacoes \
  de trafego de veiculos, em um chip Intel 8008. Com esse produto, Gates e Allen criaram uma empresa, \
  a Traf-o-Data, porem os clientes desistiram do negocio quando descobriram a idade dos donos. Enquanto \
  estudavam em Harvard, os jovens desenvolveram um interpretador da linguagem BASIC para um dos primeiros \
  computadores pessoais a serem lancado nos Estados Unidos - o Altair 8800. Apos um modesto sucesso na \
  comercializa��o deste produto, Gates e Allen fundaram a Microsoft, uma das primeiras empresas no mundo \
  focadas exclusivamente no mercado de programas para computadores pessoais ou PCs. Gates adquiriu ao \
  longo dos anos uma fama de visionario (apostou no mercado de software na epoca em que o hardware era \
  considerado muito mais valioso) e de negociador agressivo, chegando muitas vezes a ser acusado por \
  concorrentes da Microsoft de utilizar praticas comerciais desleais. Nos anos 1980, a IBM, lider \
  no mercado de grandes computadores, resolveu entrar no mercado da microinformatica com o PC, \
  porem faltava o Sistema Operacional. Para isso, fechou contrato com a recem-criada Microsoft. Todavia, \
  a Microsoft nao possuia o software ainda. O jovem Bill Gates foi a uma pequena empresa que havia \
  desenvolvido o sistema para o processador da Intel e decidiu compra-lo, pagou cerca de US$ 50 mil, personalizou \
  o programa e vendeu-o por US$ 8 milhoes, mantendo a licenca do produto. Este viria a ser o MS-DOS. \
  Fonte: https://pt.wikipedia.org/wiki/Bill_Gates";
  char texto[100][82];
  int fraseTam = strlen(frase)/75; // Descobre quantas linhas o texto terá
  pegaTexto(frase, texto, 0, "", "");
  menuPrincipal(texto, fraseTam);
  return 0;
}