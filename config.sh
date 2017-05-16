#!/bin/bash

##
##  start.sh
##  LabCMarket
##
##  Created by Guilherme Guia on 5/16/17.
##  Copyright © 2017 Guilherme Guia. All rights reserved.
##

caminho=$(pwd)
path2lib="$caminho/LabCMarketServ/LabCMarketServ/LibServ.h"

sed -i -e 's|pathtostock|"'"$caminho"'/LabCMarketServ/LabCMarketServ/stock.txt"|g' "$path2lib"
sed -i -e 's|pathtousers|"'"$caminho"'/LabCMarketServ/LabCMarketServ/users.txt"|g' "$path2lib"
sed -i -e 's|pathtogestores|"'"$caminho"'/LabCMarketServ/LabCMarketServ/gestores.txt"|g' "$path2lib"
sed -i -e 's|pathtodebug|"'"$caminho"'/LabCMarketServ/LabCMarketServ/debug.txt"|g' "$path2lib"
sed -i -e 's|pathtostatistics|"'"$caminho"'/LabCMarketServ/LabCMarketServ/statistics.txt"|g' "$path2lib"