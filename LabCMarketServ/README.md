Este arquivo contém as informações técnicas relacionadas ao servidor.

Todas as formas de dados são carregadas em memoria ao iniciar, sendo requerido do gestor um comando para salvar as mesmas.


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

8 - Verifica se o client tem dinheiro suficiente para a compra.


**Estruturas presentes no servidor:** Todas as estrutuas podem ser visulizadas no LibServ.h



