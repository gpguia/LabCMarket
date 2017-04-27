//
//  LibClientFunctions.c
//  LabCMarketClient
//
//  Created by Guilherme Guia on 3/27/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibClientFunctions.h"


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

char* writeToServer(int sock, char message[]){
    
    char* server_reply = (char*) malloc(2000 * sizeof(char));
    memset(server_reply,0,2000);
    
    if( send(sock , message , strlen(message) , 0) < 0)
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


void showMenu(){
    int option;
    printf("**MENU**\n");
    printf("1) Gerir Saldo\n");
    printf("2) Gerir Lista de Compras\n");
    printf("3) Ver Estatisticas\n");
    printf("4) Logout\n");
    scanf("%d",&option);
    while(option != 4){
        if(option == 1){
            //manegeBalance(client);
        }
    }
}
