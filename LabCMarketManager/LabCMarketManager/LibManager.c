//
//  LibServer.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/29/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"

int connToServer(){
    int sock;
    struct sockaddr_in server;
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    puts("Connected\n");
    return sock;
}


void validManager(int sock){
    char username[50], password[50];
    char message[STR_MAX_SIZE];
    memset(message,0,STR_MAX_SIZE);
    
    system("clear");
    printf("Autentique-se:\n");
    printf("Username: ");
    scanf("%s",username);
    printf("Password: ");
    scanf("%s",password);
    strcpy(message,"1:");
    strcat(message,username);
    strcat(message,":");
    strcat(message,password);
    strcat(message,":");
    while(strcmp(write2Server(sock,message),"1") != 0){
        system("clear");
        printf("Usuario nao encontrado, tente novamente.\n");
        printf("Autentique-se:\n");
        printf("Username: ");
        scanf("%s",username);
        printf("Password: ");
        scanf("%s",password);
        strcpy(message,"1:");
        strcat(message,username);
        strcpy(message,":");
        strcpy(message,password);
        strcpy(message,":");
    }
    system("clear");
    showOptions(sock);
}

void showOptions(int sock){
    int option;
    printf("**Menu Principal**\n");
    printf("1) Criar novo utilizador\n");
    printf("2) Gerir Stocks\n");
    printf("3) Ver Estatisticas\n");
    printf("4) Logout\n");
    scanf("%d",&option);
    while(option != 4){
        if(option == 1){
            system("clear");
            option = -2;
        }else if(option == 2){
            system("clear");
            option = -2;
        }else if(option == 3){
            system("clear");
            option = -2;
        }else if(option == 4){
            return;
        }else if(option == -2){
            system("clear");
            printf("**Menu**\n");
            printf("1) Criar novo utilizador\n");
            printf("2) Gerir Stocks\n");
            printf("3) Ver Estatisticas\n");
            printf("4) Logout\n");
            scanf("%d",&option);
        }else{
            printf("Verifique o numero digitado.\n");
            scanf("%d",&option);
        }
    }
}

char *write2Server(int sock, char msg[]){
    char* server_reply = (char*) malloc(2000 * sizeof(char));
    memset(server_reply,0,2000);
    
    if( send(sock , msg , strlen(msg) , 0) < 0)
    {
        puts("Send failed");
        return "-1";
    }

    if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
        return "-1";
    }
    printf("%s\n",server_reply);
    return server_reply;
}






