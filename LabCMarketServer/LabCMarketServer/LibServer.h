//
//  LibServer.h
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/29/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#ifndef LibServer_h
#define LibServer_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 9999
#define BUF_SIZE 2000
#define CLADDR_LEN 100
#define STR_MAX_SIZE 1024
#define dirStock "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/stock.txt"
#define dirUser "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/users.txt"
#define dirGestor "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/gestores.txt"

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

bool validGestor(char user[], char pass[]);
Produto *loadStock(Produto *stock, char caminho[]);
Produto *addProduto(Produto *stock, char nome[], int codigo, char desc[], int qtd, float custo, float preco);
Produto *searchProduct(Produto *stock, int codigo);
void printStock(Produto* stock);
char *validLogin(Users* lst, char login[]);
void socketCreate();
Produto *showOptions(Produto *stock, Users* usu);
void waitBuyers(int newsockfd);
int createNewUser();
Produto* manageStock(Produto *stock);
void writeChanges(Produto* stock);
Produto* showIncludeOptions(Produto* stock);
Produto* editProduct(Produto* stock);
Produto* removeProduto(Produto* stock, int cod);
Produto* sellProduct(Produto* stock, int codigo);
char *getName(char str[]);
char *getContact(char str[]);
char *getUserName(char str[]);
char *getPassword(char str[]);
Users* createUsers();
Users* addUser(Users* lst, char nome[], char contato[], char user[], char pass[], char* balance);
Users* searchUser(Users* lst, char username[]);
Users* loadUsers(Users* lst);
Users* deleteListUsu(Users* usu);
void printUsers(Users* usu);
char* getBalance(char str[]);

//Statistics
void showStatistics(Produto* stock,Users* usu);
void showStasStock(Produto* stock);
void showStatsUsuMenu(Users* usu, Produto* stock);
void showStatsUsu(Users* usu,Produto* stock, char user[]);
float calcPercentProfit(Produto* p);
float calcPercentRemaining(Produto* p);
Statistics* searchStats(Statistics* s, int cod);
Statistics* updateStatistics(Statistics* s, int cod, int qtdCom);

void updateStock(Produto* stock, int cod, int qtd);

Users* updateBuyer(Users* usu, int cod, int qtdCom, char username[]);


//Socket

void *connection_handler(void *);





#endif /* LibServer_h */
