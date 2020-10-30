#!/bin/bash

PASSED=$1

if   [ -d "${PASSED}" ]; then 
	echo "set FILENAMES:= " > filenames.dat;
	ls -Q ./$1/** >> filenames.dat;
	echo  ";" >> filenames.dat;
elif [ -f "${PASSED}" ]; then 
	echo "set FILENAMES:= \"${PASSED}\";" > filenames.dat; 
else 
	echo "${PASSED} is not valid";
     	exit 1
fi

echo  "param tlim := "$2";" >> filenames.dat;
~/software/AMPL/ampl ampliter.run;
rm filenames.dat;
