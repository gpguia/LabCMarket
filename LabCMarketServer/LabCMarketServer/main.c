//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/28/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibServer.h"

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
    
     noStock = showOptions(noStock,users);
     writeChanges(noStock);
    return 0;
}
