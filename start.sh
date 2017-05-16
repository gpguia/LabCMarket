#!/bin/bash
dir=$(pwd)

gcc $dir/LabCMarketServ/LabCMarketServ/main.c $dir/LabCMarketServ/LabCMarketServ/LibServ.c -o $dir/LabCMarketServ/LabCMarketServ/LabCServ -pthread && chmod +x $dir/LabCMarketServ/LabCMarketServ/LabCServ
gcc $dir/LabCMarketManager/LabCMarketManager/main.c $dir/LabCMarketManager/LabCMarketManager/LibManager.c -o $dir/LabCMarketManager/LabCMarketManager/manager && chmod +x $dir/LabCMarketManager/LabCMarketManager/manager
gcc $dir/LabCMarketClient/LabCMarketClient/main.c $dir/LabCMarketClient/LabCMarketClient/LibClientFunctions.c -o $dir/LabCMarketClient/LabCMarketClient/client && chmod +x $dir/LabCMarketClient/LabCMarketClient/client

