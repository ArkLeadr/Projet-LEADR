#!/bin/bash

exe_fullpath=../../GenTexLEADR/build/GenTexLEADR

find . -type f -name '*.jpg' -exec  "$exe_fullpath" {} \;
find . -type f -name '*.png' -exec  "$exe_fullpath" {} \;

