//
//  LibServ.h
//  LabCMarketServ
//
//  Created by Guilherme Guia on 4/26/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#ifndef LibServ_h
#define LibServ_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
//Socket
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h>

#define dirStock "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/stock.txt"
#define dirUser "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/users.txt"
#define dirGestor "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/gestores.txt"


#define STR_MAX_SIZE 2000
#define DEBUG 1
#define dirDebug "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServ/LabCMarketServ/debug.txt"

typedef struct statistics Statistics;

struct statistics{
    int qtdComprada;
    int cod;
    Statistics* next;
};

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


typedef struct users Users;

struct users{
    char nome[255];
    char contato[10];
    char username[50];
    char password[50];
    char balance[10];
    Statistics* statUsu;
    Users* next;
};

struct sockHandle{
    int sock;
    Users* users;
    Produto* stock;
};


int createConn(Users* lst, Produto* stock);
int getCommand(char msg[]);
void *connection_handler(void* socket_desc);

Users* createUsers();
Users* addUser(Users* lst, char nome[], char contato[], char user[], char pass[], char* balance);
Users* searchUser(Users* lst, char username[]);
Users* loadUsers(Users* lst);
Users* deleteListUsu(Users* usu);
char *getName(char str[]);
char *getContact(char str[]);
char *getUserName(char str[]);
char *getPassword(char str[]);
char *getBalance(char str[]);
void printUsers(Users* usu);

Produto *addProduto(Produto *stock, char nome[], int codigo, char desc[], int qtd, float custo, float preco);
Produto *loadStock(Produto *stock, char caminho[]);
Produto *createStock();
void printStock(Produto* stock);

int validClient(Users* lst, char str[]);
#endif /* LibServ_h */
