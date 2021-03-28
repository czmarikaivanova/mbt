#!/usr/bin/awk -f

/\yparam n\y/ {newfname=substr(FILENAME,5,length(FILENAME)); outfile="rnd"newfname; cardV=substr($NF,1,length($NF)-1)+0; printf "%d\t", cardV >> outfile; }
#/\yset E\y/   { printf "%d\t 0\t %d\n" ,NF-3,NF >> outfile; printf "%s\n",gsub(/[^0-9]/," ")  >> outfile; print edgestr;}

/\yset E\y/   { lb=log(cardV)/log(2)+1; ub=cardV-1; printf "%d\t 1\n" ,NF-3 >> outfile; for (i=4;i<=NF-1;i++) {stre=substr($i,2,length($i)-2); split(stre,a,","); printf "%d %d\n", a[1],a[2] >> outfile;} strel=substr($NF,2,length($NF)-3); split(strel,b,","); printf "%d %d\n0\n%d %d\n", b[1],b[2],lb,ub >> outfile; }
#/\EOF/     {lb=log(cardV)/log(2)+1; print lb; ub=cardV-1; printf "0\n%d\t%d\n", lb,ub >> outfile; }

