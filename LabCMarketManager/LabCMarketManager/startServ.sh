#!/bin/bash

#pid= pidof "LabCServ"


if pgrep -x "LabCServ" > /dev/null
then
		ps -ef | grep "LabCServ" | grep -v grep | awk '{print $2}' | xargs kill
		cd /Users/guilhermeguia/GoogleDrive/School/UP/2Semestre/LabDeComp/Project/LabCMarket/LabCMarketServ/LabCMarketServ/ && gcc main.c LibServ.c -o LabCServ && chmod +x LabCServ
		open -a Terminal "/Users/guilhermeguia/GoogleDrive/School/UP/2Semestre/LabDeComp/Project/LabCMarket/LabCMarketServ/LabCMarketServ/./LabCServ"
else
    cd /Users/guilhermeguia/GoogleDrive/School/UP/2Semestre/LabDeComp/Project/LabCMarket/LabCMarketServ/LabCMarketServ/ && gcc main.c LibServ.c -o LabCServ && chmod +x LabCServ
		open -a Terminal "/Users/guilhermeguia/GoogleDrive/School/UP/2Semestre/LabDeComp/Project/LabCMarket/LabCMarketServ/LabCMarketServ/./LabCServ"
fi
