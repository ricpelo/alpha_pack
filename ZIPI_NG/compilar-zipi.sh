#!/bin/sh

echo "Compilando ZIPI..."
rm -f elcirculo.ztm~
LANG=C awk -f zipi.awk elcirculo.ztm > Pistas.h
#cp -f Pistas.h ..
#cd ..
#ln -sf ZIPI_NG/Pistas.h .
#cd ..
#./compilar.sh

