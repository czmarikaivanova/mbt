#!/bin/bash

# write instance ID to objective function and CPU time log files
function logid {
	id=`head -n 1 $1`
	printf "$id\t"  >> ./logs/objlog.txt
	printf "$id\t"  >> ./logs/timelog.txt
}
function ceil {                                                                       
  echo "define ceil (x) {if (x<0) {return x/1} \
        else {if (scale(x)==0) {return x} \
        else {return x/1 + 1 }}} ; ceil($1)" | bc
}

#parameters:
# 1) AMPL model file name 
# 2) AMPL data file path
# 3) lower bound
# 4) upper bound
# 5) lower cutoff flag
# 6) integrality relaxation flag
function decProcedure {
	echo "
		print '-----------------xxx-------------Solving $1 relaxation=$6';
		reset; 
		model ./models/$1;			# location of the AMPL model
		param tlim default 3600;		# time limit for the solver
		for {i in $3..$4} {			# iterate from lower to upper bound
		        reset data;
			data ./$2;			# set AMPL data file
		        let tmax := i;
			#let spt[1] := 0;
			let tlim := tlim - _solve_time;
			option relax_integrality $6;	# set whether the integrality should be relaxed
			option cplex_options ('timelimit ' & tlim); 
			if '$1' = 'BT-int-com.mod' or '$1' = 'partition-bin-com.mod' or '$1' = 'BT-int-com-sparser.mod' then {
				option cplex_options \$cplex_options 'mipdisplay 2 mipinterval 1 absmipgap 0.99999 lowercutoff $5'; 
			}
			else {
				option cplex_options \$cplex_options 'mipdisplay 2 mipinterval 1 absmipgap 0.99999'; 
			}
		        solve;
			display _ampl_elapsed_time, _ampl_time, _solve_elapsed_time, _solve_time, _total_solve_elapsed_time,  _total_solve_time;
			display solve_result;
			option show_stats 1; 
			
		        if solve_result = 'solved' then { 	# a solution is found
				# If the model that is solved maximizes the number of informed nodes.
				# Needs to be here, because if model with obj is solved 
				# but the obj value is not n-s, the solution is in fact infeasible!
				if '$1' = 'BT-int-com.mod' or '$1' = 'partition-bin-com.mod' or '$1' = 'BT-int-com-sparser.mod' or '$1' = 'Y-CG.mod' or '$1' = 'Y-CG-simple.mod'  then { 

				        printf 'Now checking whether the value is %d\n', n-s;																      			    printf 'Objective is %f.2\n',round(objval,2); 
					if round(objval,2) = n-s then {
						printf '%4.2f\t', _total_solve_elapsed_time  >> timelog.txt;
						printf '%2.0f\t', i >> objlog.txt;
		        		        printf 'Optimal value: %d\n',i;
						printf 'And it is\n';																       
		        		        break;
					}
					else {
						printf 'And it is NOT!\n';																       
					}
				}
				else {
				      printf '%4.2f\t', _total_solve_elapsed_time  >> timelog.txt;
				      printf '%2.2f\t', i >> objlog.txt;
		                      #printf 'Optimal value: %f',objval;
		                      printf 'Optimal value: %d\n',i;
		                      break;
				}
		        }
			if solve_result = 'infeasible' then {	# the instance has no feasible solution
				if i = $4-1 then {		# if we are in the last iteration before the upper bound, the solution is indeed the UP.
					printf 'Infeasible solution for iteration %d, but upper bound %d can now be used',i,$4;
					printf '%4.2f\t',  _total_solve_elapsed_time  >> timelog.txt;
					printf '%2.0f\t', $4 >> objlog.txt;
		                	break;
				}
			}
			# If we got here, it means that we exceeded the time limit without finding the solution
			printf 'Solve result status number: %d', solve_result_num;
			 if solve_result = 'limit' then {
				printf '%4.2f\t',  _total_solve_elapsed_time  >> timelog.txt;
				printf '%2.0f\t', i >> objlog.txt;
		                printf 'Time limit exceeded. Best value found (LB): %d\n',i;
		                break;
		        }

		        printf 'Infeasible for k = %d\n',i;
		}
	" >> cmds.run
}

#parameters:
# 1) AMPL model file name 
# 2) AMPL data file path
# 3) integrality relaxation flag

function optProcedure {
	echo "
		print '-----------------xxx-------------Solving $1 relaxation=$3';
		reset; 
		model ./models/$1;
		param tlim default 3600;
		        reset data;
			data ./$2;
			let tlim := tlim - _solve_time;
			option relax_integrality $3;
			option cplex_options ('timelimit ' & tlim); 
			option cplex_options \$cplex_options 'mipdisplay 2 mipinterval 1 absmipgap 0.99999'; 
		        solve;
			printf '%d\t',ceil(objval) >> ./logs/objlog.txt;
			printf '%4.2f\t', _total_solve_elapsed_time  >> ./logs/timelog.txt;
			display _ampl_elapsed_time, _ampl_time, _solve_elapsed_time, _solve_time, _total_solve_elapsed_time,  _total_solve_time;
			display solve_result;
			option show_stats 1; 
			
	" >> cmds.run
}

#parameters:
# 1) AMPL model file name 
# 2) lower cutoff flag

function makeAMPLRun {
	logid $1 							# log instance ID - 
	javac -cp ./javaprograms LowerBounds.java -d ./bin		# compile Java program for computing lower bounds
	java -cp ./javaprograms/bin LowerBounds $1 ./logs/objlog.txt	# run it
	lb=$?								# lower bound is the value of the last executed command
	#javac -cp . UpperBounds.java -d ./bin
	#java -cp ./bin UpperBounds $1
	#ub=$?
	touch cmds-ub.run				# create AMPL run file for calculating upper bounds using the AMPL model
	echo "
		option solver '../../ampl/cplex'; 						# location of the cplex solver
		option cplex_options 'timing 1';
		option eexit -10000;
	        model ./models/matchingX.mod;							# set AMPL model
		param iterCnt default 0;
		for {h in {4,3,2,1}} {								# for deadline 4, 3, 2, and 1 (upper bound is weaker with shorter deadline)
			printf '---------------------------------Solving MATCHING_X%d\n',h;
	                option relax_integrality 0;						# relaxation of integrality 0 - solve IP. 1 - solve IP relaxation
	                reset data;								
	                data ./$1;								# set data file from the input parameter
	                let k := h;
	                repeat while card(S) < n {						# in iterative matching cardinality of 'sources' is increasing
	                        let iterCnt := iterCnt + 1;
	                        solve;
	                        for {j in S} {							# add neighbours of the sources to the sources for the next iteration
	                                for {v in N[j]} {
	                                        if x[j,v,1] == 1 then let S := S union {v};
	                                }
	                        }
	                }
			printf '%d\t',iterCnt >> iterCnt.txt;					# the number of iterations is the upper bound			
			print  _total_solve_elapsed_time > ./logs/matchTime.txt;		# log elapsed time for computing the upper bound.
		}
		" > cmds-ub.run	
	../../ampl/ampl cmds-ub.run		# execute the AMPL run file
	IFS='	' read -r -a array < iterCnt.txt# store each upper bound (for different deadlines) in an array
	ub="${array[0]}" 	
	rm iterCnt.txt
	read matchTime < matchTime.txt
	rm matchTime.txt
	echo lower bound is $lb
	echo upper bound is $ub
	if [ "$lb" -eq "$ub" ]; then		# if the upper and lower bounds are the same, only log objective function value. Elapsed time is 0 as no further computation is needed
		echo -e  "${array[0]}\t${array[1]}\t${array[2]}\t${array[3]}" >> ./logs/objlog.txt
		echo "0" >> ./logs/timelog.txt
	else					# otherwise create main AMPL run file
		touch cmds.run
		echo "created cmds"
		echo "  			# set some options
			#option solver '/opt/ibm/ILOG/CPLEX_Studio1251/cplex/bin/x86-64_sles10_4.1/cplexamp';
			option solver '../../ampl/cplex';
			option cplex_options 'timing 1';
			option eexit -10000;
		" > cmds.run
		###########################
		# here we continue creating the main AMPL run file in the function decProcedure.
		# the paramters are: model ampl_data_file, lower_bound, upper_bound
		###########################
		decProcedure MBT-maxinformed.mod $1 $lb $ub $2 1 1 
		decProcedure MBT-maxinformed.mod $1 $lb $ub $2 0 0
		
		echo "
			#printf '%d\t%d\t%d\t%d',${array[0]},${array[1]},${array[2]},${array[3]};   # when we run UB-1 .. UB-4
			printf '%d\t%d\t%d\t%d\n',${array[0]},${array[1]},${array[2]},${array[3]} >> ./logs/objlog.txt;
			#printf '%d\t',${array[0]};   #when we run only UB-4
			#printf '%d\n',${array[0]} >> ./logs/objlog.txt;
#			printf '%4.1f\n', $matchTime >> ./logs/timelog.txt;
			printf '\n' >> ./logs/timelog.txt;
 
		" >> cmds.run	
		../../ampl/ampl cmds.run
		#ampl-bin cmds.run
	fi
#	rm cmds.run
}

##########################################
           HERE IT STARTS 
##########################################
case $1 in
	raw) # processing raw files
		sourceCnt=$2;		# number of sources is the second parameter
		shift 2			# shift two parameters, so that it is possible to iterate over them 
		for filename in "$@"; do #iterate over params (files with raw data)
			echo "Processing: $filename"
#			dataFileName=$(echo "$filename" | sed -r "s/.+\/(.+)\..+/\1/")
			i=1;
		       while IFS='' read -r line || [[ -n "$line" ]]; do
				if [ "$i" -eq 1 ]; then 			# first line of a file contains node and edge count
					nodeCnt=$(echo $line | awk '{print $1}')
					edgeCnt=$(echo $line | awk '{print $2}')
					printf "$nodeCnt \t $edgeCnt \t" >> ./logs/objlog.txt 			# write the number of nodes and edges to the objective function value log file
					printf "$nodeCnt \t $edgeCnt \t" >> ./logs/timelog.txt 			# write the number of nodes and edges to the CPU time log file
					myFrac=$(echo "$nodeCnt/$sourceCnt" | bc -l);
					myLog=$(echo $myFrac | awk '{printf "%11.9f\n",log($1)/log(2)}')	# calculte the trivial logaritmic lower bound 
					paramK=`awk -v ml=$myLog 'BEGIN{printf("%.f\n", ml)}'`
					echo "$paramK"
					fileCnt=`ls ampldata/ | wc -w`						# get the number of files in the folder ampldata
					dataFileName="ampldata/inst-$nodeCnt-$sourceCnt-$fileCnt.dat"   	# AMPL file is stored in folder ampldata/ and its name indicates  
					touch "$dataFileName"							# the number of nodes and sources. The filecnt var makes the name unique
					echo -n > "$dataFileName"
					echo "# $RANDOM" >> $dataFileName	
					#echo "param k := $paramK;" >> "$dataFileName"
					echo "param n := $nodeCnt;" >> "$dataFileName"				# write parameters to the AMPL file
					echo "param s := $sourceCnt;" >> "$dataFileName"			#  --//--
					echo "set E :=" >> "$dataFileName"					# initial line for the set of edges in the AMPL file
				else 						# other than the first line 
					u=$(echo $line | awk '{print $1}')	# first number is the node u 
					v=$(echo $line | awk '{print $2}')	# second number - v
					echo "($u,$v)" >> "$dataFileName"	# write as '(u,v)' to the AMPL file
				fi
				i=$((i+1))
			done < "$filename" 
			echo ";" >> "$dataFileName"				# write semicolon at the end
			makeAMPLRun "$dataFileName" "$lco"			# call the procedure that creates AMPL run file
		done
	;;
	ampl) # processing AMPL files
		shift 1
		for filename in "$@"; do #iterate over params except the last (number of sources)
			echo "Processing: $filename"
			makeAMPLRun "$filename"			# call the procedure that creates AMPL run file
		done
	;;
	rand) # run experiments with randomly generated graphs
       		javac -cp ./javaprograms RandomGraph.java -d ./bin		# compile Java code for generating a random graph
		java -cp ./javaprograms/bin RandomGraph $2 $3 $4 $5 $6		# run Java class that generates a random graph with iven parameters  
	        files=$6* 
		for filename in $(ls $6);
       		 do
			echo "Processing: $filename"
			#echo "" >> objlog.txt	
			lco=`echo $3'-'$4'-0.0000001' | bc -l` #set lower cutoff for cplex
			makeAMPLRun  "$6$filename" "$lco"
			filecnt=`ls ampldata/random/ | wc -w`
			mv $6$filename "ampldata/experiments/"
		 done
;;
esac

