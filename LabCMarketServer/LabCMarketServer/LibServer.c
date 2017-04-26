//
//  LibServer.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/29/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibServer.h"


/*
==================================
=========Stocks Functions=========
==================================
*/

//Create the very first element of the list, in this case is the empty list
Produto *createStock(){
    return NULL;
}

//Read from a file and keep it in the memory as a linked list.
Produto *loadStock(Produto *stock, char caminho[]){
    FILE *fp;
    int codigo,qtd,vendidos;
    float custo,preco;
    char nomeProduto[50],desc[50];
    
    fp = fopen(caminho,"r");
    
    if(fp == NULL){
        printf("Erro ao abrir a lista de stock.\n");
        exit(1);
    }
    
    while(fscanf(fp,"%s%d%s%d%f%f%d",nomeProduto,&codigo,desc,&qtd,&custo,&preco,&vendidos) == 7){
        stock = addProduto(stock, nomeProduto, codigo, desc, qtd, custo, preco);
    }
    
    return stock;
}

//Function used to create a new node and return it to the list.
Produto *addProduto(Produto *stock, char nome[], int codigo, char desc[], int qtd, float custo, float preco){
    
    Produto* novo = (Produto*) malloc(sizeof(Produto));
    strcpy(novo->nome,nome);
    novo->codigo = codigo;
    strcpy(novo->descricao,desc);
    novo->qtd = qtd;
    novo->custo = custo;
    novo->preco = preco;
    novo->next = stock;
    novo->qtdSold = 0;
    
    return novo;
}

//Function to remove a node.
Produto* removeProduto(Produto* stock, int cod){
    Produto* a = NULL;
    Produto* p = stock;
    
    while(p != NULL && p->codigo != cod){
        a = p;
        p = p->next;
    }
    
    if(p == NULL){
        return stock;
    }
    
    if(a == NULL){
        stock = p->next;
    }else{
        a->next = p->next;
    }
    
    free(p);
    return stock;
}

//Function to print in stdout all products in the stock.
void printStock(Produto* stock){
    Produto *p;
    printf("\t\t\t**Lista de produtos**\n");
    printf("%6s","Produto");
    printf("%14s","Descricao");
    printf("%10s","Codigo");
    printf("%15s","Quantidade");
    printf("%10s","Custo");
    printf("%10s","Preco");
    printf("\n----------------------------------------------------------------------");
    for(p = stock; p != NULL; p = p->next){
        printf("\n");
        printf("%6s",p->nome);
        printf("%14s",p->descricao);
        printf("%10d",p->codigo);
        printf("%15d",p->qtd);
        printf("%11.2f",p->custo);
        printf("%10.2f",p->preco);
        printf("\n----------------------------------------------------------------------");
    }
    printf("\n");
    printf("\t\t\t**Fim da lista de produtos**\n");
}

//Search for a prodcut, based in its code
Produto *searchProduct(Produto *stock, int codigo){
    
    Produto* p;
    
    for(p=stock;p!=NULL;p=p->next){
        if(p->codigo == codigo){
            return p;
        }
    }
    return NULL;
}

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


void updateStock(Produto* stock, int cod, int qtd){
    Produto* p = searchProduct(stock, cod);
    if(p == NULL){
        printf("Produto nao encontrado.\n");
        return;
    }
    p->qtdSold = p->qtdSold + qtd;
    p->qtd = p->qtd - qtd;
    
}


/*
==================================
==========Users Functions=========
==================================
*/

//Create the list
Users* createUsers(){
    return NULL;
}

//Create new user in user.txt
int createNewUser(){
    char user[50], pass[50], contato[10], nome[255], balance[10];
    char strUser[STR_MAX_SIZE];
    int flag = 0,option = -2;
    memset(strUser,0,STR_MAX_SIZE);
    FILE *fp = NULL;
    
    while(flag == 0){
        fp = fopen(dirUser, "r");
        if(fp == NULL){
            printf("Erro ao abrir o arquivo users.txt\n");
            exit(1);
        }
        memset(nome,0,255);
        memset(contato,0,10);
        memset(user,0,50);
        memset(pass,0,50);
        printf("Digite o nome do novo usuario:\n");
        scanf("%s",nome);
        printf("Digite o contato do novo usuario:\n");
        scanf("%9s",contato);
        printf("Digite o username do novo usuario:\n");
        scanf("%s",user);
        printf("Digite o password do novo usuario:\n");
        scanf("%s",pass);
        printf("Digite o saldo do novo usuario:\n");
        scanf("%9s",balance);
        flag = 1;
        while(fscanf(fp,"%s",strUser) == 1){
            if(strcmp(user,getUserName(strUser)) == 0){
                printf("Esse username ja existe.\n");
                memset(strUser,0,STR_MAX_SIZE);
                flag = 0;
                fclose(fp);
            }
        }
    }
    fclose(fp);
    
    //Confirm the information
    printf("Novo utilizador:\nNome: %s\nContato: %s\nUsuario: %s\nPassword: %s\nSaldo: %s\n",nome,contato,user,pass,balance);
    printf("\nEssas informações estão corretas? (Digite: 1 para sim, 0 para nao, -1 para cancelar)\n");
    scanf("%d",&option);
    
    if(option == 1){
        
        fp = fopen(dirUser, "a");
        
        fprintf(fp,"%s:",nome);
        fprintf(fp,"%s:",contato);
        fprintf(fp,"%s:",user);
        fprintf(fp,"%s:",pass);
        fprintf(fp,"%s\n",balance);
        
        fclose(fp);
        
        printf("Usuario criado com sucesso!(\n");
        return 1;
    }else if(option == -1){
        flag = 1;
        return -1;
    }else if(option == 0){
        createNewUser();
    }
    return -1;
}

//get the name of the user in the file
char *getName(char str[]){
    int i=0;
    char tmp[STR_MAX_SIZE];
    memset(tmp,0,STR_MAX_SIZE);
    
    while(str[i] != ':'){
        tmp[i] = str[i];
        i++;
    }
    memset(str,0,STR_MAX_SIZE);
    strcpy(str,tmp);
    return str;
}

//get contact from users file
char *getContact(char str[]){
    int i = 0, j = 0;
    char tmp[STR_MAX_SIZE];
    memset(tmp,0,STR_MAX_SIZE);
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        tmp[j] = str[i];
        i++;
        j++;
    }
    memset(str,0,STR_MAX_SIZE);
    strcpy(str,tmp);
    return str;
}

//recive an string the way its saved in the file, and return the username
char *getUserName(char str[]){
    int i =0, j=0;
    char username[50];
    memset(username,0,50);
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != ':'){
        username[j] = str[i];
        i++;
        j++;
    }
    memset(str,0,STR_MAX_SIZE);
    strcpy(str,username);
    return str;
}

//recive an string the way its saved in the file, and return the username
char *getPassword(char str[]){
    int i =0, j=0;
    char pass[50];
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != ':'){
        i++;
    }
    i++;
    j=0;
    while(str[i] != ':'){
        pass[j] = str[i];
        i++;
        j++;
    }
    
    memset(str,0,STR_MAX_SIZE);
    strcpy(str,pass);
    return str;
}

char* getBalance(char str[]){
    char balance[10];
    int i=0,j=0;
    
    memset(balance,0,10);
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != ':'){
        i++;
    }
    i++;
    j=0;
    while(str[i] != ':'){
        i++;
    }
    i++;
    while(str[i] != '\0'){
        balance[j] = str[i];
        i++;
        j++;
    }
    memset(str,0,STR_MAX_SIZE);
    strcpy(str,balance);
    return str;
}


//Add new user to the list
Users* addUser(Users* lst, char nome[], char contato[], char user[], char pass[], char* balance){
    Users* novo = (Users*) malloc(sizeof(Users));
    
    strcpy(novo->nome,nome);
    strcpy(novo->contato,contato);
    strcpy(novo->username,user);
    strcpy(novo->password,pass);
    strcpy(novo->balance,balance);
    novo->statUsu = NULL;
    novo->next = lst;
    
    return novo;
}

Users* deleteListUsu(Users* usu){
    Users* uCurrent = usu;
    Users* uNext;
    
    while(uCurrent != NULL){
        uNext = uCurrent->next;
        free(uCurrent);
        uCurrent = uNext;
    }
    
    return NULL;
}


//Load all users from the users.txt to a linked list
Users* loadUsers(Users* lst){
    FILE *fp;
    char str[STR_MAX_SIZE],strTmp[STR_MAX_SIZE];
    char nome[255], contato[10], username[50], password[50],balance[10];
    memset(str,0,STR_MAX_SIZE);
    memset(strTmp,0,STR_MAX_SIZE);
    fp = fopen(dirUser,"r");
    if(fp == NULL){
        printf("Erro ao carregar os usuarios em memoria.\n");
        exit(1);
    }
    
    while(fscanf(fp,"%s",str) == 1){
        //printf("%s\n",str);
        strcpy(strTmp,str);
        strcpy(nome,getName(strTmp));
        memset(strTmp,0,STR_MAX_SIZE);
        strcpy(strTmp,str);
        strcpy(contato,getContact(strTmp));
        memset(strTmp,0,STR_MAX_SIZE);
        strcpy(strTmp,str);
        strcpy(username,getUserName(strTmp));
        memset(strTmp,0,STR_MAX_SIZE);
        strcpy(strTmp,str);
        strcpy(password,getPassword(strTmp));
        memset(strTmp,0,STR_MAX_SIZE);
        strcpy(strTmp,str);
        strcpy(balance,getBalance(strTmp));
        lst = addUser(lst, nome, contato, username, password, balance);
    }
    fclose(fp);
    return lst;
}

void printUsers(Users* usu){
    Users* u;
    printf("** Lista de usuarios **\n");
    printf("%12s","Nome");
    printf("%15s","Contato");
    printf("%13s","Username\n");
    printf("---------------------------------------------------------\n");
    for(u = usu;u!=NULL;u=u->next){
        printf("%12s",u->nome);
        printf("%15s",u->contato);
        printf("%12s",u->username);
        printf("\n---------------------------------------------------------\n");
    }
    printf("** Fim da lista de usuarios **\n");
}

//Search for a specific user, using username
Users* searchUser(Users* lst, char username[]){
    Users* p;
    
    for(p=lst;p!=NULL;p=p->next){
        if(strcmp(p->username,username) == 0){
            return p;
        }
    }
    return NULL;
}

//Update the stats struct from the users
Statistics* updateStatistics(Statistics* s, int cod, int qtdCom){
    
    Statistics* st;
    
    st = searchStats(s, cod);
    
    //In this case, the user have never bought that product, so create a new statistics for that product
    if(st == NULL){
        Statistics* nova = (Statistics*) malloc(sizeof(Statistics));
        nova->cod = cod;
        nova->qtdComprada = qtdCom;
        nova->next = s;
        return nova;
    }else{
        //If the stats already exsists, just add the new quantity;
        st->qtdComprada = st->qtdComprada + qtdCom;
    }
    
    //return the old stats
    return s;
}


//Intermediary function to update statistics struct
Users* updateBuyer(Users* usu, int cod, int qtdCom, char username[]){
    Users* u;
    u = searchUser(usu,username);
    if(u == NULL){
        printf("Usuario nao existe.\n");
        return usu;
    }
    
    u->statUsu = updateStatistics(u->statUsu, cod, qtdCom);
    
    return usu;
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

//Save all changes in the stock file.
void writeChanges(Produto* stock){
    FILE *fp;
    
    fp = fopen(dirStock,"w+");
    if(fp == NULL){
        printf("Erro ao gravar as mudancas no ficheiro stock, verifique o log\n.");
        exit(1);
    }
    
    Produto *p;
    
    for(p = stock ;p != NULL ;p = p->next){
        fprintf(fp,"%s\n",p->nome);
        fprintf(fp,"%d\n",p->codigo);
        fprintf(fp,"%s\n",p->descricao);
        fprintf(fp,"%d\n",p->qtd);
        fprintf(fp,"%f\n",p->custo);
        fprintf(fp,"%f\n",p->preco);
        fprintf(fp,"%d\n",p->qtdSold);
    }
    
    printf("Toda a alteracao foi salva no stock!\nEncerrando o servidor...\n");
    fclose(fp);
    
}


/*
==================================
========Socket Functions==========
==================================
*/


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];
    memset(client_message,0,2000);
    //Receive a message from client
    while( (read_size = (int)recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
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
    
    //Free the socket pointer
    free(socket_desc);
    
    return 0;
}













