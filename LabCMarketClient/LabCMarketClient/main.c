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
    char username[50],password[50];
    char message[101];
    memset(message,0,101);
    sock = connToServer();
    
    printf("Username: ");
    scanf("%s",username);
    printf("Password: ");
    scanf("%s",password);
    strcpy(message,username);
    strcat(message,":");
    strcat(message,password);
    while(strcmp(writeToServer(sock,message),"1") != 0){
        system("clear");
        printf("Usuario invalido, tente novamente.\n");
        memset(username,0,50);
        memset(password,0,50);
        memset(message,0,101);
        printf("Username: ");
        scanf("%s",username);
        printf("Password: ");
        scanf("%s",password);
        strcpy(message,username);
        strcat(message,":");
        strcat(message,password);
    }
    
    return 0;
}