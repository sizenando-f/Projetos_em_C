#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

// Função para imprimir parte do número por extenso
void imprimirParte(int num) {
    // Possíveis escritas por extenso
    char *unidade[] = {"", "UM", "DOIS", "TRES", "QUATRO", "CINCO", "SEIS", "SETE", "OITO", "NOVE"};
    char *dezAteVinte[] = {"DEZ", "ONZE", "DOZE", "TREZE", "CATORZE", "QUINZE", "DEZESSEIS", "DEZESSETE", "DEZOITO", "DEZENOVE"};
    char *dezena[] = {"", "", "VINTE", "TRINTA", "QUARENTA", "CINQUENTA", "SESENTA", "SETENTA", "OITENTA", "NOVENTA"};
    char *centena[] = {"", "CENTO", "DUZENTOS", "TREZENTOS", "QUATROCENTOS", "QUINHENTOS", "SEISCENTOS", "SETECENTOS", "OITOCENTOS", "NOVECENTOS"};

    if (num >= 100) {
        printf("%s", centena[num / 100]);
        num %= 100;
        if (num == 0) {
            return;
        } else {
            printf(" E ");
        }
    }

    if (num >= 20) {
        printf("%s", dezena[num / 10]);
        num %= 10;
        if (num == 0) {
            return;
        } else {
            printf(" E ");
        }
    }

    if (num >= 10) {
        printf("%s", dezAteVinte[num - 10]);
    } else if (num > 0) {
        printf("%s", unidade[num]);
    }
}

// Função para converter um número para extenso
void numeroParaExtenso(int num) {
    if (num < 0 || num > 180000) {
        printf("NUMERO FORA DO INTERVALO SUPORTADO.\n");
        return;
    }

    if (num == 0) {
        printf("ZERO\n");
        return;
    }

    if (num >= 1000) {
        imprimirParte(num / 1000);
        printf(" MIL");
        num %= 1000;
        if (num == 0) {
            printf("\n");
            return;
        } else {
            printf(" E ");
        }
    }

    imprimirParte(num);
    printf("\n");
}

// Procedimento responsável por armazenar todos os dados de clientes
void banco_de_dados(char* cpf, char* conta_corrente, int codigo, int *check, int valor_saque){
  static int clientes = 0;
  static char cpfs[50][15];
  static char contas[50][10];
  static int num_saques[50];
  static int hist_saques[50][100];

  // Bloco responsável por zerar os vetores
  static int control = 0;
  if(!control){
    for(int i = 0; i < 50; i++){
      num_saques[i] = 0;
    }
  }
  control = 1;

  static int ced_quin = 100, ced_centEcinq = 200, ced_cinq = 400, ced_vint = 800, ced_dez = 1600, ced_cinc = 3200, ced_dois = 6400, ced_um = 12800;
  int ced_total = ced_quin + ced_centEcinq + ced_cinq + ced_vint + ced_dez + ced_cinc + ced_dois + ced_um;
  int valor_total = (ced_quin*500) + (ced_centEcinq*150) + (ced_cinq*50) + (ced_vint*20) + (ced_dez*10) + (ced_cinc*5) + (ced_dois*2) + (ced_um);

  // O "código" refere-se ao tipo de tratamento que os dados recebidos irão ter
  if(codigo == 0){ // 0 = Criar conta
    if(clientes != 0){
      int existe_conta = 0, existe_cpf = 0;

      // Checa se conta ou CPF existe
      for(int i = 0; i < clientes; i++){
        if(!strcmp(cpf, cpfs[i])){
          existe_cpf = 1;
        }
        if(!strcmp(conta_corrente, contas[i])){
          existe_conta = 1;
        }
      }

      if(existe_cpf || existe_conta){
        printf("! CONTA EXISTENTE, TENTE NOVAMENTE !");
      } else {
        for(int i = 0; i < 15; i++){
          cpfs[clientes][i] = cpf[i];
        }
        for(int i = 0; i < 10; i++){
          contas[clientes][i] = conta_corrente[i];
        }
        clientes++;
        system("cls");
        printf("//// PROCESSO EFETUADO COM SUCESSO ////\n");
      }
    } else {
      for(int i = 0; i < 15; i++){
        cpfs[clientes][i] = cpf[i];
      }
      for(int i = 0; i < 10; i++){
        contas[clientes][i] = conta_corrente[i];
      }
      clientes++;
      system("cls");
      printf("//// PROCESSO EFETUADO COM SUCESSO ////\n");
    }
  } else if (codigo == 1){  // 1 = Mostrar Clientes
    system("cls");
    printf("--- CLIENTES CADASTRADOS (%d) ---\n", clientes);
    if(clientes){
      for(int i = 0; i < clientes; i++){
        printf("\n/// CLIENTE %d ///\n", i+1);
        printf("CPF:     %s\nCONTA:   %s\n", cpfs[i], contas[i]);
      }
    } else {
      printf("! NAO EXISTEM CLIENTES CADASTRADO !\n");
    }
    system("pause");
  } else if (codigo == 2){  // 2 = Verifica se conta existe
    for(int i = 0; i < clientes; i++){
      if(!strcmp(conta_corrente, contas[i])){
        *check = 1;
      }
    }
  } else if (codigo == 3){ // 3 = Sacar
    if(valor_total >= valor_saque){
      int rest = valor_saque;
      int notas;
      printf("----- NOTAS SACADAS -----\n");
      if(rest >= 500 && (rest/500 < ced_quin)){
        notas = rest/500;
        printf("NOTAS DE 500: %d\n", notas);
        rest %= 500;
        ced_quin -= notas;
      } if (rest >= 150 && (rest/150 < ced_centEcinq)){
        notas = rest/150;
        printf("NOTAS DE 150: %d\n", notas);
        rest %= 150;
        ced_centEcinq -= notas;
      }  if (rest >= 50 && (rest/50 < ced_cinq)){
        notas = rest/50;
        printf("NOTAS DE 50: %d\n", notas);
        rest %= 50;
        ced_cinq -= notas;
      }  if (rest >= 20 && (rest/20 < ced_vint)){
        notas = rest/20;
        printf("NOTAS DE 20: %d\n", notas);
        rest %= 20;
        ced_vint -= notas;
      }  if (rest >= 10 && (rest/10 < ced_dez)){
        notas = rest/10;
        printf("NOTAS DE 10: %d\n", notas);
        rest %= 10;
        ced_dez -= notas;
      }  if (rest >= 5 && (rest/5 < ced_cinc)){
        notas = rest/5;
        printf("NOTAS DE 5: %d\n", notas);
        rest %= 5;
        ced_cinc -= notas;
      } if (rest >= 2 && (rest/2 < ced_dois)){
        notas = rest/2;
        printf("NOTAS DE 2: %d\n", notas);
        rest %= 2;
        ced_dois -= notas;
      } if (rest >= 1 && (rest/1 < ced_um)){
        notas = rest/1;
        printf("NOTAS DE 1: %d\n", notas);
        rest %= 1;
        ced_um -= notas;
      }
      printf("--------------------\nSAQUE POR EXTENSO: ");
      numeroParaExtenso(valor_saque);
      printf("--------------------\n");
      for(int i = 0; i < clientes; i++){
        if(!strcmp(conta_corrente, contas[i])){
          hist_saques[i][num_saques[i]] = valor_saque;
          num_saques[i]++;
        }
      }  
    } else {
      printf("SALDO INSUFICIENTE!");
    }
  } else if (codigo == 4){  // 4 = Ver valores sacados
    system("cls");
    printf("--------- RELATORIOS - VALORES SACADOS ---------\n");
    int soma_geral = 0;
    for(int i = 0; i < clientes; i++){
      int soma = 0;
      printf("%s - %s\n", contas[i], cpfs[i]);
      for(int j = 0; j < num_saques[i]; j++){
        printf("                           R$%d,00\n", hist_saques[i][j]);
      }
      for(int l = 0; l < num_saques[i]; l++){
          soma += hist_saques[i][l];
        }
      soma_geral += soma;
      printf("                                       R$%d,00\n", soma);
      printf("\n");
      printf("--------------------------------------------------------\n");
      printf("R$ ");
      numeroParaExtenso(soma_geral);
      printf("--------------------------------------------------------\n");
    }
  }
}

// Retorna uma letra maiúscula aleatória do alfabeto
char gera_letra_aleatoria() {  
    return 'A' + rand() % 26;
}

// Gera uma conta corrente formatada
char* geraContaCorrente() {
    srand(time(NULL));
    int numeroConta = rand() % 900000 + 100000; // Gera um número entre 100000 e 999999
    char letraVerificadora = gera_letra_aleatoria();
    static char conta_corrente[10];
    sprintf(conta_corrente, "%03d.%03d-%c" , (numeroConta / 1000), (numeroConta % 1000), letraVerificadora);

    return conta_corrente;
}

// Menu de relatório
void menu_relatorios(){
  int esc;
  do{
    system("cls");
    printf("----- MENU RELATORIOS -----\n");
    printf("1. VALORES SACADOS\n2. VALOR DO SALDO EXISTENTE\n3. QUANTIDADE DE CÉDULAS EXISTENTES\n4. VOLTAR AO MENU PRINCIPAL\n");
    printf("--------------------------\n");
    printf("INSIRA SUA ESCOLHA: ");
    scanf("%d", &esc);
    switch(esc){
      case 1:
        banco_de_dados("", "", 4, 0, 0);
        system("pause");
        break;
      default:
        break;
    }
  }while((esc < 1 && esc > 4) || esc != 4);
}

// Retorna um CPF valido e formatado
char* gera_cpf_valido() {
    int soma = 0, resto = 0;
    int multiplicador1[9] = { 10, 9, 8, 7, 6, 5, 4, 3, 2 };
    int multiplicador2[10] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 };

    srand(time(NULL));
    char semente[14];
    // Gera valor aleatório, formata para ter 9 dígitos e insere no vetor
    sprintf(semente, "%09d", rand() % 900000000 + 100000000);

    for (int i = 0; i < 9; i++)
        // Mulitiplica cada número do CPF pelo mulitiplicador respectivo
        soma += (semente[i] - '0') * multiplicador1[i];

    resto = soma % 11;
    // Gera primeiro dígito verificador
    if (resto < 2)
        resto = 0;
    else
        resto = 11 - resto;

    // Insere primeiro dígito verificador no vetor
    sprintf(semente + 9, "%d", resto);
    soma = 0;

    for (int i = 0; i < 10; i++)
        // Mulitiplica cada número do CPF pelo mulitiplicador respectivo
        soma += (semente[i] - '0') * multiplicador2[i];

    resto = soma % 11;
    // Gera segundo dígito verificador
    if (resto < 2)
        resto = 0;
    else
        resto = 11 - resto;

    // Insere primeiro dígito verificador no vetor
    sprintf(semente + 10, "%d", resto);


    // Formata CPF no modelo 999.999.999-99
    static char cpf_formatado[15];
    sprintf(cpf_formatado, "%c%c%c.%c%c%c.%c%c%c-%c%c", semente[0], semente[1], semente[2], semente[3], semente[4], semente[5], semente[6], semente[7], semente[8], semente[9], semente[10]);

    return cpf_formatado;
}

// Menu do cliente
void menu_cliente(){
  int esc;

  do{
    system("cls");
    printf("----- MENU CLIENTE -----\n");
    printf("1. INCLUIR\n2. MOSTRAR\n3. ALTERAR\n4. EXLUIR\n5. VOLTAR\n");
    printf("--------------------------\n");
    printf("INSIRA SUA ESCOLHA: ");
    scanf("%d", &esc);
    switch (esc)
    {
      case 1:
        system("cls");

        if(esc == 1){
          char* cpf = gera_cpf_valido();
          char* conta_corrente = geraContaCorrente();
          int opc;
          printf("---------------------\n");
          printf("CPF GERADO: %s\n", cpf);
          printf("CONTA GERADA: %s\n", conta_corrente);
          
          do{
            printf("\n----- DESEJA CONTINUAR? -----\n1. SIM\n2. NAO\n-> ");
            scanf("%d", &opc);
            switch(opc){
              case 1:
                banco_de_dados(cpf, conta_corrente, 0, 0, 0);
                break;
              case 2:
                printf("---------------------\nOPERACAO CANCELADA\n---------------------\n");
                break;
              default:
                printf("---------------------\nOPCAO INVALIDA\n---------------------\n");
                break;
            }
          } while((opc < 1) || (opc > 2));

        }
        system("pause");
        break;
      case 2:
        banco_de_dados("", "", 1, 0, 0);
        break;
      default:
        break;
    }
  }while((esc < 1 && esc > 5) || esc != 5);
}

// Menu principal
void menu_principal(){
  int esc;
  do{
    system("cls");
    printf("----- MENU PRINCIPAL -----\n");
    printf("1. CLIENTE\n2. SAQUE\n3. RELATORIOS\n4. FINALIZAR\n");
    printf("--------------------------\n");
    printf("INSIRA SUA ESCOLHA: ");
    scanf("%d", &esc);
    switch (esc)
    {
      case 1:
        menu_cliente();
        break;
      case 2:
        if(esc == 2){
          int saque_valor;
          char conta[10];
          int check = 0;

          system("cls");
          printf("----------------------\n(formato aceito: 999.999-X)\nINSIRA SUA CONTA CORRENTE: ");
          getchar();
          fgets(conta, sizeof(conta), stdin);
          banco_de_dados("", conta, 2, &check, 0);
          if(check){
            system("cls");
            printf("-------------------\n  CONTA EXISTENTE\n-------------------\nINSIRA O VALOR DO SAQUE: ");
            scanf("%d", &saque_valor);
            banco_de_dados("", conta, 3, 0, saque_valor);
          } else {
            system("cls");
            printf("-------------------\nCONTA INEXISTENTE, POR FAVOR, TENTE NOVAMENTE\n-------------------\n");
          }
          system("pause");
        }
        break;
      case 3:
        menu_relatorios();
        break;
      case 4:
        printf("-------------------\n     PROGRAMA ENCERRADO!\n-------------------\n");
      default:
        break;
    }
  }while((esc < 1 && esc > 4) || esc != 4);
}

int main(){
  menu_principal();
  return 0;
}

