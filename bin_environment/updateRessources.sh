#!/bin/bash

#-N : only if file is newer
wget -N http://149.91.81.176/ServerPackage/Ressources_ultra_lzma.zip

7z x -aoa Ressources_ultra_lzma.zip

cp -r ./Ressources/* .
rm -rf ./Ressources
