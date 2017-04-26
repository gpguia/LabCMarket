//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/28/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"


int main(int argc, const char * argv[]) {
    char username[50],password[50];
    system("clear");
    
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
