Este arquivo contém as informações técnicas relacionadas ao servidor.

**Debug**

Caso o define DEBUG for setado para 1, ele gera o arquivo para debug.txt, esse arquivo é apagado sempre que iniciar o servidor.

**Caso não queira gerar o debug, mude para 0** 

(por definição já vem setado para 0)


Todas as formas de dados são carregadas em memoria ao iniciar, os arquivos existentes, são usados para carregar as memorias.

A forma de comunicação entre o servidor, clients e gerente, são feitas através de sockets. O servidor recebe uma string e devolve uma string. Para isso, antes de cada informação passada para o servidor é passado um comando com a seguinte forma "numero:inforação".

Exemplo: "1:login:senha:" 
Neste exemplo será validado o gestor com o login e senha apresentatos.

**Lista de todos os comandos:** Estes comandos forma adotados de acordo com o desenvolvimento das funções.

1 - Valida Gestor

2 - Valida Client

3 - Gerir Saldo

4 - Depositar dinheiro

5 - listar os todos os produtos

6 - Produto foi add no carrinho, logo tem que diminuir q quantidade.

7 - retorna 1 produto especifico

8 - Checkout, verifica se o client possui o dinheiro suficiente, e cria as statisticas caso a compra seja efetuado com sucesso.

9 - Mostra as statisticas para o client;

10 - Adicionar novo usuario.

11 - listar todos os produtos (para o gestor);

12 - incluir novo produto no stock.

13 - retorna se um produto existe.(1 para sim, 0 para nao)

14 - alterar a quantidade de um produto

15 - alterar o custo de um produto

16 - alterar o preco de um produto

17 - remover um produto

18 - lista todos os usuarios para o manager.

19 - verifica se um usuario existe.(retorna 1 se existir ou 0 se nao exisitr).

20 - quando o usuario remover o produto do carrinho, voltar ele para o stock.

21 - salva todas as informações nos arquivos.


**Demais informações relevantes:**

Ao cancelar o servidor, ou fechar o terminal onde ele está sendo executado, impede de que seja salvo o status atual do stock, usuarios e estatisticas.
O motivo principal de ter sido feito assim, foi para facilitar o desenvolvimento e testes do programa.

No arquivo LibServ.h pode-se verificar que existem três grandes blocos de funções, cada bloco está dividido e organizado para suas funcionalidades.


**Estruturas presentes no servidor:** 

Estrutura das estatísticas:

```
typedef struct statistics Statistics;

struct statistics{
    int qtdComprada;
    int cod;
    struct tm tm;
    float valorGasto;
    char nome[50];
    Statistics* next;
};
```

Estrutura do stock, para guardar os produtos:

```
typedef struct produto Produto;

struct produto{
    char nome[50];
    int codigo;
    char descricao[50];
    int qtd;
    float custo;
    float preco;
    int qtdSold;
    Produto* next;
};
```

Estrutura dos usuários:

Esta estrutura é a única que possui uma lista duplamente ligada, pois cada usuário possui sua própria estatística


```
typedef struct users Users;

struct users{
    char nome[255];
    char contato[10];
    char username[50];
    char password[50];
    char balance[20];
    Statistics* statUsu;
    Users* next;
};
```

Estrutura do handler:

Estra estrutura teve que ser criada pois quando se cria um novo handler de conexão, só é possível passar um ponteiro,
e como era necessário passar as informações do sock, e as listas de usuarios e produtos, foi criado essa estrutura para isso.

Isso pode ser visto na função:

```
int createConn(Users* lst, Produto* stock)
```

e na função:

```
void *connection_handler(void* socket_desc)
```
ambas dentro do LibServ.c

```
struct sockHandle{
    int sock;
    Users* users;
    Produto* stock;
};
```


