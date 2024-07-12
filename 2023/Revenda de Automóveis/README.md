# Revenda de Automóveis

O código implementa um sistema de gerenciamento de estoque de carros. As funcionalidades incluem:

**Cadastro de carros:**

- Gera placa, modelo, fabricante, ano de fabricação, ano do modelo, tipo de combustível, cor e opcionais aleatoriamente.
- Permite ao usuário visualizar as informações do carro gerado e confirmar o cadastro.
- Salva as informações do carro em um arquivo binário ("carros.bin").

**Consulta de carros:**

- Lista carros ordenados por fabricante e modelo.
- Permite filtrar carros por opcionais selecionados pelo usuário.
- Permite filtrar carros por faixa de ano de fabricação.
- Exibe detalhes de cada carro encontrado, incluindo placa, modelo, fabricante, ano de fabricação, ano do modelo, tipo de combustível, cor, opcionais e preço de compra.

**Remoção de carros:**

- Solicita ao usuário a placa do carro a ser removido.
- Verifica se o carro existe no cadastro.
- Se o carro existir e não tiver sido vendido, remove o carro do arquivo binário ("carros.bin").

**Outras funcionalidades:**

- Utiliza funções do `stdlib.h`, `time.h`, `string.h`, `ctype.h`, `windows.h` e `unistd.h`.
- Define constantes para o tamanho máximo de strings e para o nome dos arquivos binários.
- Utiliza estruturas para armazenar dados de carros, clientes, endereços, telefones, datas e vendas.
- Define funções para gerar números aleatórios dentro de um intervalo, gerar placas de carros, gerar informações de carros, inserir carros no cadastro, verificar a existência de um carro, remover um carro, comparar carros por fabricante, listar carros por fabricante, listar carros por opcionais, listar carros por faixa de ano de fabricação e verificar se um carro já foi vendido.
- Implementa um menu principal para o usuário navegar pelas funcionalidades do sistema.
