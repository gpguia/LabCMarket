//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia and Jorge Sousa on 3/28/17.
//  Copyright © 2017 Guilherme Guia and Jorge Sousa. All rights reserved.
//

#include "LibManager.h"



int main(int argc, const char * argv[]) {
    int sock;

    sock = connToServer();
    validManager(sock);
    
    return 0;
}
