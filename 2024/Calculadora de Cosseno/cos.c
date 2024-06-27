// Autor: Sizenando S. França
// Última atualização: 06/04/2024

#include <stdio.h>  // Para entrada e saída (scanf, printf)
#include <math.h>   // Para número PI   (M_PI)
#include <windows.h>// Para pausas no programa e limpeza de tela  (system)
#include <time.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884197
#endif

// Cálculo de fatorial
double fat(int n) {
    if(n == 1){
        return 1;
    }

    return n * fat(n - 1);
}

// Converte graus para radiano
double to_radian(double x) {return (x * M_PI )/ 180;}; 

// Conserta grau recebidos para evitar overflow
double fix_dg(double dg){
    while(dg > 360){
        dg -= 360;
    }

    return dg;
}

// Conserta radiano recebido para evitar overflow
double fix_radian(double radian){
    while(radian > 6.283185307179586){
        radian -= 6.283185307179586;
    }

    return radian;
}

// Retorna cosseno a partir de um radiano
double cosen_rad(double rad, int n) {
    double res = 1.0;   // Varíavel onde será armazenado o resultado
    int exp = 2;    // Varíavel exponencial

    rad = rad < 0 ? rad*(-1) : rad; // Inverte sinal se radiano negativo, necessário para conserto de overflow
    rad = fix_radian(rad);  // Overflow é evitado

    // Valores padrões de entrada
    if(rad == 1.570796326794896 || rad == 4.712388980384689){
        return 0.0;
    } else if(rad == 0.0 || rad == 6.283185307179586){
        return 1.0;
    } else if(rad == 3.141592653589793){
        return -1.0;
    }

    // Verificação e tratamento de erro de entrada 
    if(n > 100){
        printf("\033[0;31m");
        printf("\n  [ #<- ] ERRO: Valor de N excedeu o limite, reduzindo para 100...\n");
        printf("\033[0;37m");
        n = 100;
    } else if(n < 0){
        printf("\033[0;31m");
        printf("\n  [ #<- ] ERRO: Valor de N eh menor 0, aumentando para 1...\n");
        printf("\033[0;37m");
        n = 1;
    }

    // Cálculo do cosseno
    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            res += pow(rad, exp) / fat(exp);
        } else {
            res -= pow(rad, exp) / fat(exp);
        }

        exp += 2;
    }

    return res;
}

// Retorna cosseno a partir de um grau
double cosen_dg(double dg, int n) {
    double res = 1.0;   // Varíavel onde será armazenado o resultado
    int exp = 2;    // Variável exponencial

    // Valores padrões de entrada
    if(dg == 90.0 || dg == 270.0){
        return 0.0;
    } else if(dg == 0.0 || dg == 360.0){
        return 1.0;
    } else if(dg == 18.0){
        return -1.0;
    }

    // Verificação e tratamento de erro de entrada
    if(n > 100){
        printf("\033[0;31m");
        printf("\n[ #<- ] ERRO: Valor de N excedeu o limite, reduzindo para 100...\n");
        printf("\033[0;37m");
        n = 100;
    } else if(n < 0){
        printf("\033[0;31m");
        printf("\n[ #<- ] ERRO: Valor de N eh menor 0, aumentando para 1...\n");
        printf("\033[0;37m");
        n = 1;
    }

    dg = dg < 0 ? dg*(-1) : dg; // Inverte sinal se grau negativo, necessário para tratamento de overflow
    double fixed_dg = fix_dg(dg);   // Overflow é evitado 
    double x = to_radian(fixed_dg); // Conversão de graus para radiano

    // Cálculo do cosseno
    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            res += pow(x, exp) / fat(exp);
        } else {
            res -= pow(x, exp) / fat(exp);
        }

        exp += 2;
    }

    return res;
}

int main() { 
    int n;  // Variável de número de repetições
    double resultado, tempo_gasto;   // Variável onde ficará o resultado final
    float dg, x;    // Variáveis para grau e radianos, respectivamente
    int entrada = 0;    // Inicialização da variável de escolha do menu
    struct timespec inicio, fim;
    do{
        system("cls");  // Limpa a tela
        // Exibe texto no menu  
        printf(" ________  ________  ________   ________  _______   ________   ________\n");
        printf("|\\   ____\\|\\   __  \\|\\   ____\\ |\\   ____\\|\\  ___ \\ |\\   ___  \\|\\   __  \\ \n");
        printf("\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\___|_\\ \\  \\___|\\ \\   __/|\\ \\  \\\\ \\  \\ \\  \\|\\  \\ \n");
        printf(" \\ \\  \\    \\ \\  \\\\\\  \\ \\_____  \\\\ \\_____  \\ \\  \\_|/_\\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \n");
        printf("  \\ \\  \\____\\ \\  \\\\\\  \\|____|\\  \\\\|____|\\  \\ \\  \\_|\ \\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \n");
        printf("   \\ \\_______\\ \\_______\\____\\_\\  \\ ____\\_\\  \\ \\_______\\ \\__\\\\ \\__\\ \\_______\\ \n");
        printf("    \\|_______|\\|_______|\\_________\\\\_________\\|_______|\\|__| \\|__|\\|_______| \n");
        printf("                       \\|_________\\|_________| \n");
        printf("\033[0;37m");

        // Menu
        printf("\033[0;34m[ 1 ]\033[0;37m GRAU\n");
        printf("\033[0;34m[ 2 ]\033[0;37m RADIANO\n");
        printf("\033[0;34m[ 3 ]\033[0;37m SAIR\n");
        printf("=============================\n");

        // Inserção da escolha do usuário
        printf("Escolha o tipo de entrada > ");
        printf("\033[1;34m");
        scanf("%d", &entrada);
        printf("\033[0;37m");

        switch (entrada){
            // Escolhe graus
            case 1:
                // Inserção do grau
                printf("[ <- ] Insira o valor em grau (ex.: 90) > ");
                getchar();
                printf("\033[1;34m");
                scanf("%f", &dg);
                printf("\033[0;37m");
                do{
                    // Inserção do número de repetições
                    printf("[ <- ] Insira o valor de N (max.: 100) > ");
                    getchar();
                    printf("\033[1;34m");
                    scanf("%d", &n);
                    printf("\033[0;37m");

                    // N não pode ser 0
                    if(n == 0){
                        printf("\033[0;31m");
                        printf("[ #<- ] ERRO: Insira outro valor diferente de 0\n");
                        printf("\033[0;37m");
                    }
                }while(n == 0);
                clock_gettime(CLOCK_MONOTONIC, &inicio);
                resultado = cosen_dg(dg, n);    // Cálculo do cosseno
                clock_gettime(CLOCK_MONOTONIC, &fim);
                tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec)/1E9;
                printf("\033[0;32m");
                printf("\n[ -> ] Cosseno de \033[4;32m%.2f\033[0;32m graus:\033[1;32m %.15f\n", dg, resultado); // Exibição do resultado
                printf("\033[0;37m");
                printf("[ -> ] Tempo gasto: %f\n", tempo_gasto);    
                break;
            // Escolhe radiano
            case 2:
                // Inserção do radiano
                printf("[ <- ] Insira o valor em radiano (ex.: 1.570796) > ");
                getchar();
                printf("\033[1;34m");
                scanf("%f", &x);
                printf("\033[0;37m");
                do{
                    // Inserção do número de repetições
                    printf("[ <- ] Insira o valor de N (max.: 100) > ");
                    getchar();
                    printf("\033[1;34m");
                    scanf("%d", &n);
                    printf("\033[0;37m");
                    // N não pode ser 0
                    if(n == 0){
                        printf("\033[0;31m");
                        printf("[ #<- ] ERRO: Insira outro valor diferente de 0\n");
                        printf("\033[0;37m");
                    }
                } while(n == 0); 
                clock_gettime(CLOCK_MONOTONIC, &inicio);
                resultado = cosen_rad(x, n);    // Cálculo do cosseno
                clock_gettime(CLOCK_MONOTONIC, &fim);
                tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec)/1E9;
                printf("\033[0;32m");
                printf("\n[ -> ] Cosseno de \033[4;32m%f\033[0;32m radianos:\033[1;32m %.15f\n", x, resultado);   // Exbição do resultado
                printf("\033[0;37m");
                printf("[ -> ] Tempo gasto: %f\n", tempo_gasto);
                break;
            // Escolhe sair do programa
            case 3:
                printf("\033[0;32m");
                printf("\n[ -> ] Programa encerrado\n");    // Mensagem de confirmação
                printf("\033[0;37m");
                break;
            // Escolhe uma opção inexistente
            default:
                printf("\033[0;31m");
                printf("\n[ #<- ] ERRO: Opcao invalida, tente novamente\n");    // Mensagem de aviso
                printf("\033[0;37m");
                break;
        }
        system("pause");
    }while(entrada != 3);

    return 0;
}