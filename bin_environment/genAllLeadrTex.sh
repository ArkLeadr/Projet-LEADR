#!/bin/bash

exe_fullpath=../../GenTexLEADR/GenTexLEADR/build/GenTexLEADR.exe

find . -type f -name '*.jpg' -exec  "$exe_fullpath" {} \;
find . -type f -name '*.png' -exec  "$exe_fullpath" {} \;

