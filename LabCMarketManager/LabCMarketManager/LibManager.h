//
//  LibServer.h
//  LabCMarketServer
//
//  Created by Guilherme Guia and Jorge Sousa on 3/29/17.
//  Copyright © 2017 Guilherme Guia and Jorge Sousa. All rights reserved.
//

#ifndef LibServer_h
#define LibServer_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
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
int exec_program(pid_t pid);

void validManager(int sock);
void showOptions(int sock);

char *write2Server(int sock, char msg[], char server_reply[]);


int addnewUser(int sock, char server_reply[]);
void manageStock(int sock,char server_reply[]);
void listStock(int sock, char server_reply[]);
void includeNewProduct(int sock, char server_reply[]);
void editStock(int sock, char server_reply[]);
char* itoa(int val, int base);
void showStatisticsOptions(int sock,char server_reply[]);
void listAllUsers(int sock,char server_reply[]);
void listStatisticsSpecificUser(int sock, char server_reply[]);
#endif /* LibServer_h */
