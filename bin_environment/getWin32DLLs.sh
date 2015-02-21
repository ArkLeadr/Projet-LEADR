#!/bin/bash

arg=`tr [:upper:] [:lower:] <<< $1`
echo "You asked for dlls of type : $arg"

dll_type=""


if [ "$arg" == "debug" ]
then
	dll_type="Debug"
	echo $dll_type
elif [ "$arg" == "release" ]
then
	dll_type="Release"
	echo $dll_type
else 
	echo Usage getWin32DLLs [debug/release]
	exit 1
fi

filename="$dll_type"_ultra_lzma.zip

#-N : only if file is newer
wget -N http://149.91.81.176/ServerPackage/DLLs/"$filename"

7z e -aoa "$filename"
