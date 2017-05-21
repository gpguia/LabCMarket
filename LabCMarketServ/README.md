Este arquivo contém as informações técnicas relacionadas ao servidor.

**Debug**

Caso o define DEBUG for setado para 1, ele cria o arquivo para debug.txt, esse arquivo é apagado sempre que iniciar o servidor.

**Caso não queira gerar o debug, mude para 0** 

(por definição já vem setado para 0)


Todas as formas de dados são carregadas em memória ao iniciar, os arquivos existentes, são usados para carregar as memórias.

A forma de comunicação entre o servidor, clientes e gerente, são feitas através de sockets. O servidor recebe uma string e devolve uma string. Para isso, antes de cada informação passada para o servidor é passado um comando com a seguinte forma "numero:inforação".

Exemplo: "1:login:senha:" 
Neste exemplo será validado o gestor com o login e senha apresentados.

**Lista de todos os comandos:** Estes comandos foram adotados de acordo com o desenvolvimento das funções.

1 - Valida Gestor

2 - Valida Cliente

3 - Gerir Saldo

4 - Depositar dinheiro

5 - Listar os todos os produtos

6 – Produto que foi adicionado no carrinho, diminui assim a sua quantidade.

7 - retorna 1 produto especifico

8 - Checkout, verifica se o cliente possui o dinheiro suficiente, e cria as estatísticas caso a compra seja efetuado com sucesso.

9 - Mostra as estatísticas para o cliente;

10 - Adicionar novo usuário.

11 - listar todos os produtos (para o gestor);

12 - incluir novo produto no stock.

13 - retorna se um produto existe.(1 para sim, 0 para não)

14 - alterar a quantidade de um produto

15 - alterar o custo de um produto

16 - alterar o preço de um produto

17 - remover um produto

18 - lista todos os usuários para o manager.

19 - verifica se um usuário existe.(retorna 1 se existir ou 0 se não existir).

20 - quando o usuário remover o produto do carrinho, retorna a quantidade para o stock.

21 - salva todas as informações nos arquivos.


**Demais informações relevantes:**

Ao cancelar o servidor ou fechar o terminal onde ele está a ser  executado, impede que seja salvo o status atual do stock, usuários e estatísticas.
O principal motivo de ter sido feito assim, foi para facilitar o desenvolvimento e testes do programa.

No arquivo LibServ.h pode-se verificar que existem três grandes blocos de funções, cada bloco está dividido e organizado para cada uma das suas funcionalidades.


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

```
struct sockHandle{
    int sock;
    Users* users;
    Produto* stock;
};
```

Essa estrutura teve que ser criada pois quando se cria um novo handler de conexão, só é possível passar um ponteiro e como era necessário passar as informações do sock, e as lista de usuários e produtos, foi criada essa estrutura.

Isso pode ser visto na função:

```
int createConn(Users* lst, Produto* stock)
```

e na função:

```
void *connection_handler(void* socket_desc)
```
ambas dentro do LibServ.c  