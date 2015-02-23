#!/bin/bash

exe_fullpath=../../IrradianceMap/build/IrradianceMap.exe

find . -type f -name '*.float' -exec  "$exe_fullpath" {} \;


