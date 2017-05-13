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


//Dont forget to change this, so the program can start.
#define dirStock "/home/gpguia/LabCMarket/LabCMarketServ/LabCMarketServ/stock.txt"
#define dirUser "/home/gpguia/LabCMarket/LabCMarketServ/LabCMarketServ/users.txt"
#define dirGestor "/home/gpguia/LabCMarket/LabCMarketServ/LabCMarketServ/gestores.txt"
#define dirDebug "/home/gpguia/LabCMarket/LabCMarketServ/LabCMarketServ/debug.txt"

#define STR_MAX_SIZE 2000

//If debug is set to 1, it will write some info in debug file, just for debug the program.
#define DEBUG 0

typedef struct statistics Statistics;

struct statistics{
    int qtdComprada;
    int cod;
    struct tm tm;
    float valorGasto;
    char nome[50];
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
    char balance[20];
    Statistics* statUsu;
    Users* next;
};

struct sockHandle{
    int sock;
    Users* users;
    Produto* stock;
};

char* itoa(int val, int base);
int createConn(Users* lst, Produto* stock);
int getCommand(char msg[]);
void *connection_handler(void* socket_desc);

Users* createUsers();
Users* addUser(Users* lst, char nome[], char contato[], char user[], char pass[], char* balance);
Users* searchUser(Users* lst, char username[]);
Users* loadUsers(Users* lst);
Users* deleteListUsu(Users* usu);
void getName(char str[], char name[]);
void getContact(char str[], char contato[]);
void getUserName(char str[], char username[]);
void getPassword(char str[],char pass[]);
void getBalance(char str[], char balance[]);
void printUsers(Users* usu);
void verifyMoney(int sock, char msg[], Users* usu, Produto* stock);

Produto *addProduto(Produto *stock, char nome[], int codigo, char desc[], int qtd, float custo, float preco);
Produto *loadStock(Produto *stock, char caminho[]);
Produto *createStock();
void printStock(Produto* stock);
Produto *updateProducts(Produto *stock,char msg[], int sock);
Produto *searchProduct(Produto *stock,int cod);
void sendAProduct(int sock, char msg[], Produto *stock);
void sendStatistics(int sock, char client_msg[], Users* lst);
int validClient(Users* lst, char str[]);
int validManager(char str[]);
void addBalance(Users* lst, char str[], int sock);
void sendListProdcut(Produto* p, int sock);
void sendBalance(Users* lst, char str[], int sock);

//Manager functions
int validManager(char str[]);
void createNewUser(int sock, char client_message[], Users* lst);
void listProducts2Manager(int sock, Produto* lst);
Produto* addNewProduct(int sock,char client_message[], Produto* stock);
void verifyProduct(int sock, char client_message[], Produto* stock);
Produto* editQtty(int sock, char client_message[], Produto* stock);
Produto* editCost(int sock, char client_message[], Produto* stock);
Produto* editPrice(int sock, char client_message[], Produto* stock);
Produto* removeProduct(int sock,char client_message[] , Produto* stock);
void listUsers2Manager(int sock,Users* lst);
void verifyUserExist(int sock, char client_message[], Users* lst);

Statistics *addStatistics(Statistics* lst, int qtdComprada, int cod, struct tm timer, float valorGasto, char name[]);

#endif /* LibServ_h */
