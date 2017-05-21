//
//  LibServ.c
//  LabCMarketServ
//
//  Created by Guilherme Guia and Jorge Sousa on 4/26/17.
//  Copyright © 2017 Guilherme Guia and Jorge Sousa. All rights reserved.
//

#include "LibServ.h"

char* itoa(int val, int base){
    
    static char buf[32] = {0};
    
    int i = 30;
    
    for(; val && i ; --i, val /= base)
        
        buf[i] = "0123456789abcdef"[val % base];
    
    return &buf[i+1];
    
}

int createConn(Users* lst, Produto* stock){
    int socket_desc , client_sock , c ;//new_sock;
    struct sockaddr_in server , client;
    struct sockHandle sh;
    sh.stock = stock;
    sh.users = lst;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Erro ao criar a socket.\n (funcao createConn)");
    }
    puts("Socket criada.\n");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET; //TCP
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Erro ao dar o bind no endereco. \n (funcao createConn");
        return 1;
    }
    puts("bind feito com sucesso!\n");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Esperando por conexoes...");
    c = sizeof(struct sockaddr_in);
    
    
    //Accept and incoming connection
    puts("Esperando por novas conexoes...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Conexao aceita.");
        
        pthread_t sniffer_thread;
        //new_sock = malloc(1);
        //*new_sock = client_sock;
        sh.sock = client_sock;
        
        
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler ,&sh) < 0)
        {
            perror("Erro ao criar o novo thread.");
            return 1;
        }
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler criado com sucesso!");
    }
    
    if (client_sock < 0)
    {
        perror("Erro na aceitacao.");
        return 1;
    }
    
    return 0;
}

//Get the first number from the client/manager string, where its allways a number to do something(look at readme.md inside LabCMarketServ folder)
int getCommand(char msg[]){
    int command = 0;
    int i=0,j=0;
    char aux[STR_MAX_SIZE], strTmp[STR_MAX_SIZE];
    memset(aux,0,STR_MAX_SIZE);
    memset(strTmp,0,STR_MAX_SIZE);
    
    while(msg[i] != ':'){
        aux[i] = msg[i];
        i++;
    }
    i++;
    while(msg[i] != '\0'){
        strTmp[j] = msg[i];
        i++;
        j++;
    }
    command = atoi(aux);
    memset(msg,0,STR_MAX_SIZE);
    strcpy(msg,strTmp);
    if(DEBUG == 1){
        FILE* fp;
        fp = fopen(dirDebug,"a");
        fprintf(fp,"getCommand Debug\n");
        fprintf(fp,"msg sem o comando: %s\n",msg);
        fprintf(fp,"Command: %d\n",command);
        fprintf(fp,"Fim getCommand Debug\n");
        fprintf(fp,"\n");
        fclose(fp);
    }
    return command;
}

//Create the list
Users* createUsers(){
    return NULL;
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

//get the name of the user in the file
void getName(char str[], char name[]){
    int i=0;
    memset(name,0,255);
    while(str[i] != ':'){
        name[i] = str[i];
        i++;
    }
}

//get contact from users file
void getContact(char str[], char contato[]){
    int i = 0, j = 0;
    memset(contato,0,10);
    
    while(str[i] != ':'){
        i++;
    }
    i++;
    
    while(str[i] != ':'){
        contato[j] = str[i];
        i++;
        j++;
    }
}

//recive an string the way its saved in the file, and return the username
void getUserName(char str[], char username[]){
    int i =0, j=0;
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
}

//recive an string the way its saved in the file, and return the username
void getPassword(char str[],char pass[]){
    int i =0, j=0;
    memset(pass,0,50);
    
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
}

void getBalance(char str[], char balance[]){
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
}

//Load all users from the users.txt to a linked list
Users* loadUsers(Users* lst){
    FILE *fp;
    char str[STR_MAX_SIZE],strTmp[STR_MAX_SIZE];
    char nome[255], contato[10], username[50], password[50],balance[10];
    memset(str,0,STR_MAX_SIZE);
    memset(strTmp,0,STR_MAX_SIZE);
    memset(nome,0,255);
    memset(contato,0,10);
    memset(username,0,50);
    memset(password,0,50);
    memset(balance,0,10);
    fp = fopen(dirUser,"r");
    if(fp == NULL){
        printf("Erro ao carregar os usuarios em memoria.\n");
        exit(1);
    }
    
    while(fscanf(fp,"%s",str) == 1){
        getName(str,nome);
        getContact(str,contato);
        getUserName(str,username);
        getPassword(str, password);
        getBalance(str,balance);
        lst = addUser(lst, nome, contato, username, password, balance);
        memset(str,0,STR_MAX_SIZE);
    }
    fclose(fp);
    return lst;
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

void printUsers(Users* usu){
    Users* u;
    printf("** Lista de usuarios **\n");
    printf("%12s","Nome");
    printf("%15s","Contato");
    printf("%13s","Username\n");
    printf("%13s","Password\n");
    printf("---------------------------------------------------------\n");
    for(u = usu;u!=NULL;u=u->next){
        printf("%12s",u->nome);
        printf("%15s",u->contato);
        printf("%12s",u->username);
        printf("%12s",u->password);
        printf("\n---------------------------------------------------------\n");
    }
    printf("** Fim da lista de usuarios **\n");
}

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
    fclose(fp);
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

//Valid client
int validClient(Users* lst, char str[]){
    int i=0,j=0;
    char user[STR_MAX_SIZE],pass[50];
    Users* u;
    
    memset(user,0,STR_MAX_SIZE);
    memset(pass,0,50);
    
    while(str[i] != ':'){
        user[i] = str[i];
        i++;
    }
    i++;
    while(str[i] != ':'){
        pass[j] = str[i];
        i++;
        j++;
    }
    u = searchUser(lst,user);
    if(DEBUG == 1){
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        FILE*fp;
        fp=fopen(dirDebug,"a");
        if(fp == NULL){
            perror("Erro ao abrir o debug.txt(ValidClient function)\n");
            exit(1);
        }
        fprintf(fp,"Program Running on: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        fprintf(fp,"Username: %s\n",user);
        fprintf(fp,"Password: %s\n",pass);
        fprintf(fp,"\n");
        fclose(fp);
    }
    if(u != NULL){
        if(strcmp(pass,u->password) == 0){
            return 1;
        }
    }
    
    
    return 0;
}

int validManager(char str[]){
    int i=0,j=0;
    char username[50],password[50],fuser[50],fpass[50],gestores[STR_MAX_SIZE];
    memset(username,0,50);
    memset(password,0,50);
    memset(fuser,0,50);
    memset(fpass,0,50);
    memset(gestores,0,STR_MAX_SIZE);
    
    while(str[i] != ':'){
        username[i] = str[i];
        i++;
    }
    i++;
    while(str[i] != ':'){
        password[j] = str[i];
        i++;
        j++;
    }
    
    FILE *fp;
    fp = fopen(dirGestor,"r");
    if(fp == NULL){
        printf("Erro ao abrir o arquivo dos gestores.\n");
        exit(1);
    }
    i=0;
    j=0;
    while(fscanf(fp,"%s",gestores) == 1){
        while(gestores[i] != ':'){
            i++;
        }
        i++;
        while(gestores[i] != ':'){
            i++;
        }
        i++;
        while(gestores[i] != ':'){
            fuser[j] = gestores[i];
            i++;
            j++;
        }
        i++;
        j=0;
        while(gestores[i] != ':'){
            fpass[j] = gestores[i];
            i++;
            j++;
        }
    }
    
    if(strcmp(username,fuser)==0){
        if(strcmp(password,fpass) == 0){
            return 1;
        }
    }
    
    fclose(fp);
    return 0;
}

void sendBalance(Users* lst, char str[], int sock){
    char username[50];
    char msg[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    
    int i=0;
    Users* u;
    
    while(str[i] != ':'){
        username[i] = str[i];
        i++;
    }
    u = searchUser(lst,username);
    if(u == NULL){
        strcpy(msg,"0");
    }else{
        strcpy(msg,u->nome);
        strcat(msg,":");
        strcat(msg,u->balance);
        strcat(msg,":");
    }
    if(DEBUG == 1){
        FILE *fp;
        fp = fopen(dirDebug,"a");
        if(fp == NULL){
            perror("Erro ao abrir o debug.txt(sendBalance)\n");
            exit(1);
        }
        fprintf(fp,"sendBalance Function\n");
        fprintf(fp,"Msg enviada: %s\n",msg);
        fprintf(fp,"\n");
        fclose(fp);
    }
    write(sock,msg,strlen(msg));
}

void addBalance(Users* lst, char str[], int sock){
    int i=0,j=0;
    float newBalance=0;
    Users* u;
    char username[STR_MAX_SIZE], balance[STR_MAX_SIZE];
    memset(username,0,STR_MAX_SIZE);
    memset(balance,0,STR_MAX_SIZE);
    
    while(str[i] != ':'){
        username[i] = str[i];
        i++;
    }
    i++;
    while(str[i] != ':'){
        balance[j] = str[i];
        i++;
        j++;
    }
    u = searchUser(lst,username);
    if(u == NULL){
        perror("Erro, username nao encontrado.\n");
        exit(1);
    }
    newBalance = atof(u->balance);
    newBalance = newBalance + atof(balance);
    
    memset(balance,0,STR_MAX_SIZE);
    snprintf(balance,STR_MAX_SIZE,"%f",newBalance);
    strcpy(u->balance,balance);
    
    write(sock,"1",strlen("1"));
    
}

void sendListProdcut(Produto* p, int sock){
    Produto* p1;
    char msg[STR_MAX_SIZE];
    char tmp[STR_MAX_SIZE];
    
    memset(tmp,0,STR_MAX_SIZE);
    memset(msg,0,STR_MAX_SIZE);
    for(p1 = p; p1 != NULL; p1 = p1->next){
        strcat(msg,p1->nome);
        strcat(msg,":");
        strcat(msg,p1->descricao);
        strcat(msg,":");
        strcat(msg,itoa(p1->codigo,10));
        strcat(msg,":");
        strcat(msg,itoa(p1->qtd,10));
        strcat(msg,":");
        snprintf(tmp,STR_MAX_SIZE,"%f", p1->preco);
        strcat(msg,tmp);
        memset(tmp,0,STR_MAX_SIZE);
        strcat(msg,":");
    }
    write(sock,msg,strlen(msg));
}

Produto *searchProduct(Produto *stock,int cod){
    Produto *p;
    
    for(p = stock; p != NULL ;p = p->next){
        if(p->codigo == cod){
            return p;
        }
    }
    return NULL;
}

Produto *updateProducts(Produto *stock,char msg[], int sock){
    Produto *p;
    int cod,i=0,j=0, qtdComprada;
    char tmp[STR_MAX_SIZE];
    memset(tmp,0,STR_MAX_SIZE);
    
    while (msg[i] != ':') {
        tmp[i] = msg[i];
        i++;
    }
    i++;
    cod = atoi(tmp);
    memset(tmp,0,STR_MAX_SIZE);
    while(msg[i] != ':'){
        tmp[j] = msg[i];
        i++;
        j++;
    }
    qtdComprada = atoi(tmp);
    p = searchProduct(stock,cod);
    if(qtdComprada > p->qtd){
        write(sock,"0",strlen("0"));
    }else{
        if(qtdComprada == p->qtd){
            p->qtd = 0;
            write(sock,"1",strlen("1"));
        }else{
            p->qtd = p->qtd - qtdComprada;
            write(sock,"1",strlen("1"));
        }
    }
    return stock;
}

void sendAProduct(int sock, char msg[], Produto *stock){
    Produto *p;
    int i=0,code;
    char str[STR_MAX_SIZE];
    char tmp[STR_MAX_SIZE];
    char aux[STR_MAX_SIZE];
    
    memset(aux,0,STR_MAX_SIZE);
    memset(str,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    
    while(msg[i] != ':'){
        str[i] = msg[i];
        i++;
    }
    i++;
    code = atoi(str);
    
    p = searchProduct(stock,code);
    
    memset(str,0,STR_MAX_SIZE);
    
    strcpy(str,p->nome);
    strcat(str,":");
    snprintf(tmp,STR_MAX_SIZE,"%f", p->preco);//Change float to string;
    for(i=0;i<4;i++){//Make it like eg(1.00 and not 1.00000);
        aux[i] = tmp[i];
    }
    strcat(str,aux);
    strcat(str,":");
    
    write(sock,str,strlen(str));
    
}

Statistics *addStatistics(Statistics* lst, int qtdComprada, int cod, struct tm timer, float valorGasto, char name[]){
    Statistics *novo = (Statistics*) malloc(sizeof(Statistics));
    
    novo->cod = cod;
    strcpy(novo->nome,name);
    novo->qtdComprada = qtdComprada;
    novo->tm = timer;
    novo->valorGasto = valorGasto;
    novo->next = lst;
    return novo;
}

void verifyMoney(int sock, char msg[], Users* usu, Produto* stock){
    int i=0,j=0,codigo;
    Users *u;
    Produto* p;
    char username[50];
    char tmp[10],aux[STR_MAX_SIZE];
    float total, balance;
    
    memset(aux,0,STR_MAX_SIZE);
    memset(username,0,50);
    memset(tmp,0,10);
    
    while(msg[i] != ':'){//Username
        username[i] = msg[i];
        i++;
    }
    i++;
    j=0;
    while(msg[i] != ':'){//Valor que será pago.
        tmp[j] = msg[i];
        i++;
        j++;
    }
    i++;
    total = atof(tmp);
    memset(aux,0,STR_MAX_SIZE);
    j=0;
    while(msg[i] != ':'){//codigo do produto.
        aux[j] = msg[i];
        i++;
        j++;
    }
    codigo = atoi(aux);
    u = searchUser(usu,username);
    p = searchProduct(stock,codigo);
    balance = atof(u->balance);
    
    memset(aux,0,STR_MAX_SIZE);
    if(balance >= total){
        time_t t = time(NULL);
        balance = balance - total;
        memset(aux,0,STR_MAX_SIZE);
        snprintf(aux,STR_MAX_SIZE,"%f\n", balance);
        strcpy(u->balance,aux);
        u->statUsu = addStatistics(u->statUsu, (total / p->preco), codigo, *localtime(&t), total, p->nome);
        write(sock,"1",strlen("1"));
    }else{
        write(sock,"0",strlen("0"));
    }
    
    
}

void sendStatistics(int sock, char client_msg[], Users* lst){
    int i=0;
    Statistics *s;
    Users *u;
    char username[50],aux[50], msg[STR_MAX_SIZE];
    memset(username,0,50);
    memset(aux,0,50);
    memset(msg,0,STR_MAX_SIZE);
    
    while(client_msg[i] != ':'){
        username[i] = client_msg[i];
        i++;
    }
    
    u = searchUser(lst, username);
    
    if(u->statUsu == NULL){
        write(sock,"0",strlen("0"));
    }
    
    
    for(s = u->statUsu ; s != NULL ; s = s->next){
        strcat(msg,itoa(s->cod,10));
        strcat(msg,":");
        strcat(msg,s->nome);
        strcat(msg,":");
        strcat(msg,itoa(s->qtdComprada,10));
        strcat(msg,":");
        snprintf(aux,50,"%f",s->valorGasto);
        strcat(msg,aux);
        strcat(msg,":");
        strcat(msg,itoa(s->tm.tm_hour,10));
        strcat(msg,":");
        strcat(msg,itoa(s->tm.tm_min,10));
        strcat(msg,":");
        strcat(msg,itoa(s->tm.tm_mday,10));
        strcat(msg,":");
    }
    write(sock,msg,strlen(msg));
}

void createNewUser(int sock, char client_message[], Users* lst){//Manger, create new user.
    //Example of reciving data: guilherme:987654321:gui:gui321:1000:
    int i=0,j=0;
    char nome[255],contato[10],username[50],password[50],cBalance[10];
    memset(nome,0,255);
    memset(contato,0,10);
    memset(username,0,50);
    memset(password,0,50);
    memset(cBalance,0,10);
    
    while(client_message[i] != ':'){
        nome[i] = client_message[i];
        i++;
    }
    i++;
    while(client_message[i] != ':'){
        contato[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    while(client_message[i] != ':'){
        username[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    while(client_message[i] != ':'){
        password[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    while(client_message[i] != ':'){
        cBalance[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    
    FILE *fp;
    
    fp = fopen(dirUser,"a");
    if(fp == NULL){
        printf("Erro ao abrir o arquivo dos usuarios (Create new user function).\n");
        write(sock,"0",strlen("0"));
        exit(1);
    }
    fprintf(fp,"%s",nome);
    fprintf(fp,"%s",":");
    fprintf(fp,"%s",contato);
    fprintf(fp,"%s",":");
    fprintf(fp,"%s",username);
    fprintf(fp,"%s",":");
    fprintf(fp,"%s",password);
    fprintf(fp,"%s",":");
    fprintf(fp,"%s\n",cBalance);
    
    fclose(fp);
    
    write(sock,"1",strlen("1"));
    
    
}

void listProducts2Manager(int sock, Produto* lst){
    Produto* p1;
    char msg[STR_MAX_SIZE];
    char tmp[STR_MAX_SIZE];
    
    memset(tmp,0,STR_MAX_SIZE);
    memset(msg,0,STR_MAX_SIZE);
    for(p1 = lst; p1 != NULL; p1 = p1->next){
        strcat(msg,p1->nome);
        strcat(msg,":");
        strcat(msg,p1->descricao);
        strcat(msg,":");
        strcat(msg,itoa(p1->codigo,10));
        strcat(msg,":");
        strcat(msg,itoa(p1->qtd,10));
        strcat(msg,":");
        snprintf(tmp,STR_MAX_SIZE,"%f",p1->custo);
        strcat(msg,tmp);
        memset(tmp,0,STR_MAX_SIZE);
        strcat(msg,":");
        snprintf(tmp,STR_MAX_SIZE,"%f", p1->preco);
        strcat(msg,tmp);
        memset(tmp,0,STR_MAX_SIZE);
        strcat(msg,":");
    }
    write(sock,msg,strlen(msg));
}

Produto* addNewProduct(int sock,char client_message[], Produto* stock){
    char nome[50],desc[50],tmp[STR_MAX_SIZE];
    int codigo, qtd,i=0,j=0;
    float custo,preco;
    Produto *p;
    memset(nome,0,50);
    memset(desc,0,50);
    memset(tmp,0,STR_MAX_SIZE);
    
    while(client_message[i] != ':'){//Get the name
        nome[i] = client_message[i];
        i++;
    }
    i++;
    while(client_message[i] != ':'){//get description
        desc[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    while(client_message[i] != ':'){//get code
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    codigo = atoi(tmp); //change character to int;
    i++;
    j=0;
    memset(tmp,0,STR_MAX_SIZE); //clear the mem to use tmp again.
    while(client_message[i] != ':'){//get cost
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    custo = atof(tmp);
    memset(tmp,0,STR_MAX_SIZE);//Clear the mem to use tmp again
    while(client_message[i] != ':'){//get price
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    i++;
    j=0;
    preco = atof(tmp);
    memset(tmp,0,STR_MAX_SIZE);//Clear again.
    while(client_message[i] != ':'){//get qtty
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    qtd = atoi(tmp);
    i++;
    j=0;
    
    p = searchProduct(stock, codigo);
    if(p != NULL){//Send -1 to manager to inform that the code already exist.
        write(sock,"-1",strlen("-1"));
        return stock;
    }
    //add new product
    stock = addProduto(stock,nome , codigo, desc, qtd, custo, preco);
    
    write(sock,"1",strlen("1"));
    
    return stock;
    
}

void verifyProduct(int sock, char client_message[], Produto* stock){
    Produto* p;
    int codigo;
    
    codigo = atoi(client_message);
    
    p = searchProduct(stock, codigo);
    
    if(p != NULL){
        write(sock,"1",strlen("1"));
        return;
    }
    
    write(sock,"0",strlen("0"));
    
}

Produto* editQtty(int sock, char client_message[], Produto* stock){
    int cod, i=0, j=0, qtd;
    char msg[STR_MAX_SIZE],tmp[STR_MAX_SIZE];
    Produto* p;
    
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    
    while(client_message[i] != ':'){
        tmp[i] = client_message[i];
        i++;
    }
    i++;
    qtd = atoi(tmp);
    memset(tmp,0,STR_MAX_SIZE);
    while(client_message[i] != ':'){
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    cod = atoi(tmp);
    
    p = searchProduct(stock,cod);
    
    p->qtd = qtd;
    
    write(sock,"1",strlen("1"));
    
    return stock;
    
}

Produto* editCost(int sock, char client_message[], Produto* stock){
    Produto* p;
    char msg[STR_MAX_SIZE], tmp[STR_MAX_SIZE];
    int i=0,j=0;
    float custo, cod;
    
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    
    while(client_message[i] != ':'){
        tmp[i] = client_message[i];
        i++;
    }
    i++;
    custo = atof(tmp);
    memset(tmp,0,STR_MAX_SIZE);
    while(client_message[i] != ':'){
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    cod = atoi(tmp);
    
    p = searchProduct(stock, cod);
    
    p->custo = custo;
    
    write(sock,"1",strlen("1"));
    
    return stock;
    
}

Produto* editPrice(int sock, char client_message[], Produto* stock){
    Produto* p;
    char msg[STR_MAX_SIZE], tmp[STR_MAX_SIZE];
    int i=0,j=0;
    float price, cod;
    
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    
    while(client_message[i] != ':'){
        tmp[i] = client_message[i];
        i++;
    }
    i++;
    price = atof(tmp);
    memset(tmp,0,STR_MAX_SIZE);
    while(client_message[i] != ':'){
        tmp[j] = client_message[i];
        i++;
        j++;
    }
    cod = atoi(tmp);
    
    p = searchProduct(stock, cod);
    
    p->preco = price;
    
    write(sock,"1",strlen("1"));
    
    return stock;
    
}

Produto* removeProduct(int sock,char client_message[] , Produto* stock){
    Produto* a = NULL;
    Produto* p = stock;
    int i=0,cod;
    char msg[STR_MAX_SIZE],tmp[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    
    
    while(client_message[i] != ':'){
        tmp[i] = client_message[i];
        i++;
    }
    i++;
    cod = atoi(tmp);
    
    //Search for the product, keeping the previus one.
    while(p != NULL && p->codigo != cod){
        a = p;
        p = p->next;
    }
    
    //If dont find it, just leave the way it is;
    if(p == NULL){
        return stock;
    }
    
    //If the product is the first one.
    if(a == NULL){
        stock = p->next;
    }else{//Or not.
        a->next = p->next;
    }
    //Realse memory
    free(p);
    
    write(sock,"1",strlen("1"));
    
    //return
    return stock;
}

void listUsers2Manager(int sock,Users* lst){
    char msg[STR_MAX_SIZE];
    Users* u;
    memset(msg,0,STR_MAX_SIZE);
    
    for(u=lst;u!=NULL;u=u->next){
        strcat(msg,u->nome);
        strcat(msg,":");
        strcat(msg,u->username);
        strcat(msg,":");
        strcat(msg,u->contato);
        strcat(msg,":");
    }
    write(sock,msg,strlen(msg));
    
}

void verifyUserExist(int sock, char client_message[], Users* lst){
    Users* u;
    
    u = searchUser(lst, client_message);
    
    if(u == NULL){
        write(sock,"0",strlen("0"));
    }else{
        write(sock,"1",strlen("1"));
    }
    
}

Produto* addProductBackToStock(int sock,char client_message[], Produto* stock){
    Produto* p;
    int code, i=0, qtd=0,j=0;
    char codeChar[STR_MAX_SIZE],qtdChar[STR_MAX_SIZE];
    
    memset(codeChar,0,STR_MAX_SIZE);
    memset(qtdChar,0,STR_MAX_SIZE);
    
    while(client_message[i] != ':'){
        codeChar[i] = client_message[i];
        i++;
    }
    i++;
    while(client_message[i] != ':'){
        qtdChar[j] = client_message[i];
        i++;
        j++;
    }
    
    code = atoi(codeChar);
    qtd = atoi(qtdChar);
    
    p = searchProduct(stock, code);
    
    if(p == NULL){//didnt find the product
        write(sock,"0",strlen("0"));
        return stock;
    }
    
    p->qtd = p->qtd + qtd;
    
    write(sock,"1",strlen("1"));
    
    return stock;
}

//Save all progress to the files.(stock, statistics, users.)
void writeToFiles(int sock, Produto* stock,Users* users){
    Produto* p;
    Users* u;
    Statistics *s;
    char write2File[STR_MAX_SIZE];
    
    FILE *fp,*fp2;
    
    fp = fopen(dirStock,"w");
    
    
    
    if(fp == NULL){
        printf("Erro ao gravar o stock!\n");
        write(sock,"0",strlen("0"));
        exit(1);
    }
    
    for(p = stock;p!=NULL;p = p->next){
        fprintf(fp,"%s\n",p->nome);
        fprintf(fp,"%d\n",p->codigo);
        fprintf(fp,"%s\n",p->descricao);
        fprintf(fp,"%d\n",p->qtd);
        fprintf(fp,"%f\n",p->custo);
        fprintf(fp,"%f\n",p->preco);
        fprintf(fp,"%d\n",p->qtdSold);
    }
    fclose(fp);
    
    if(fp == NULL){
        printf("Erro ao gravar os usuarios!\n");
        write(sock,"0",strlen("0"));
        exit(1);
    }
    
    fp = fopen(dirUser,"w");
    fp2 = fopen(dirStats,"w");
    
    for(u=users;u!=NULL;u=u->next){
        memset(write2File,0,STR_MAX_SIZE);
        strcpy(write2File,u->nome);
        strcat(write2File,":");
        strcat(write2File,u->contato);
        strcat(write2File,":");
        strcat(write2File,u->username);
        strcat(write2File,":");
        strcat(write2File,u->password);
        strcat(write2File,":");
        strcat(write2File,u->balance);
        fprintf(fp,"%s\n",write2File);
        for(s=u->statUsu;s!=NULL;s=s->next){
            fprintf(fp2,"%s\n",u->username);
            fprintf(fp2,"%s\n",s->nome);
            fprintf(fp2,"%d\n",s->qtdComprada);
            fprintf(fp2,"%d\n",s->cod);
            fprintf(fp2,"%f\n",s->valorGasto);
            fprintf(fp2,"%d\n",s->tm.tm_hour);
            fprintf(fp2,"%d\n",s->tm.tm_min);
            fprintf(fp2,"%d\n",s->tm.tm_mday);
            fprintf(fp2,"%d\n",s->tm.tm_mon);
        }
    }
    fclose(fp2);
    fclose(fp);
    
    write(sock,"1",strlen("1"));

    
}

void loadStatistics(Users* users){
    Users* u;
    FILE *fp;
    struct tm tm;
    char username[50],nomeProd[50];
    int qtdComprada,codigo,hour,min,mday,mon;
    float valorGasto;
    
    fp = fopen(dirStats,"r");
    if(fp == NULL){
        printf("Erro ao tentar ler o arquivo de statistics. \n");
        printf("Verifique os defines no LibServ.h \n");
        exit(1);
    }
    
    while(fscanf(fp,"%s%s%d%d%f%d%d%d%d",username,nomeProd,&qtdComprada,&codigo,&valorGasto,&hour,&min,&mday,&mon) == 9){
        tm.tm_hour = hour;
        tm.tm_mon = mon;
        tm.tm_mday = mday;
        tm.tm_min = min;
        u = searchUser(users, username);
        if(u == NULL){
            printf("Erro ao carregar as estatisticas, usuario nao encontrado. \n");
            exit(1);
        }
        u->statUsu = addStatistics(u->statUsu, qtdComprada, codigo, tm, valorGasto, nomeProd);
    }
    
    
}

void *connection_handler(void* socket_desc){
    struct sockHandle *sh = socket_desc;
    //Get the socket descriptor
    //int sock = *(int*)socket_desc;
    int sock = sh->sock;
    int read_size;
    char client_message[STR_MAX_SIZE];
    int command;
    
    //Receive a message from client
    while( (read_size = (int)recv(sock , client_message , STR_MAX_SIZE , 0)) > 0 )
    {
        command = getCommand(client_message);
        if(command == 1){//To valid the manager
            if(validManager(client_message) == 1){
                write(sock,"1",strlen("1"));
            }else{
                write(sock,"0",strlen("0"));
            }
        }else if(command == 2){//To Valid the client
            if(validClient(sh->users,client_message) == 1){
                write(sock,"1",strlen("1"));
            }else{
                write(sock,"0",strlen("0"));
            }
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 3){//Manager Balance(Client)
            sendBalance(sh->users,client_message,sock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 4){//Deposit money(Client)
            addBalance(sh->users,client_message,sock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 5){//List all the products
            sendListProdcut(sh->stock,sock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 6){//Decress the number of a certan product that was add to the cart.
            sh->stock = updateProducts(sh->stock,client_message,sh->sock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 7){//Return a specific product
            sendAProduct(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 8){//Verify if the client has money to buy.
            verifyMoney(sock,client_message,sh->users,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 9){//Send data from the statistics of a user.
            sendStatistics(sock,client_message,sh->users);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 10){//Craete a new user for the market
            createNewUser(sock,client_message,sh->users);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 11){//List all products to manager
            listProducts2Manager(sock,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 12){//Include new product (only manager)
           sh->stock = addNewProduct(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 13){//Return if a product exist.
            verifyProduct(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 14){//Edit the qty of a product
            sh->stock = editQtty(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 15){//Edit the cost of a product
            sh->stock = editCost(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 16){//Edit the price of a product
            sh->stock = editPrice(sock, client_message, sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 17){//Remove a product from the list.
            sh->stock = removeProduct(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 18){//List all users to manager.
            listUsers2Manager(sock,sh->users);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 19){//Verify if a user exist(based in username)
            verifyUserExist(sock,client_message,sh->users);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 20){//After a user remove the product from the cart, it goes back to stock
            addProductBackToStock(sock,client_message,sh->stock);
            memset(client_message,0,STR_MAX_SIZE);
        }else if(command == 21){//Write the changes to the files in the end
            writeToFiles(sock,sh->stock,sh->users);
            memset(client_message,0,STR_MAX_SIZE);
        }
        
        //Send the message back to client
        //write(sock , client_message , strlen(client_message));
        memset(client_message,0,STR_MAX_SIZE);
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
    
    return 0;
}

