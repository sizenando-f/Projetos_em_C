# Encontrar o Maior Quadrado Menor ou Igual a N

## Descrição

Este programa em C solicita um valor inteiro e positivo N do usuário e encontra o maior quadrado que seja menor ou igual a N. Caso o valor de N não seja válido (negativo), o programa emite uma mensagem de erro apropriada.

## Funcionalidades

- **Entrada de Dados:** Solicita ao usuário que insira um valor inteiro e positivo N.
- **Cálculo do Maior Quadrado:** Utiliza a função matemática `sqrt` para calcular a raiz quadrada de N e determinar o maior quadrado que se encaixa nesse critério.
- **Verificação de Validade:** Verifica se o valor inserido é positivo. Caso contrário, emite uma mensagem de erro.
- **Exibição de Saída:** Apresenta o maior quadrado encontrado que é menor ou igual a N, formatado conforme especificado em `SAIDA_1`, ou mensagens de erro em casos inválidos (`SAIDA_2` e `SAIDA_3`).

## Funcionamento

1. **Compilação e Execução:** Compile o programa em um ambiente de desenvolvimento C.
2. **Entrada de Dados:** Insira um valor inteiro e positivo para N quando solicitado pelo programa.
3. **Cálculo e Saída:** O programa determinará o maior quadrado menor ou igual a N e o exibirá. Se N for negativo ou se forem fornecidos argumentos inválidos, o programa exibirá a mensagem apropriada.

## Como Compilar

Para compilar o programa em um ambiente Unix/Linux, utilize o seguinte comando:

```bash
gcc main.c -o maior_quadrado -lm
```

Certifique-se de ter um compilador C instalado no seu sistema. O parâmetro `-lm` é necessário para incluir a biblioteca matemática.

## Observações

Este programa foi desenvolvido como parte de um exercício para demonstrar habilidades em manipulação de entrada de dados, cálculos matemáticos e controle de fluxo em linguagem C.
