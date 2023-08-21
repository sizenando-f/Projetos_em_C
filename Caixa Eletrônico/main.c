#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

char* geraContaCorrente();
char* gera_cpf_valido();

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
    for(int i = 0; i < 50; i++){
      for(int j = 0; j < 10; j++){
        contas[i][j] = '-';
      }
    }
    for(int i = 0; i < 50; i++){
      for(int j = 0; j < 15; j++){
        cpfs[i][j] = '-';
      }
    }
    for(int i = 0; i < 50; i++){
      for(int j = 0; j < 100; j++){
        hist_saques[i][j] = 0;
      }
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
      printf("-------------------------\n");
      printf("----- NOTAS SACADAS -----\n");
      if(rest >= 500){
        if(rest/500 < ced_quin){
          notas = rest/500;
          rest %= 500;
          ced_quin -= notas;
        } else {
          notas = ced_quin;
          rest -= 500*notas;
          ced_quin -= ced_quin;
        }
        printf("NOTAS DE 500: %d\n", notas);     
      } 
      if(rest >= 150){
        if(rest/150 < ced_centEcinq){
          notas = rest/150;
          rest %= 150;
          ced_centEcinq -= notas;
        } else {
          notas = ced_centEcinq;
          rest -= 150*notas;
          ced_centEcinq -= ced_centEcinq;
        }
        printf("NOTAS DE 150: %d\n", notas);     
      }
      if(rest >= 50){
        if(rest/50 < ced_cinq){
          notas = rest/50;
          rest %= 50;
          ced_cinq -= notas;
        } else {
          notas = ced_cinq;
          rest -= 50*notas;
          ced_cinq -= ced_cinq;
        }
        printf("NOTAS DE 50: %d\n", notas);     
      }
      if(rest >= 20){
        if(rest/20 < ced_vint){
          notas = rest/20;
          rest %= 20;
          ced_vint -= notas;
        } else {
          notas = ced_vint;
          rest -= 20*notas;
          ced_vint -= ced_vint;
        }
        printf("NOTAS DE 20: %d\n", notas);     
      }
      if(rest >= 10){
        if(rest/10 < ced_dez){
          notas = rest/10;
          rest %= 10;
          ced_dez -= notas;
        } else {
          notas = ced_dez;
          rest -= 10*notas;
          ced_dez -= ced_dez;
        }
        printf("NOTAS DE 10: %d\n", notas);     
      } 
      if(rest >= 5){
        if(rest/5 < ced_cinc){
          notas = rest/5;
          rest %= 5;
          ced_cinc -= notas;
        } else {
          notas = ced_cinc;
          rest -= 5*notas;
          ced_cinc -= ced_cinc;
        }
        printf("NOTAS DE 5: %d\n", notas);     
      } 
      if(rest >= 2){
        if(rest/2 < ced_dois){
          notas = rest/2;
          rest %= 2;
          ced_dois -= notas;
        } else {
          notas = ced_dois;
          rest -= 2*notas;
          ced_dois -= ced_dois;
        }
        printf("NOTAS DE 2: %d\n", notas);     
      } 
      if(rest >= 1){
        if(rest/1 < ced_um){
          notas = rest/1;
          rest %= 1;
          ced_um -= notas;
        } else {
          notas = ced_um;
          rest -= 1*notas;
          ced_um -= ced_um;
        }
        printf("NOTAS DE 1: %d\n", notas);     
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
      printf("------------------------------\n");
      printf("SALDO INSUFICIENTE!\n");
      printf("------------------------------\n");
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
    }
    printf("--------------------------------------------------------\n");
    printf("R$ ");
    numeroParaExtenso(soma_geral);
    printf("--------------------------------------------------------\n");
  } else if (codigo == 5){  // 5 = Alterar cpf ou conta
    int control = 0;
    for(int i = 0; i < clientes; i++){
      if(!strcmp(conta_corrente, contas[i])){
        if(num_saques[i] == 0){
          int esc = 0;
          printf("VOCE DESEJA ALTERAR CPF OU CONTA?\n1. CPF\n2. CONTA\n-> ");
          scanf("%d", &esc);
          printf("--------------------------\n");
          if(esc == 1){
            char* cpf = gera_cpf_valido();
            printf("SEU NOVO CPF EH: %s\n", cpf);
            if(strcmp(cpf, cpfs[i])){
              for(int j = 0; j < 15; j++){
                cpfs[i][j] = cpf[j];
              }
            } else {
              printf("CPF JA EXISTENTE, POR FAVOR, TENTE NOVAMENTE\n");
            }
          } else if (esc == 2){
            char* conta = geraContaCorrente();
            printf("SUA NOTA CONTA EH: %s\n", conta);
            if(strcmp(conta, contas[i])){
              for(int j = 0; j < 10; j++){
                contas[i][j] = conta[j];
              }
            } else {
              printf("CONTA JA EXISTENTE, POR FAVOR, TENTE NOVAMENTE\n");
            }
          } else {
            printf("ENTRADA INCORRETA\n");
          }
        } else {
          system("cls");
          printf("--------------------------\n");
          printf("NAO FOI POSSIVEL ALTERAR CONTA, SAQUE JA REALIZADO\n");
          printf("--------------------------\n");
        }
      } else {
        control++;
      }
    }

    if(control == clientes){
      printf("CONTA INEXISTENTE\n");
    }

    system("pause");
  } else if (codigo == 6){ // 6 = Remove cliente
    for(int i = 0; i < clientes; i++){
      if(!strcmp(conta_corrente, contas[i])){ // Descobre qual indice a conta se encontra
        if(num_saques[i] == 0){ // Se não houver saques
          for(int j = i; j < clientes; j++){ // Varre todos os clientes a partir da conta atual
            strcpy(cpfs[j], cpfs[j+1]);
            strcpy(contas[j], contas[j+1]);
            num_saques[j] = num_saques[j+1];
            for (int k = 0; k < num_saques[i+1]; k++) { // Move todos valores para esquerda
                hist_saques[j][k] = hist_saques[j+1][k];
            }
          }
          // Zera último indice de clientes
          memset(cpfs[clientes - 1], '-', sizeof(cpfs[0]));
          memset(contas[clientes - 1], '-', sizeof(contas[0]));
          num_saques[clientes - 1] = 0;
          memset(hist_saques[clientes - 1], 0, sizeof(hist_saques[0]));

          clientes--;

          system("cls");
          printf("///////\n REMOCAO REALIDA COM SUCESSO\n///////\n");
          system("pause");
        } else {
          system("cls");
          printf("--------- REMOCAO CANCELADA, SAQUE JA REALIZADO ---------\n");
        }
      }
    }
  } else if (codigo == 7){ // 7 = Saldo existente
    system("cls");
    printf("------------------------------\n");
    printf("R$%d,00\n", valor_total);
    printf("------------------------------\n");
    numeroParaExtenso(valor_total);
    printf("------------------------------\n");
  } else if (codigo == 8){ // 8 = Notas existentes
    system("cls");
    printf("CEDULA DE 500: %d -> ", ced_quin);
    numeroParaExtenso(ced_quin);
    printf("\n");
    printf("CEDULA DE 150: %d -> ", ced_centEcinq);
    numeroParaExtenso(ced_centEcinq);
    printf("\n");
    printf("CEDULA DE 50: %d -> ", ced_cinq);
    numeroParaExtenso(ced_cinq);
    printf("\n");
    printf("CEDULA DE 20: %d -> ", ced_vint);
    numeroParaExtenso(ced_vint);
    printf("\n");
    printf("CEDULA DE 10: %d -> ", ced_dez);
    numeroParaExtenso(ced_dez);
    printf("\n");
    printf("CEDULA DE 5: %d -> ", ced_cinc);
    numeroParaExtenso(ced_cinc);
    printf("\n");
    printf("CEDULA DE 2: %d -> ", ced_dois);
    numeroParaExtenso(ced_dois);
    printf("\n");
    printf("CEDULA DE 1: %d -> ", ced_um);
    numeroParaExtenso(ced_um);
    printf("\n");
    system("pause");
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
    printf("1. VALORES SACADOS\n2. VALOR DO SALDO EXISTENTE\n3. QUANTIDADE DE CEDULAS EXISTENTES\n4. VOLTAR AO MENU PRINCIPAL\n");
    printf("--------------------------\n");
    printf("INSIRA SUA ESCOLHA: ");
    scanf("%d", &esc);
    switch(esc){
      case 1:
        banco_de_dados("", "", 4, 0, 0);
        system("pause");
        break;
      case 2:
        banco_de_dados("", "", 7, 0, 0);
        system("pause");
        break;
      case 3:
        banco_de_dados("", "", 8, 0, 0);
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
      case 3:
        if(esc == 3){
          system("cls");
          char conta[10];
          int check;
          printf("--------------------------\n");
          printf("INSIRA A SUA CONTA: ");
          getchar();
          fgets(conta, sizeof(conta), stdin);
          printf("--------------------------\n");
          banco_de_dados("", conta, 2, &check, 0);
          if(check){
            banco_de_dados("", conta, 5, 0, 0);
          } else {
            printf("CONTA INEXISTENTE");
          }
        }
        break;
      case 4:
        system("cls");
        char conta[10];
        int check;
        printf("--------------------------\n");
        printf("INSIRA A SUA CONTA: ");
        getchar();
        fgets(conta, sizeof(conta), stdin);
        printf("--------------------------\n");
        banco_de_dados("", conta, 2, &check, 0);
        if(check){
          banco_de_dados("", conta, 6, 0, 0);
        } else {
          printf("CONTA INEXISTENTE");
        }
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
        printf("-------------------\nPROGRAMA ENCERRADO!\n-------------------\n");
      default:
        break;
    }
  }while((esc < 1 && esc > 4) || esc != 4);
}

int main(){
  menu_principal();
  return 0;
}

