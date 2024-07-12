# Determinação de Números Perfeitos

Este programa em C determina se um número fornecido pelo usuário é um número perfeito e lista seus divisores. Um número é considerado perfeito se for igual à soma de seus divisores (excluindo ele mesmo). Por exemplo, os divisores de 6 são 1, 2 e 3, e a soma desses divisores é igual a 6, logo, 6 é um número perfeito.

## Funcionalidades

- **Verificação de Número Perfeito**: O programa verifica se o número fornecido pelo usuário é um número perfeito.
- **Listagem de Divisores**: Se o número for perfeito, o programa lista todos os seus divisores.

## Estrutura do Código

### Funções

- `int nPerfeitos(int n, int vetor[100])`: Esta função determina se o número `n` é perfeito e armazena seus divisores no vetor fornecido. Retorna o número de divisores encontrados se `n` for perfeito, caso contrário, retorna 0.

  ```c
  int nPerfeitos(int n, int vetor[100]){
      int soma = 0, cont = 0;
      for(int i = 1; i < n; i++){
          if(n%i == 0){
              soma += i;
              vetor[cont] = i;
              cont++;
          }
      }
      if(soma == n){
          return cont;
      } else {
          return 0;
      }
  }
  ```

### Função Principal

- `int main()`: A função principal que solicita ao usuário um número, chama a função `nPerfeitos` para verificar se é um número perfeito, e exibe os divisores se for o caso.

  ```c
  int main(){
      int n;
      int vetor[100];

      printf("Insira um valor: ");
      scanf("%d", &n);

      int cont = nPerfeitos(n, vetor);

      if(cont){
          printf("Eh um numero perfeito, seus divisiveis sao: \n");
          for(int i = 0; i < cont; i++){
              printf("%d\n", vetor[i]);
          }
      } else {
          printf("Nao eh um numero perfeito");
      }
      return 0;
  }
  ```

## Uso

1. Compile o programa utilizando um compilador C, por exemplo:

   ```sh
   gcc -o perfeito main.c
   ```

2. Execute o programa:

   ```sh
   ./perfeito
   ```

3. Insira um número quando solicitado. O programa irá verificar se o número é perfeito e, em caso afirmativo, listar seus divisores.

## Exemplo de Execução

```
Insira um valor: 6
Eh um numero perfeito, seus divisiveis sao:
1
2
3
```

No exemplo acima, o número 6 é verificado como um número perfeito e seus divisores 1, 2 e 3 são listados.
