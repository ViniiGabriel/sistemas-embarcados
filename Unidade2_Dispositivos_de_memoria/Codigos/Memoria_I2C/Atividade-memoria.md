# Atividade usando memória i2c

## Objetivo:

Nesta atividade faremos experimentos com  armazenamento e leitura de dados da memória i2c (CI 24c512 ou CI 24c256) no laboratório. Nossa aplicação alvo é um sistema embarcado com IDs e senhas para abrir uma porta. Cada pessoa tem tem um ID e senha e deve entrar com esses dados para abrir a porta. O controlador embarcado da fechadura tem um banco de dados em memória i2c com esses dados. Você esta desenvolvendo a aplicação que irá inserir/remover e listar os dados nesse banco de dados. Você conecta o seu PC e realiza as operações sobre o dispositivo eletrônico.

## Desenvolvimento:

Na aula anterior vimos como criar um rotina **leiaByte()** e **escreveByte()**. Em posse dessas é possível criar outras rotinas de mais alto nível para ler e escrever conjuntos de dados. 

Neste momento o objetivo é escrever uma aplicação que crie um banco de dados e consiga inserir, remover e listar as credenciais no banco de dados. A aplicação precisa ter um menu com as opções tal mostrado abaixo:

```
[1] Lista todas as IDs e senhas
[2] Adiciona uma nova entrada (ID e Senha)
[3] Mostra qtd de pessoas cadastradas
[4] Remove uma entrada de ID/senha
[5] Inicializa BD
```

Para tornar o mais simples possível, vamos utilizar um diagrama de memória como visto em aula (quando usamos o sistema de arquivos disponível no ESP32). No inicio existe um cabeçalho que indica quantos são os registros cadastrados. Na sequencia existem as entradas (ID e senha) para cada uma das pessoas.

1) Inicialmente o  banco de dados precisa ser inicializado (**opção 5** do menu). Aqui é preciso armazenar no  cabeçalho o conteúdo zero ou seja, 0 registros cadastrados). 
2) Quando uma operação de adição de uma entrada for realizada (**opção 2**)  é preciso obter o valor do cabeçalho, localizar a posição de memória para escrever e ali inserir o ID e senha, depois, atualiza o valor do cabeçalho.
3) Para remover (**opção 4**) fazemos um apagamento lógico, se o BD possui 10 registros e desejamos apagar o 2, basta localizar última entrada, ler os dados e sobrescrever na posição 2, depois, decrementa o cabeçalho.
4) Mostrar a quantidade de entradas (**opção 3**) é apenas ler o mostrar o valor do cabeçalho
5) Listas todas as entradas é apenas ler o cabeçalho, acessar cada uma das entradas e mostrar no terminal.

```
+----------------+
|    CABECALHO   |
+----------------+
| ID / senha     |
+----------------+
|     .....      |
+----------------+
|     .....      |
+----------------+
| ID / senha     |
+----------------+ 
```
