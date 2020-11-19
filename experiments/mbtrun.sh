#!/bin/bash

iflag=
mflag=

echo > filenames.dat;
while getopts imt: name
do
    case $name in
    i) 	  echo "param doILP := 1;" >> filenames.dat
	  iflag=1;;
    m)    echo "param doMATCHING := 1;" >> filenames.dat
	  mflag=1;;
    t)	  echo "param tlim := $OPTARG;" >> filenames.dat;;
    ?)    printf "Usage: %s: [-a] [-b value] args\n" $0
          exit 2;;
    esac
done
if [ -z "$iflag" ] && [ -z "$mflag" ]; then
	echo "param doILP := 1;" >> filenames.dat;
	echo "param doMATCHING := 1;" >> filenames.dat;
fi

shift $(($OPTIND - 1))

#printf "Remaining arguments are: %s\n" "$*"


PASSED=$1

if   [ -d "${PASSED}" ]; then 
	echo "set FILENAMES:= " >> filenames.dat;
	ls -Q ./$1/** >> filenames.dat;
	echo  ";" >> filenames.dat;
elif [ -f "${PASSED}" ]; then 
	echo "set FILENAMES:= \"${PASSED}\";" >> filenames.dat; 
else 
	echo "${PASSED} is not valid";
     	exit 1
fi


~/software/AMPL/ampl ampliter.run;
rm filenames.dat;
