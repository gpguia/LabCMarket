//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 4/26/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibServer.h"

int main(int argc, const char * argv[]) {

    //arrumar as structs pra funcionar
    Produto* noStock;
    noStock = createStock();
    noStock = loadStock(noStock,dirStock);
    
    
    return 0;
}