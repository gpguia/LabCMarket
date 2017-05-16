//
//  main.c
//  LabCMarketClient
//
//  Created by Guilherme Guia on 3/27/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibClientFunctions.h"

int main(int argc, const char * argv[]) {
    system("clear");
    int sock;
    char username[50],*password;
    char message[STR_MAX_SIZE];
    char server_replay[STR_MAX_SIZE];
    memset(message,0,STR_MAX_SIZE);
    memset(server_replay,0,STR_MAX_SIZE);
    sock = connToServer();
    
    printf("Username: ");
    scanf("%s",username);
    password = getpass("Password:");
    strcpy(message,"2:");
    strcat(message,username);
    strcat(message,":");
    strcat(message,password);
    strcat(message,":");
    writeToServer(sock,message,server_replay);
    while(strcmp(server_replay,"1") != 0){
        memset(message,0,STR_MAX_SIZE);
        memset(server_replay,0,STR_MAX_SIZE);
        system("clear");
        printf("Usuario invalido, tente novamente.\n");
        memset(username,0,50);
        memset(password,0,50);
        memset(message,0,101);
        printf("Username: ");
        scanf("%s",username);
        password = getpass("Password:");
        strcpy(message,"2:");
        strcat(message,username);
        strcat(message,":");
        strcat(message,password);
        strcat(message,":");
        writeToServer(sock,message,server_replay);
    }
    system("clear");
    showMenu(sock,username);
    return 0;
}
