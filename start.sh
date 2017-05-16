#!/bin/bash

##
##  start.sh
##  LabCMarket
##
##  Created by Guilherme Guia on 5/16/17.
##  Copyright © 2017 Guilherme Guia. All rights reserved.
##

dir=$(pwd)

gcc $dir/LabCMarketServ/LabCMarketServ/main.c $dir/LabCMarketServ/LabCMarketServ/LibServ.c -o $dir/LabCMarketServ/LabCMarketServ/LabCServ -pthread && chmod +x $dir/LabCMarketServ/LabCMarketServ/LabCServ
gcc $dir/LabCMarketManager/LabCMarketManager/main.c $dir/LabCMarketManager/LabCMarketManager/LibManager.c -o $dir/LabCMarketManager/LabCMarketManager/manager && chmod +x $dir/LabCMarketManager/LabCMarketManager/manager
gcc $dir/LabCMarketClient/LabCMarketClient/main.c $dir/LabCMarketClient/LabCMarketClient/LibClientFunctions.c -o $dir/LabCMarketClient/LabCMarketClient/client && chmod +x $dir/LabCMarketClient/LabCMarketClient/client

gnome-terminal -e $dir/LabCMarketServ/LabCMarketServ/./LabCServ
gnome-terminal -e $dir/LabCMarketManager/LabCMarketManager/./manager
gnome-terminal -e $dir/LabCMarketClient/LabCMarketClient/./client