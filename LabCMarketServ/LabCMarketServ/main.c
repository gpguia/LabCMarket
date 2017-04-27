//
//  main.c
//  LabCMarketServ
//
//  Created by Guilherme Guia on 4/26/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibServ.h"

int main(int argc, const char * argv[]) {
    
    //arrumar as structs pra funcionar
    Produto* noStock;
    noStock = createStock();
    noStock = loadStock(noStock,dirStock);
    
    //Load all users
    Users* users;
    users = createUsers();
    users = loadUsers(users);
    
    createConn(users,noStock);
    
    char msg[STR_MAX_SIZE];
    
    strcpy(msg, "2:gui:guia321");
    getCommand(msg);
    return 0;
}
