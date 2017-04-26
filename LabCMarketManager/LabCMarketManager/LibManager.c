//
//  LibServer.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/29/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"


//This function show options to edit or add an item form the stock(Include remove).
Produto* manageStock(Produto *stock){
    int flag = 0, option;
    printf("**Menu Gerir Stock**\n");
    printf("1) Listar os produtos.\n");
    printf("2) Incluir novo produto.\n");
    printf("3) Editar produto existente.\n");
    printf("4) Retornar ao menu principal.\n");
    scanf("%d",&option);
    while(flag == 0){
        if(option == 1){
            flag = 1;
            system("clear");
            printStock(stock);
        }else if(option == 2){
            flag = 1;
            stock = showIncludeOptions(stock);
        }else if(option == 3){
            flag = 1;
            stock = editProduct(stock);
        }else if(option == 4){
            flag = 1;
            system("clear");
            return stock;
        }else{
            printf("Verifique o numero digitado.\n");
            scanf("%d",&option);
        }
    }
    stock = manageStock(stock);
    return stock;
}

//Edit a product that is already in the stock.
Produto* editProduct(Produto* stock){
    Produto *p = NULL;
    int cod = 0, flag = 1,option=0, changed;
    float changedf;
    system("clear");
    printStock(stock);
    printf("\n");
    while(flag == 1){
        printf("Digite o codigo do produto que deseja editar.\n");
        scanf("%d",&cod);
        p = searchProduct(stock, cod);
        if(p == NULL){
            printf("Codigo invalido, digite 1 para tentar novamente ou digite 0 para cancelar.\n");
            scanf("%d",&flag);
        }else{
            flag = 2;
        }
    }
    
    if(flag == 0){
        system("clear");
        return stock;
    }
    
    printf("1) Alterar quantidade\n");
    printf("2) Alterar custo\n");
    printf("3) Alterar preco\n");
    printf("4) Remover produto\n");
    scanf("%d",&option);
    while(option != -3){
        if(option == 1){
            printf("Digite a nova quantidade: \n");
            scanf("%d",&changed);
            p->qtd = changed;
            option = -3;
        }else if(option == 2){
            printf("Digite o novo custo: \n");
            scanf("%f",&changedf);
            p->custo = changedf;
            option = -3;
        }else if(option == 3){
            printf("Digite o novo preco: \n");
            scanf("%f",&changedf);
            p->preco = changedf;
            option = -3;
        }else if(option == 4){
            stock = removeProduto(stock, cod);
            option = -3;
        }else{
            printf("Verifique a alterantiva selecionada.\n");
        }
    }
    
    return stock;
}

/*
 ==================================
 =======Statistics Functions=======
 ==================================
 */

void showStatistics(Produto* stock,Users* usu){
    int option = 0;
    
    while(option != 3){
        printf("1) Ver estatisticas do stock.\n");
        printf("2) Ver estatisticas dos usuarios.\n");
        printf("3) Voltar ao menu principal.\n");
        scanf("%d",&option);
        if(option == 3){
            return;
        }else if(option == 1){
            showStasStock(stock);
        }else if(option == 2){
            system("clear");
            showStatsUsuMenu(usu,stock);
        }else{
            system("clear");
            printf("***Esta opcao nao existe.***\n\n");
        }
    }
    
}


void showStasStock(Produto* stock){
    Produto* p;
    int totalVendido = 0;
    
    system("clear");
    printf("\t\t\t** Estatisticas dos produtos **\n");
    printf("%6s","Produto");
    printf("%8s","Codigo");
    printf("%15s","Qtd Vendidos");
    printf("%15s","% Lucro");
    printf("%25s","Restantes no stock");
    printf("\n----------------------------------------------------------------------");
    for(p=stock;p!=NULL;p=p->next){
        totalVendido = totalVendido + p->qtdSold;
        printf("\n");
        printf("%6s",p->nome);
        printf("%8d",p->codigo);
        printf("%10d",p->qtdSold);
        printf("%20.2f%c",calcPercentProfit(p),37);
        printf("%15d",p->qtd);
        printf("\n----------------------------------------------------------------------");
        
    }
    
    printf("\n** Total de produtos vendidos: %d\n",totalVendido);
    
    printf("\n\t\t\t** Fim das Estatisticas **\n");
    
    
    
}

float calcPercentProfit(Produto* p){
    return (p->preco - p->custo)/p->preco * 100;
}

float calcPercentRemaining(Produto* p){
    return (p->qtd - p->qtdSold)/p->qtd * 100;
}


void showStatsUsu(Users* usu,Produto* stock, char user[]){
    Statistics* s;
    Users* u;
    Produto* p;
    u = searchUser(usu, user);
    float soma=0;
    
    
    printf("%s","Usuario: ");
    printf("%s\n",u->nome);
    printf("%s","Username: ");
    printf("%s\n",u->username);
    printf("\n");
    printf("%10s","Produto");
    printf("%15s","Qtd Comprada");
    printf("%18s\n","Valor gasto");
    printf("----------------------------------------------------------------------\n");
    
    if(u->statUsu == NULL){
        printf("Usuario nunca fez qualquer compra.\n");
        return;
    }
    
    for(s=u->statUsu;s!=NULL;s = s->next){
        p = searchProduct(stock, s->cod);
        printf("%10s",p->nome);
        printf("%14d",s->qtdComprada);
        printf("%18.2f",p->preco*s->qtdComprada);
        printf("\n----------------------------------------------------------------------\n");
        soma = soma + p->preco*s->qtdComprada;
    }
    
    printf("\nValor total gasto pelo usuario: %.2f\n\n",soma);
}


void showStatsUsuMenu(Users* usu, Produto* stock){
    int options = 0;
    char usuName[50];
    Users* u;
    
    while(options != 3){
        printf("1) Um usuario especifico.\n");
        printf("2) Todos os usuarios.\n");
        printf("3) Retornar ao menu anterior.\n");
        scanf("%d",&options);
        
        if(options == 3){
            system("clear");
            return;
        }else if(options == 1){
            system("clear");
            printUsers(usu);
            printf("Digite o username do usuario que deseja ver: \n");
            scanf("%s",usuName);
            system("clear");
            while(searchUser(usu, usuName) == NULL){
                printf("Username nao existente, digite novamente ou digite 0 para cancelar.\n");
                scanf("%s",usuName);
                if(strcmp(usuName,"0") == 0){
                    return;
                }
            }
            showStatsUsu(usu,stock,usuName);
        }else if(options == 2){
            system("clear");
            for(u = usu; u != NULL; u = u->next){
                printf("\n");
                showStatsUsu(usu,stock,u->username);
                printf("\n");
            }
        }else{
            system("clear");
            printf("** Esta opcao nao existe. **\n");
        }
    }
}

Statistics* searchStats(Statistics* s, int cod){
    Statistics* st;
    
    for(st=s;st!=NULL;st=st->next){
        if(st->cod == cod){
            return st;
        }
    }
    return NULL;
}


/*
 ==================================
 ====Valid/ShowOptions Functions===
 ==================================
 */

//Funções para gerir os logins dos clients
char *validLogin(Users* lst, char login[]){
    Users* p;
    char username[50], password[50];
    char strTmp[STR_MAX_SIZE];
    memset(username,0,50);
    memset(password,0,50);
    memset(strTmp,0,STR_MAX_SIZE);
    
    //Nesse caso estou usando as funcoes ja criadas para pegar o nome e o contato, para separar o login e o password
    //que estao sendo enviados atraves do client, pois usam o mesmo principio e estao separados por ':';
    
    strcpy(strTmp,login);
    strcpy(username,getName(strTmp));
    memset(strTmp,0,STR_MAX_SIZE);
    strcpy(strTmp,login);
    strcpy(password,getContact(strTmp));
    
    p = searchUser(lst, username);
    
    if(p != NULL){
        //Neste caso eu so preciso verificar o password, pois ele so entra no if caso encontre o username.
        if(strcmp(p->password,password) == 0){
            return "1";
        }
    }
    
    return "0";
}

//Função para gerir login dos gestores
bool validGestor(char user[], char pass[]){
    char strUser[STR_MAX_SIZE],strPass[STR_MAX_SIZE];
    char fuser[50], fpass[50];
    memset(fuser,0,50);
    memset(fpass,0,50);
    memset(strUser,0,STR_MAX_SIZE);
    memset(strPass,0,STR_MAX_SIZE);
    FILE *fp;
    fp = fopen(dirGestor,"r");
    
    if(fp == NULL){
        printf("Erro ao abrir o arquivo de gestores\n");
        exit(1);
    }
    while(fscanf(fp,"%s",strUser) == 1){
        strcpy(strPass,strUser);
        if(strcmp(user,getUserName(strUser)) == 0){
            if(strcmp(pass,getPassword(strPass)) == 0){
                return true;
            }
        }
    }
    return false;
}

//Função para criar os sockets
void socketCreate(){
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
        exit(1);
    }
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        exit(1);
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
    }
    puts("Connection accepted");
    
    
    
    
    
    //Receive a message from client
    while( (read_size = (int)recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        printf("Messagem 1: %s\n",client_message);
        if(strcmp(client_message,"1") == 0){
            //float saldo = gerirSaldo(client_message);
        }else if(strcmp(client_message,"2") == 0){
            
        }else if(strcmp(client_message,"3") == 0){
            
        }else if(strcmp(client_message,"4") == 0){
            write(client_sock , "Logout success!" , sizeof("Logout success!"));
            close(client_sock);
        }else{
            //write(client_sock,validLogin(client_message),sizeof(client_message));
        }
        //Send the message back to client
        //write(client_sock , client_message , strlen(client_message));
        memset(client_message,0,2000);
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
}

//Show all options
Produto *showOptions(Produto *stock, Users* usu){
    int option;
    printf("**Menu Principal**\n");
    printf("1) Criar novo utilizador\n");
    printf("2) Gerir Stocks\n");
    printf("3) Ver Estatisticas\n");
    printf("4) Logout\n");
    scanf("%d",&option);
    while(option != 4){
        if(option == 1){
            system("clear");
            option = -2;
            usu = deleteListUsu(usu);
            createNewUser();
            usu = loadUsers(usu);
        }else if(option == 2){
            system("clear");
            option = -2;
            stock = manageStock(stock);
        }else if(option == 3){
            system("clear");
            option = -2;
            showStatistics(stock,usu);
        }else if(option == 4){
            return stock;
        }else if(option == -2){
            system("clear");
            printf("**Menu**\n");
            printf("1) Criar novo utilizador\n");
            printf("2) Gerir Stocks\n");
            printf("3) Ver Estatisticas\n");
            printf("4) Logout\n");
            scanf("%d",&option);
        }else{
            printf("Verifique o numero digitado.\n");
            scanf("%d",&option);
        }
    }
    return stock;
}

//List options to insert a product
Produto* showIncludeOptions(Produto* stock){
    int option, flag =0;
    char str[STR_MAX_SIZE];
    char nome[50],des[50];
    int cod,qtd;
    float custo,preco;
    memset(str,0,STR_MAX_SIZE);
    system("clear");
    printf("**Inserir Produto Menu**\n");
    printf("1) Adicionar atraves de um ficheiro.\n");
    printf("2) Adicionar manualmente.\n");
    printf("3) Voltar ao menu anterior.\n");
    scanf("%d",&option);
    while(flag == 0){
        if(option == 1){
            flag = 1;
            printf("Porfavor, digite o caminho completo para o ficheiro(eg: ~/Desktop/lista.txt, ou digite 0 para cancelar).\n");
            scanf("%s",str);
            if(strcmp(str,"0") == 0){
                printf("Operacao cancelada.\n");
                return stock;
            }else{
                stock = loadStock(stock, str);
            }
        }else if(option == 2){
            int flag2 = 1;
            flag = 1;
            while(flag2 == 1){
                printf("Nome do produto: \n");
                scanf("%s",nome);
                printf("Codigo do produto: \n");
                scanf("%d",&cod);
                printf("Descricao do produto: \n");
                scanf("%s",des);
                printf("Quantidade do produto: \n");
                scanf("%d",&qtd);
                printf("Custo do produto: \n");
                scanf("%f",&custo);
                printf("Preco do produto: \n");
                scanf("%f",&preco);
                if(searchProduct(stock, cod) == NULL){
                    flag2 = 0;
                    stock = addProduto(stock, nome, cod, des, qtd, custo,preco);
                }else{
                    printf("Esse codigo ja existe.\n");
                    printf("Inserir outro produto? (1 para sim, 0 para nao.)\n");
                    scanf("%d",&flag2);
                }
            }
        }else if(option == 3){
            flag = 1;
            system("clear");
            return stock;
        }else if(option == -2){
            flag = 1;
        }else{
            printf("Verifique a opcao digitada.\n");
        }
    }
    return stock;
}


/*
 ==================================
 ========Socket Functions==========
 ==================================
 */













