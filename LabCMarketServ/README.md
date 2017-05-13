Este arquivo contém as informações técnicas relacionadas ao servidor.

Alterações necessárias para fazer o servidor funcionar:

Antes de tudo, acesse o arquivo: /LabCMarket/LabCMarketServ/LabCMarketServ/LibServ.h 

**Altere o caminho dos arquivos nos defines:(Caso já não tenha feito, ou não tenha extraido a pasta principal no diretorio 'home' do usuario.**

```
#define dirStock ~/LabCMarket/LabCMarketServ/LabCMarketServ/stock.txt
#define dirUser ~/LabCMarket/LabCMarketServ/LabCMarketServ/users.txt
#define dirGestor ~/LabCMarket/LabCMarketServ/LabCMarketServ/gestores.txt
#define dirDebug ~/LabCMarket/LabCMarketServ/LabCMarketServ/debug.txt
```

**Debug**

Caso o define DEBUG for setado para 1, ele gera o arquivo para debug.txt, esse arquivo é apagado sempre que iniciar o servidor.

**Caso não queira gerar o debug, mude para 0** 

(por definição já vem setado para 0)


Todas as formas de dados são carregadas em memoria ao iniciar, os arquivos existentes, são usados para carregar as memorias.


A forma de comunicação entre o servidor, clients e gerente, são feitas através de sockets. O servidor recebe uma string e devolve uma string. Para isso, antes de cada informação passada para o servidor é passado um comando com a seguinte forma "numero:inforação".

Para cada novo client/gestor conectado ele cria um handler.

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

**Estruturas presentes no servidor:** Todas as estrutuas podem ser visulizadas no LibServ.h
