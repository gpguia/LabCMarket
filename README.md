Grupo:
Guilherme Paiva Guia
Jorge Sousa

Este README, contem as caracteristicas funcionais do programa, nenhum aspecto técnico será abordado aqui,
para maiores informações, verifique o README.md dentro das respectivas pastas, seja do servidor, manager ou client.


**Configurações básicas**
Foi assumido que o programa foi extraido na pasta 'home' do usuario. (eg: ~/LabCMarket)

	Em caso de não estar usando na pasta home, siga as seguintes instruções: 
	
	1 - Altere os caminhos dos defines no ficheiro: `/LabCMarket/LabCMarketServ/LabCMarketServ/LibServ.h` (stock,users,gestores,debug) 
	
	2 - Altere o caminho do script (startServ.sh) no ficheiro: `/LabCMarket/LabCMarketManager/LabCMarketManager/LibManager.h`
	
	3 - Compile o programa uasndo o seguinte comando:
	
	```
	gcc <PATH TO FOLDER>/LabCMarketClient/LabCMarketClient/main.c <PATH TO FOLDER>/LabCMarketClient/LabCMarketClient/LibClientFunctions.c -o <PATH TO FOLDER>/LabCMarketClient/LabCMarketClient/client && chmod +x <PATH TO FOLDER>/LabCMarketClient/LabCMarketClient/client
	gcc <PATH TO FOLDER>/LabCMarketManager/LabCMarketManager/main.c <PATH TO FOLDER>/LabCMarketManager/LabCMarketManager/LibManager.c -o <PATH TO FOLDER>/LabCMarketManager/LabCMarketManager/manager && chmod +x <PATH TO FOLDER>/LabCMarketManager/LabCMarketManager/manager
	```
	
	o startServer.sh, é executado sempre que o manager abre, ou seja, ao executar o manager, ele executa o script, para que seja compilado e executado
	o servidor.
	
	em seguida, execute os programas:
	
	./<PATH TO FOLDER>/manager
	./<PATH TO FOLDER>/client
	
**Operações que podem ser realizadas com o Client:**

**_Autenticar-se:_** Antes de fazer qualquer coisa é necessário autenticar-se com o servidor.
    
    1 - Gerir Saldo - Nesta funcionalidade o client pode visualizar o Saldo da conta, adicionar mais fundo e voltar ao menu Anterior.
        Opções:
            1 - Adicionar mais dinheiro.
            2 - Voltar ao menu anterior.
    
    2 - Gerir Lista de compras - Fazer qualquer operação relacionada ao carrinho de compras, seja adicionar ou remover um produto, também é possível finalizar a compra.
        Opções:
            1 - Adicionar produto ao carrinho.
            2 - Listar produtos do carrinho. (Verificar, remover ou finalizar a compra).
                1 - Finalizar compra.
                2 - Remover produto.
                3 - Voltar.
            3 - Voltar.
    3 - Ver Estatisticas - Mostra o histórico do client, mostra as ultimas compras, data e quanto gastou.
    
    4 - Logout.

**Operações que podem ser realizadas com o Gerente:**

**_Autenticar-se:_** Assim como o client, também é preciso fazer login com o gerente.

		1 - Crira novo utilizado - Cria um novo client.
		
		2 - Gerir stock. - Opções para gerir o stock, listar, adicionar, editar e remover um produto especifico.
			1 - Listar produtos.
			2 - Incluir novo produto.
			3 - Editar produto existente.
			4 - Retornar ao menu anterior.
			
		3 - Ver estatisticas. - mostra as estatisticas dos usuarios, valores gastos, data da compra, hora, minutos.
			2 - um usuario especifico.
			3 - Voltar.
			
			
