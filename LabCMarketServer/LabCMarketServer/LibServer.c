//
//  LibServer.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 4/26/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibServer.h"


/*
==================================
========Socket Functions==========
==================================
*/


int startServer(){
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
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
        return 1;
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
        
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    
    return 0;
}



void *connection_handler(void *socket_desc){
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



char* writeToClient(int socket, char message[]);

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
    char aux[STR_MAX_SIZE];
    
    fp = fopen(caminho,"r");
    
    if(fp == NULL){
        printf("Erro ao abrir a lista de stock.\n");
        exit(1);
    }
    
    while(fscanf(fp,"%s",aux) == 1){
        stock = addProduto(stock,aux);
    }
    
    return stock;
}

//Function used to create a new node and return it to the list.
Produto *addProduto(Produto *stock, char str[]){
    char nome[50],desc[50];
    int codigo,qtd;
    float custo,preco;
    strcpy(nome,getProductName(str));
    strcpy(desc,getProductDes(str));
    
    codigo = getProductCode(str);
    custo = getProductCost(str);
    preco = getProductPrice(str);
    qtd = getProductQtty(str);
    
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
    
    printf("got here\n");
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
        fprintf(fp,"%s:",p->nome);
        fprintf(fp,"%d:",p->codigo);
        fprintf(fp,"%s:",p->descricao);
        fprintf(fp,"%d:",p->qtd);
        fprintf(fp,"%f:",p->custo);
        fprintf(fp,"%f:",p->preco);
        fprintf(fp,"%d\n",p->qtdSold);
    }
    
    printf("Toda a alteracao foi salva no stock!\nEncerrando o servidor...\n");
    fclose(fp);
    
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

void updateStock(Produto* stock, int cod, int qtd){
    Produto* p = searchProduct(stock, cod);
    if(p == NULL){
        printf("Produto nao encontrado.\n");
        return;
    }
    p->qtdSold = p->qtdSold + qtd;
    p->qtd = p->qtd - qtd;
    
}

char *getProductName(char str[]){
    char *name;
    name = (char*) malloc(50*sizeof(char));
    int i=0;
    while(str[i] != ':'){
        name[i] = str[i];
        i++;
    }
    return name;
}

int getProductCode(char str[]){
    int code,i=0,j=0;
    char aux[10];
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        aux[j] = str[i];
        i++;
        j++;
    }
    code = atoi(aux);
    return code;
}

char *getProductDes(char str[]){
    int i=0,j=0;
    char *des;
    des = (char*) malloc(50*sizeof(char));
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        des[j] = str[i];
        i++;
        j++;
    }
    return des;
}

int getProductQtty(char str[]){
    int i=0,j=0, qtty;
    char aux[30];
    
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
    while(str[i] != ':'){
        aux[j] = str[i];
        i++;
        j++;
    }
    
    qtty = atoi(aux);
    return qtty;
}

float getProductCost(char str[]){
    int i=0,j=0;
    char aux[30];
    float cost;
    
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
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        aux[j] = str[i];
        i++;
        j++;
    }
    
    cost = atof(aux);
    return cost;
}

float getProductPrice(char str[]){
    int i=0,j=0;
    char aux[30];
    float price;
    
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
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        aux[j] = str[i];
        i++;
        j++;
    }
    
    price = atof(aux);
    return price;
}


int getProductQtdSold(char str[]){
    int i=0,j=0;
    int QtdSold;
    char aux[30];
    
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
    
    
    while(str[i] != '\0'){
        aux[j] = str[i];
        i++;
        j++;
    }

    QtdSold = atoi(aux);
    return QtdSold;
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
/*int createNewUser(){
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

 */
 
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

//Delete the entier list
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


Statistics* searchStats(Statistics* s, int cod){
    Statistics* st;
    
    for(st=s;st!=NULL;st=st->next){
        if(st->cod == cod){
            return st;
        }
    }
    return NULL;
}

