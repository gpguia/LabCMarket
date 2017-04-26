//
//  LibServer.h
//  LabCMarketServer
//
//  Created by Guilherme Guia on 4/26/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#ifndef LibServer_h
#define LibServer_h

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define dirStock "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/stock.txt"
#define dirUser "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/users.txt"
#define dirGestor "/Users/guilhermeguia/Google Drive/School/UP/2Semestre/LabDeComp/Project/LabCMarketServer/LabCMarketServer/gestores.txt"
#define STR_MAX_SIZE 1024



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


/*
==================================
=========Stocks Functions=========
==================================
*/


Produto *createStock();
Produto *loadStock(Produto *stock, char caminho[]);
Produto *addProduto(Produto *stock, char str[]);
char *getProductName(char str[]);
int getProductCode(char str[]);
char *getProductDes(char str[]);
int getProductQtty(char str[]);
float getProductCost(char str[]);
float getProductPrice(char str[]);
int getProductQtdSold(char str[]);
Produto *searchProduct(Produto *stock, int codigo);
void writeChanges(Produto* stock);
void printStock(Produto* stock);
Produto* removeProduto(Produto* stock, int cod);

/*
==================================
========Socket Functions==========
==================================
*/

int startServer();
void *connection_handler(void *socket_desc);
char* writeToClient(int socket, char message[]);


/*
======================================
========Statistics Functions==========
======================================
*/


Statistics* searchStats(Statistics* s, int cod);
Statistics* updateStatistics(Statistics* s, int cod, int qtdCom);

#endif /* LibServer_h */
