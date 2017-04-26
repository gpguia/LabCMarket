//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/28/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"

int testSocket(){
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
        
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    
    return 0;
}



int main(int argc, const char * argv[]) {
    char username[50],password[50];
    system("clear");
    
    //arrumar as structs pra funcionar
    Produto* noStock;
    noStock = createStock();
    noStock = loadStock(noStock,dirStock);
    
    //Load all users
    Users* users;
    users = createUsers();
    users = loadUsers(users);
    
    
    testSocket();
    
    //users = updateBuyer(users,10005,10,"gui");
    //users = updateBuyer(users,10005,20,"gui");
    //users = updateBuyer(users,10006,20,"gui");
    //updateStock(noStock, 10005, 10);
    //updateStock(noStock, 10005, 20);
    
   
    /*
    printf("Username : \n");
    scanf("%s",username);
    printf("Password : \n");
    scanf("%s",password);
    while(validGestor(username, password) == false){
        printf("Gestor invalido, tente novamente.\n");
        printf("Username : \n");
        scanf("%s",username);
        printf("Password : \n");
        scanf("%s",password);
    }
    system("clear");
    
    //system("clear");
    //socketCreate();
    noStock = showOptions(noStock,users);
    writeChanges(noStock);
     */
    return 0;
}
