//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/28/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"


int main(int argc, const char * argv[]) {
    int sock;
    
    sock = connToServer();
    validManager(sock);
    return 0;
}
