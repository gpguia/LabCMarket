//
//  main.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/28/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"


int main(int argc, const char * argv[]) {
    int sock,status;
    
    status = system(execServ);
    
    printf("%d\n",status);
    
    sleep(10);
    sock = connToServer();
    validManager(sock);
    return 0;
}
