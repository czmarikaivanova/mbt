#!/bin/bash
echo "set FILENAMES := " > filenames.dat;
ls -Q ./data/** >> filenames.dat;
echo ";" >> filenames.dat;
cat filenames.dat;
~/software/AMPL/ampl ampliter.run;
#rm filenames.dat;
