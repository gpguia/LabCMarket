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

void writeToServer(int sock, char message[], char server_reply[]){
    
    memset(server_reply,0,STR_MAX_SIZE);
    
    if( send(sock , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        exit(1);
    }
    
    if( recv(sock , server_reply , STR_MAX_SIZE , 0) < 0)
    {
        puts("recv failed");
        exit(1);
    }
    
}

void manegeBalance(char server_replay[]){
    int i=0,j=0;
    char name[STR_MAX_SIZE],balance[STR_MAX_SIZE];
    memset(name,0,STR_MAX_SIZE);
    memset(balance,0,STR_MAX_SIZE);
    
    while(server_replay[i] != ':'){
        name[i] = server_replay[i];
        i++;
    }
    i++;
    while(server_replay[i] != ':'){
        balance[j] = server_replay[i];
        i++;
        j++;
    }
    printf("\t%s","** Saldo **\n");
    printf("Nome: %s",name);
    printf("\n");
    printf("Saldo: %s",balance);
    printf("\n");
    printf("\t%s","** Fim Saldo **\n\n");
    memset(server_replay,0,STR_MAX_SIZE);
}

void showManageOptions(int sock, char username[], char server_replay[]){
    int option=0;
    char deposit[STR_MAX_SIZE];
    char msg[STR_MAX_SIZE];
    memset(deposit,0,STR_MAX_SIZE);
    memset(msg,0,STR_MAX_SIZE);
    
    printf("1) Adicionar mais dinheiro\n");
    printf("2) Voltar ao menu anterior\n");
    scanf("%d",&option);
    
    while(option != 2){
        if(option == 1){
            option = 2;
            system("clear");
            printf("Digite a quantidade que deseja depositar: \n");
            scanf("%s",deposit);
            strcpy(msg,"4:");
            strcat(msg,username);
            strcat(msg,":");
            strcat(msg,deposit);
            strcat(msg,":");
            writeToServer(sock,msg,server_replay);
        }else if(option == 2){
            return;
        }else{
            printf("Opcao nao reconhecida.\n");
            printf("Tente novamente: \n");
            scanf("%d",&option);
        }
    }
    
    memset(server_replay,0,STR_MAX_SIZE);
}

void listProducts(int sock, char server_replay[]){
    char msg[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    strcpy(msg,"5:");
    
    writeToServer(sock,msg,server_replay);
    
    
    
    memset(server_replay,0,STR_MAX_SIZE);
}

void showMenu(int sock, char *username){
    int option;
    char msg[STR_MAX_SIZE];
    char server_repaly[STR_MAX_SIZE];
    Carts* c;
    
    printf("**MENU**\n");
    printf("1) Gerir Saldo\n");
    printf("2) Gerir Lista de Compras\n");
    printf("3) Ver Estatisticas\n");
    printf("4) Logout\n");
    scanf("%d",&option);
    while(option != 4){
        if(option == 1){
            option = -2;
            system("clear");
            memset(msg,0,STR_MAX_SIZE);
            strcpy(msg,"3:");
            strcat(msg,username);
            strcat(msg,":");
            writeToServer(sock,msg,server_repaly);
            manegeBalance(server_repaly);
            showManageOptions(sock,username,server_repaly);
        }else if(option == 2){
            
        }else if(option == -2){
            system("clear");
            printf("**MENU**\n");
            printf("1) Gerir Saldo\n");
            printf("2) Gerir Lista de Compras\n");
            printf("3) Ver Estatisticas\n");
            printf("4) Logout\n");
            scanf("%d",&option);
        }
    }
}
