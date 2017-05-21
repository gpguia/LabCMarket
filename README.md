Grupo:
Guilherme Paiva Guia
Jorge Sousa

Este README contém as características funcionais do programa, nenhum aspecto técnico será abordado aqui. Para maiores informações verifique o README.md dentro da pasta LabCMarketServ.


**Configurações básicas**
Como executar o programa:
 
Abra o terminal e vá até onde está a pasta principal (LabCMarket-master)
 
Apenas na primeira execução é necessário executar o config.sh

```
./config.sh
```

Esse script, irá alterar o caminho dos ficheiros stock.txt,users.txt,gestores.txt,statistics.txt e debug.txt no arquivo LibServ.h para que possa ficar compatível com o local onde foi extraído os arquivos.

Em seguida, execute o start.sh

```
./start.sh
```

Esse script irá compilar e abrir 3 novas telas do terminal, executando os 3 programas, o servidor, gestor e cliente.

Siga as instruções para uso:
 
**Operações que podem ser realizadas com o Cliente:**

**_Autenticar-se:_** Antes de fazer qualquer coisa é necessário autenticar-se com o servidor.

Login e senha para o cliente:

```
login: gui
senha: gui123
ou
login: jorge
senha: braga
```
 
 
    1 - Gerir Saldo - Nesta funcionalidade o cliente pode visualizar o Saldo da conta, adicionar mais dinheiro e voltar ao menu Anterior.
        Opções:
            1 - Adicionar mais dinheiro.
            2 - Voltar ao menu anterior.
    
    2 - Gerir Lista de compras – Pode fazer qualquer operação relacionada ao carrinho de compras, adicionar ou remover um produto, também é possível finalizar a compra.
        Opções:
            1 - Adicionar produto ao carrinho.
            2 - Listar produtos do carrinho. (Verificar, remover ou finalizar a compra).
                1 - Finalizar compra.
                2 - Remover produto.
                3 - Voltar.
            3 - Voltar.
    3 - Ver Estatísticas - Mostra o histórico do cliente, mostra as ultimas compras, data e quanto gastou.
    
    4 - Salvar as alterações feitas no servidor nos ficheiros.
    
    5 - Logout.

**Operações que podem ser realizadas com o Gerente:**

**_Autenticar-se:_** Assim como o cliente, também é preciso fazer login com o gerente.

Login e senha para o gestor:

```
login: admin
senha: admin
```

		1 - Criar novo utilizador - Cria um novo cliente.
		
		2 - Gerir stock. - Opções para gerir o stock, listar, adicionar, editar e remover um produto específico.
			1 - Listar produtos.
			2 - Incluir novo produto.
			3 - Editar produto existente.
			4 - Retornar ao menu anterior.
			
		3 - Ver estatísticas. - mostra as estatísticas dos usuários, valores gastos, data da compra, hora, minutos.
			2 - um usuário especifico.
			3 - Voltar.
			
