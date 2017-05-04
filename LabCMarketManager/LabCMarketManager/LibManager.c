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
    char username[50], password[50],server_reply[STR_MAX_SIZE];
    char message[STR_MAX_SIZE];
    memset(message,0,STR_MAX_SIZE);
    memset(server_reply,0,STR_MAX_SIZE);
    
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
    write2Server(sock,message,server_reply);
    while(strcmp(server_reply,"1") != 0){
        system("clear");
        printf("Usuario nao encontrado, tente novamente.\n");
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
        write2Server(sock,message,server_reply);
    }
    system("clear");
    showOptions(sock);
}

void addnewUser(int sock, char server_reply[]){
    int option=0;
    char msg[STR_MAX_SIZE];
    char nome[255],contato[10],username[50],password[50];
    memset(nome,0,255);
    memset(contato,0,10);
    memset(username,0,50);
    memset(password,0,50);
    memset(msg,0,STR_MAX_SIZE);
    
    
    printf("Digite o nome: \n");
    scanf("%s",nome);
    printf("Digite o contato (max 9 digitos): \n");
    scanf("%s",contato);
    printf("Digite o username: \n");
    scanf("%s",username);
    printf("Digite a senha: \n");
    scanf("%s",password);
    
    strcpy(msg,"10");
    strcat(msg,":");
    strcat(msg,nome);
    strcat(msg,":");
    strcat(msg,contato);
    strcat(msg,":");
    strcat(msg,username);
    strcat(msg,":");
    strcat(msg,password);
    strcat(msg,":");
    strcat(msg,"1000");
    strcat(msg,":");
    
    printf("Porfavor, confirme as informacoes: \n");
    printf("Nome: %s\nContato: %s\nUsername: %s\n,Password: %s\n",nome,contato,username,password);
    printf("\nDigite 1 para confirmar, 0 para digitar novamente ou -1 para cancelar\n");
    scanf("%d",&option);
    while(option == 1){
      //AQUII
    }
    
    write2Server(sock,msg,server_reply);
    if(strcmp(server_reply,"1") == 0){
        printf("Usuario criado com sucesso!\n");
    }else{
        printf("Algum erro ocorreu.]n");
    }
    
}

void showOptions(int sock){
    int option;
    char server_reply[STR_MAX_SIZE];
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
            addnewUser(sock,server_reply);
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

char *write2Server(int sock, char msg[], char server_reply[]){
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
    return server_reply;
}






