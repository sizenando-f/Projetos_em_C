#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#define TAM 50

char opcionais[][TAM]={ {"4.portas"}, {"cambio.automatico"}, {"vidros.eletricos"}, {"abs"}, {"air.bag"}, {"ar.condicionado"},
  {"banco.couro"}, {"sensor.estacionamento"}};

int nAleatorio(int menor, int maior){
  srand(time(NULL));
  return menor + (rand() % (maior - menor + 1));
}

struct CARRO {
  char placa[9]; //AAA-1234
  char modelo[TAM]; //gol, celta, palio, ...
  char fabricante[TAM]; //chevrolet, fiat, ...
  int ano_fabricacao; //1980 à 2016
  int ano_modelo; //1980 à 2017
  char combustivel[TAM]; //alcool, gasolina, flex, diesel
  char cor[TAM]; //branca, prata, preta
  int opcional[8]; //ver matriz opcionais
  float preco_compra;
};

struct ENDERECO {
  char rua[TAM];
  int numero;
  char bairro[TAM];
  char cidade[TAM];
  char estado[3];
  char cep[11]; //99.999-999
};

struct TELEFONE {
 char telefone[14]; //99 99999-9999
};

struct CLIENTE {
  char nome[TAM];
  char cpf[15]; //999.999.999-99
  struct ENDERECO endereco;
  struct TELEFONE residencial;
  struct TELEFONE celular[5];
  float renda_mensal;
};

struct DATA {
  int dia, mes, ano;
};

struct VENDA_CARRO {
  char placa_car[9];
  char cpf_cli[15];
  float preco_venda;
  struct DATA data_venda;
};

void geraCarroInfo(char * modelo, char * fabricante, int * anoFab, int * anoMod, char * combustivel, char * cor, int * opcional, float * preco){
  char * modelos[] = {"onix", "s10", "celta", "strada", "palio", "corolla", "hillux", "saveiro", "voyage", "gol"};
  // srand(time(NULL));

  strcpy(fabricante, "");

  int n = nAleatorio(0, 9);
  strcpy(modelo, modelos[n]);
  if(n < 3){
    strcpy(fabricante, "chevrolet");
  } else if(n < 5){
    strcpy(fabricante, "fiat");
  }else if(n < 7){
    strcpy(fabricante, "toyota");
  } else {
    strcpy(fabricante, "volkswagen");
  }

  switch (n){
    case 0:
      *anoMod = 2011 + (rand() % (2023 - 2011 + 1)); 
      *anoFab = 2011; 
      break;
    case 1:
      *anoFab = 1993; 
      *anoMod = 1993 + (rand() % (2023 - 1993 + 1)); 
      break;
    case 2:
      *anoFab = 2000; 
      *anoMod = 2000 + (rand() % (2023 - 2000 + 1)); 
      break;
    case 3:
      *anoFab = 1997; 
      *anoMod = 1997 + (rand() % (2023 - 1997 + 1)); 
      break;
    case 4:
      *anoFab = 1996; 
      *anoMod = 1996 + (rand() % (2023 - 1996 + 1)); 
      break;
    case 5:
      *anoFab = 2000; 
      *anoMod = 2000 + (rand() % (2023 - 2000 + 1)); 
      break;
    case 6:
      *anoFab = 1984; 
      *anoMod = 1984 + (rand() % (2023 - 1984 + 1)); 
      break;
    case 7:
      *anoFab = 2008; 
      *anoMod = 2008 + (rand() % (2023 - 2008 + 1)); 
      break;
    case 8:
      *anoFab = 1982; 
      *anoMod = 1982 + (rand() % (2023 - 1982 + 1)); 
      break;
    case 9:
      *anoFab = 1991;
      *anoMod = 1991 + (rand() % (2023 - 1991 + 1));
    default:
      break;
  }

  char * combustiveis[] = {"flex", "gasolina", "alcool", "diesel"};
  strcpy(combustivel, combustiveis[nAleatorio(0, 3)]);

  char * cores[] = {"preto", "marrom", "branco", "prata", "vermelho", "amarelo", "cinza", "laranja", "verde"};
  strcpy(cor, cores[nAleatorio(0, 8)]);

  *opcional = nAleatorio(0, 7);
  *preco = nAleatorio(6000, 190000);
}

void geraPlaca(char * placa){
  char letras[27][2];
  char *numeros[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

  for(int i = 0; i < 26; i++){
    letras[i][0] = 'A' + i;
    letras[i][1] = '\0';
  }

  srand(time(NULL));

  for(int i = 0; i < 3; i++){
    int nAleatorio = rand() % 26;
    strcat(placa, letras[nAleatorio]);
  }
  strcat(placa, "-");
  for(int i = 0; i < 3; i++){
    int nAleatorio = rand() % 9;
    strcat(placa, numeros[nAleatorio]);
  }

}

void inserirCarro(){
  char placa[9], modelo[11], fabricante[11], combustivel[8], cor[10], esc;
  int anoFab, anoMod, opcional;
  float preco;
  struct CARRO car;

  geraPlaca(placa);
  geraCarroInfo(modelo, fabricante, &anoFab, &anoMod, combustivel, cor, &opcional, &preco);
  do{
    system("cls");
    printf("--- DESEJA O SEGUINTE CARRO? ---\n");
    printf("PLACA: %s\n", placa);
    printf("MODELO: %s\n", modelo);
    printf("FABRICANTE: %s\n", fabricante);
    printf("ANO DE FABRICACAO: %d\n", anoFab);
    printf("ANO DO MODELO: %d\n", anoMod);
    printf("COMBUSTIVEL: %s\n", combustivel);
    printf("COR: %s\n", cor);
    printf("OPCIONAL: %s\n", opcionais[opcional]);
    printf("PRECO DE COMPRA: %.2f\n", preco);
    printf("S/N -> ");
    scanf(" %c", &esc);

    esc = toupper(esc);
    if(esc == 'S'){
      strcpy(car.placa, placa);
      strcpy(car.modelo, modelo);
      strcpy(car.fabricante, fabricante);
      car.ano_fabricacao = anoFab;
      car.ano_modelo = anoMod;
      strcpy(car.combustivel, combustivel);
      strcpy(car.cor, cor);
      car.opcional[0] = opcional;
      car.preco_compra = preco;
      FILE * fp = fopen("carros.bin", "ab");

      if(fp == NULL){
        printf("ERRO DE ABERTURA DE ARQUIVO!\n");
        exit(100);
      }

      fwrite(&car, sizeof(car), 1, fp);
      fclose(fp);

    } else if(esc == 'N'){
      printf("OPERACAO CANCELADA!\n");
      Sleep(2);
    } else {
      printf("ESCOLHA INVALIDA!\n");
      Sleep(2);
    }
  } while(esc != 'S' && esc != 'N');
}

void menuCarro(){
  int esc;
  do{
    system("cls");
    printf("1. INSERIR UM CARRO\n");
    printf("2. EXCLUIR UM CARRO\n");
    printf("3. LISTAR CARROS DISPONIVEIS PARA VENDA ORDENADOS POR FABRICANTE E MODELO\n");
    printf("4. LISTAR CAROS DISPONIVEIS PARA VENDA POR SELECAO DE UM OU MAIS OPCIONAIS\n");
    printf("5. LISTAR OS CARROS DISPONIVEIS PARA VENDA POR SELECAO DA FAIXA DO ANO DE FABRICACAO\n");
    printf("6. SAIR\n");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        inserirCarro();
        break;
      case 2:
        break;
      default:
        break;
    }
  } while(esc != 6);
}

char * gera_nome(){
  char *nomes[20] = {"Alice", "Bob", "Carol", "David", "Eve", "Frank", "Grace", "Hannah", "Isaac", "Jack", "Katherine", "Liam", "Mia", "Noah", "Olivia", "Peter", "Quinn", "Rachel", "Samuel", "Taylor"};
  static char nome[20];

  strcpy(nome, nomes[nAleatorio(0, 19)]);
  return nome;
}

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

char * gera_cep(){
  static char cep[11];
  sprintf(cep, "%d%d.%d%d%d-%d%d%d", nAleatorio(0,9), nAleatorio(0,9), nAleatorio(0,9), nAleatorio(0,9), nAleatorio(0,9), nAleatorio(0,9), nAleatorio(0,9), nAleatorio(0,9));
  return cep;
}

struct ENDERECO gera_endereco(){
  char * ruas[20] = { "Rua A", "Rua B", "Rua C", "Rua D", "Rua E", "Rua F", "Rua G", "Rua H", "Rua I", "Rua J", "Rua K", "Rua L", "Rua M", "Rua N", "Rua O", "Rua P", "Rua Q", "Rua R", "Rua S", "Rua T" };
  char * bairros[20] = { "Bairro da Amizade", "Bairro das Flores", "Bairro da Liberdade", "Bairro do Sol", "Bairro Esperança", "Bairro Felicidade", "Bairro Harmonia", "Bairro Primavera", "Bairro Recanto Verde", "Bairro Sossego", "Bairro Vila Alegre", "Bairro Vista Bela", "Bairro Novo Horizonte", "Bairro Jardim Azul", "Bairro Vale Encantado", "Bairro do Lago", "Bairro das Montanhas", "Bairro da Lua", "Bairro da Paz", "Bairro Estrela Brilhante" };
  char * cidades[20] = { "Cidade das Maravilhas", "Cidade Encantada", "Cidade Radiante", "Cidade dos Sonhos", "Cidade Luz", "Cidade da Alegria", "Cidade da Esperança", "Cidade Mágica", "Cidade das Cores", "Cidade Bela Vista", "Cidade Serrana", "Cidade Oásis", "Cidade do Futuro", "Cidade Celestial", "Cidade Paradisíaca", "Cidade do Arco-Íris", "Cidade das Marés", "Cidade das Estrelas", "Cidade dos Ventos", "Cidade Harmoniosa" };
  char * estados[20] = { "AC", "AL", "AP", "AM", "BA", "CE", "DF", "ES", "GO", "MA", "MT", "MS", "MG", "PA", "PB", "PR", "PE", "PI", "RJ", "RN" };
  static struct ENDERECO endereco;
  strcpy(endereco.rua, ruas[nAleatorio(0, 19)]);
  strcpy(endereco.bairro, bairros[nAleatorio(0, 19)]);
  strcpy(endereco.cidade, cidades[nAleatorio(0, 19)]);
  strcpy(endereco.estado, estados[nAleatorio(0, 19)]);
  strcpy(endereco.cep, gera_cep());
  endereco.numero = nAleatorio(100, 9999);
  return endereco;
}

void inserirCliente(){
  char * nome = gera_nome();
  char * cpf = gera_cpf_valido();
  char esc;
  do{
    printf("--- VOCE DESEJA INSERIR O SEGUINTE CLIENTE? ---\n");
    printf("NOME: %s\n", nome);
    printf("CPF: %s\n", cpf);
    printf("S/N -> ");
    scanf(" %c", &esc);
    esc = toupper(esc);
  } while(esc != 'S' && esc != 'N');
}

void menuCliente(){
  int esc;
  do{
    printf("1. INSERIR UM CLIENTE\n");
    printf("2. EXCLUIR UM CLIENTE\n");
    printf("3. LISTAR OS CLIENTES ORDENADOS POR NOME\n");
    printf("4. LISTAR OS CLIENTES ORDENADOS PELA FAIXA DE RENDA SALARIAL MENSAL\n");
    printf("5. SAIR\n");
    scanf("%d", &esc);
    switch (esc){
      case 1:
        inserirCliente();
        break;
      default:
        break;
    }
  }while(esc != 5);
}

int main(){
  int esc;
  do{
    system("cls");
    printf("1. CARRO\n");
    printf("2. CLIENTE\n");
    printf("3. VENDA\n");
    printf("4. SAIR\n");
    scanf("%d", &esc);

    switch (esc){
      case 1:
        menuCarro();
        break;
      case 2:
        menuCliente();
        break;
      default:
        break;
    }
  } while(esc != 4);

  return 0;
}