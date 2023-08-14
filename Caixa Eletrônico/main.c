#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

// Procedimento responsável por armazenar todos os dados de clientes
void banco_de_dados(char* cpf, char* conta_corrente, int codigo){
  static int clientes = 0;
  static char cpfs[50][15];
  static char contas[50][10];

  // O código refere-se ao tipo de tratamento que os dados recebidos irão ter
  if(codigo == 0){
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
  } else if (codigo == 1){
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

          printf("CPF GERADO: %s\n", cpf);
          printf("CONTA GERADA: %s\n", conta_corrente);
          
          do{
            printf("\n----- DESEJA CONTINUAR? -----\n1. SIM\n2. NAO\n-> ");
            scanf("%d", &opc);
            switch(opc){
              case 1:
                banco_de_dados(cpf, conta_corrente, 0);
                break;
              case 2:
                printf("OPERACAO CANCELADA\n");
                break;
              default:
                printf("OPCAO INVALIDA\n");
                break;
            }
          } while((opc < 1) || (opc > 2));

        }
        system("pause");
        break;
      case 2:
        banco_de_dados("", "", 1);
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
      case 3:
        menu_relatorios();
        break;
      case 4:
        printf("PROGRAMA ENCERRADO!");
      default:
        break;
    }
  }while((esc < 1 && esc > 4) || esc != 4);
}

int main(){
  menu_principal();
  return 0;
}

