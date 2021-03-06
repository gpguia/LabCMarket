//
//  main.c
//  LabCMarketServ
//
//  Created by Guilherme Guia and Jorge Sousa on 4/26/17.
//  Copyright © 2017 Guilherme Guia and Jorge Sousa. All rights reserved.
//

#include "LibServ.h"

int main(int argc, const char * argv[]) {
    if(DEBUG == 1){
        FILE* fp;
        fp = fopen(dirDebug,"w");
        if(fp == NULL){
            perror("Erro ao abrir o arquivo debug.txt (main)\n");
            exit(1);
        }
        fclose(fp);
    }
    //arrumar as structs pra funcionar
    Produto* noStock;
    noStock = createStock();
    noStock = loadStock(noStock,dirStock);
    
    //Load all users
    Users* users;
    users = createUsers();
    users = loadUsers(users);
    
    //Load statistics
    loadStatistics(users);
    
    createConn(users,noStock);
    return 0;
}
