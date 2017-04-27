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
    char nomeProduto[STR_MAX_SIZE];
    Carts* next;
};


void startProgram();

bool validUser(char username[], char password[]);

void error(char *msg);

void showMenu(int sock, char *username);
void showManageOptions(int sock, char username[], char server_replay[]);
void manegeBalance(char str[]);
void listProducts(int sock, char server_replay[]);

int connToServer();
void writeToServer(int sock, char message[], char server_reply[]);

#endif /* LibClientFunctions_h */
