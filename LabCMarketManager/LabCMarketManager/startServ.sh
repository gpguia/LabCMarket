#!/bin/bash
#	startServ.sh
#	LabCMarket
#
#	Created by Guilherme Paiva Guia on 5/7/17
# Copyright © 2017 Guilherme Guia. All rights reserved.
#
#

#Check if the program is running
if pgrep -x "LabCServ" > /dev/null
then
#If the program is running, kill it,then compile it again, and execute it, using screen so it will work in a single terminal.
		ps -ef | grep "LabCServ" | grep -v grep | awk '{print $2}' | xargs kill
		cd ~/LabCMarket/LabCMarketServ/LabCMarketServ && gcc main.c LibServ.c -o LabCServ -pthread && chmod +x LabCServ
		screen -d -m ./LabCServ
else
#If the program is not running, just compile it again and run it using screen.
    cd ~/LabCMarket/LabCMarketServ/LabCMarketServ && gcc main.c LibServ.c -o LabCServ -pthread && chmod +x LabCServ
		screen -d -m ./LabCServ
fi
