# Banco de Dados de Clientes

Este é um projeto em C para gerenciar um banco de dados de clientes com funcionalidades de criação, exclusão, modificação e saque de contas correntes. Este projeto foi desenvolvido como parte do Trabalho 3 de Algoritmos e Estruturas de Dados (AED) por Sizenando Souza França, Felipe Natan de Oliveira Lima, e Weliton Fernandes Alves.

## Funcionalidades

1. **Criação de Contas**
2. **Listagem de Clientes**
3. **Verificação de Existência de Conta**
4. **Saque**
5. **Relatórios de Saques**
6. **Alteração de CPF ou Conta Corrente**
7. **Remoção de Cliente**
8. **Visualização de Saldo Total**
9. **Visualização de Notas Existentes**

## Como Usar

### Compilação e Execução

Para compilar e executar o programa, use os seguintes comandos:

```sh
gcc -o banco main.c
./banco
```

### Menu Principal

O menu principal oferece as seguintes opções:

1. **Cliente**: Gerenciar contas de clientes (Incluir, Mostrar, Alterar, Excluir).
2. **Saque**: Realizar saques em contas existentes.
3. **Relatórios**: Gerar relatórios de valores sacados, saldo existente e quantidade de cédulas existentes.
4. **Sair**: Finalizar o programa.

### Menu Cliente

Dentro do menu de cliente, as seguintes operações podem ser realizadas:

1. **Incluir**: Criar uma nova conta com CPF e número de conta gerados automaticamente.
2. **Mostrar**: Listar todos os clientes cadastrados.
3. **Alterar**: Modificar o CPF ou o número da conta corrente de um cliente (apenas se não houver saques).
4. **Excluir**: Remover um cliente do banco de dados (apenas se não houver saques).
5. **Voltar**: Retornar ao menu principal.

### Menu Relatórios

Dentro do menu de relatórios, você pode gerar os seguintes relatórios:

1. **Valores Sacados**: Exibir os valores sacados por todos os clientes.
2. **Valor do Saldo Existente**: Mostrar o saldo total existente no banco.
3. **Quantidade de Cédulas Existentes**: Mostrar a quantidade de cédulas de cada denominação disponível no banco.
4. **Voltar**: Retornar ao menu principal.

## Funções Principais

### Geração de CPF

```c
char* gera_cpf_valido() {
    // Função que gera um CPF válido e formatado
}
```

### Geração de Conta Corrente

```c
char* geraContaCorrente() {
    // Função que gera um número de conta corrente formatado
}
```

### Impressão de Números por Extenso

```c
void numeroParaExtenso(int num) {
    // Função que converte um número inteiro para a sua representação por extenso
}
```

### Banco de Dados

```c
void banco_de_dados(char* cpf, char* conta_corrente, int codigo, int *check, int valor_saque) {
    // Função principal responsável por gerenciar o banco de dados de clientes e realizar operações como criação de contas, saques e relatórios
}
```

## Exemplo de Uso

### Criar uma Nova Conta

1. Selecione a opção "1. Cliente" no menu principal.
2. Escolha "1. Incluir" para criar uma nova conta.
3. O CPF e o número da conta serão gerados automaticamente.
4. Confirme para continuar e a conta será criada.

### Realizar um Saque

1. Selecione a opção "2. Saque" no menu principal.
2. Informe o número da conta e o valor do saque.
3. O saque será realizado se houver saldo suficiente e a quantidade de cédulas será atualizada.

### Gerar Relatório de Valores Sacados

1. Selecione a opção "3. Relatórios" no menu principal.
2. Escolha "1. Valores Sacados" para visualizar os valores sacados por todos os clientes.

## Contribuição

Este projeto foi desenvolvido por:

- Sizenando Souza França
- Felipe Natan de Oliveira Lima
- Weliton Fernandes Alves

## Licença

Este projeto é livre para uso educacional. Sinta-se à vontade para modificar e aprimorar conforme necessário.
