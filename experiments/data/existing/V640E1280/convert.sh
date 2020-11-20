#!/usr/bin/awk -f

/\yNodes\y/ { outfile="new"FILENAME; printf "%d\t",$2 >> outfile; cardV=$2; }
/\yEdges\y/ { printf "%d\t1\n", $2 >> outfile }
/\yE\y/     { print $2-1, "\t", $3-1  >> outfile }
/\yEOF\y/   {lb=log(cardV)/log(2); ub=cardV-1; printf "0\n%d\t%d\n", lb,ub >> outfile }

