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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "netinet/in.h"
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>

#define STR_MAX_SIZE 2000

int connToServer();
void validManager(int sock);
void showOptions(int sock);

char *write2Server(int sock, char msg[], char server_reply[]);


int addnewUser(int sock, char server_reply[]);
void manageStock(int sock,char server_reply[]);
void listStock(int sock, char server_reply[]);
void includeNewProduct(int sock, char server_reply[]);
void editStock(int sock, char server_reply[]);
char* itoa(int val, int base);
#endif /* LibServer_h */
