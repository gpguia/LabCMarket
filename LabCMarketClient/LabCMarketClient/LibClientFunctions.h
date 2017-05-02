//
//  LibClientFunctions.h
//  LabCMarketClient
//
//  Created by Guilherme Guia on 3/27/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#ifndef LibClientFunctions_h
#define LibClientFunctions_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "netinet/in.h"
#include <stdbool.h>

#define servip 127.0.0.1
#define PORT 8888
#define BUF_SIZE 2000
#define STR_MAX_SIZE 2000

typedef struct cart Carts;

struct cart{
    int cod;
    int qtd;
    Carts* next;
};



void startProgram();

char* itoa(int val, int base);

bool validUser(char username[], char password[]);

void error(char *msg);

void showMenu(int sock, char *username);
void showManageOptions(int sock, char username[], char server_replay[]);
void manegeBalance(char str[]);
void listProducts(int sock, char server_replay[], int codigos[], int *qtdProduct);
void listStatistics(int sock, char username[], char server_repaly[]);
float listProductInCart(Carts *lst, int sock, char server_reply[]);

int connToServer();
void writeToServer(int sock, char message[], char server_reply[]);


Carts *manageProducts(int sock, char server_reply[], Carts *c, char username[]);
Carts *createCart();
Carts *addProduct2Cart(int sock,char server_reply[] , Carts *lst, int code, int qtd);
Carts *searchCart(Carts *lst, int code);
Carts *manageCart(Carts *lst, int sock, char server_reply[], char username[]);
int checkProductList(int qtdProduct, int lst[], int key);
Carts *checkout(Carts *lst, char username[], char server_reply[], int sock, float total, int cod);
Carts *removeProductFromCart(Carts *lst, int code);



#endif /* LibClientFunctions_h */
