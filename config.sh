#!/bin/bash

##
##  start.sh
##  LabCMarket
##
##  Created by Guilherme Guia on 5/16/17.
##  Copyright © 2017 Guilherme Guia. All rights reserved.
##

dir=$(pwd)

#sed -i 's|foo|bar|g' filename

sed -i -e 's|pathtostock|"'"$dir"'/LabCMarketServ/LabCMarketServ/stock.txt"|g' ''"$dir"'/LabCMarketServ/LabCMarketServ/﻿﻿LibServ.h'
sed -i -e 's|pathtousers|"'"$dir"'/LabCMarketServ/LabCMarketServ/users.txt"|g' ''"$dir"'/LabCMarketServ/LabCMarketServ/﻿﻿LibServ.h'
sed -i -e 's|pathtogestores|"'"$dir"'/LabCMarketServ/LabCMarketServ/gestores.txt"|g' ''"$dir"'/LabCMarketServ/LabCMarketServ/﻿﻿LibServ.h'
sed -i -e 's|pathtodebug|"'"$dir"'/LabCMarketServ/LabCMarketServ/debug.txt"|g' ''"$dir"'/LabCMarketServ/LabCMarketServ/﻿﻿LibServ.h'
sed -i -e 's|pathtostatistics|"'"$dir"'/LabCMarketServ/LabCMarketServ/statistics.txt"|g' ''"$dir"'/LabCMarketServ/LabCMarketServ/﻿LibServ.h'
