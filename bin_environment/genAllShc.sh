#!/bin/bash

exe_fullpath=../../IrradianceMap/build/IrradianceMap

find . -type f -name '*.float' -exec  "$exe_fullpath" {} \;


