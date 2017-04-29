//
//  LibServ.c
//  LabCMarketServ
//
//  Created by Guilherme Guia on 4/26/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
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
    int socket_desc , client_sock , c ;//*new_sock;
    struct sockaddr_in server , client;
    struct sockHandle sh;
    sh.stock = stock;
    sh.users = lst;
    
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
        //new_sock = malloc(1);
        //*new_sock = client_sock;
        sh.sock = client_sock;
        
        
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler ,&sh) < 0)
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
    printf("---------------------------------------------------------\n");
    for(u = usu;u!=NULL;u=u->next){
        printf("%12s",u->nome);
        printf("%15s",u->contato);
        printf("%12s",u->username);
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
    char user[STR_MAX_SIZE],pass[STR_MAX_SIZE];
    Users* u;
    
    memset(user,0,STR_MAX_SIZE);
    memset(pass,0,STR_MAX_SIZE);
    
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
        if(strcmp(u->password,pass) == 0){
            return 1;
        }
    }
    
    
    return 0;
}

int validManager(char str[]){
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
    int i=0,j=0,newBalance=0;
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
    newBalance = atoi(u->balance);
    newBalance = newBalance + atoi(balance);
    
    strcpy(u->balance,itoa(newBalance,10));
    
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
                
            }
            
        }else if(command == 2){//To Valid the client
            if(validClient(sh->users,client_message) == 1){
                write(sock,"1",2);
            }else{
                write(sock,"0",2);
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
        }
        
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
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
