# Parameters:
param cardV; 			# Total number of satellites
param cardS; 			# Number of source satellites
param cardE;			# Number of edges

param tmax default cardV - 1;	# Upper bound on broadcast time

# Sets:
set V = 0 .. cardV - 1;						# Set of all nodes
set S = 0 .. cardS - 1;						# Set of sources
set E within {(i,j) in V cross V: i<j}; 			# Set of edges (communication links) 
set A={(i,j) in V cross V: (i,j) in E || (j,i) in E}; 		# set of arcs (directed communication links)
set N{i in V} within V = {j in V: (i,j) in E || (j,i) in E}; 	# N[i] is a set of all neighbors of i

# Variables:
var x{(i,j) in A, t in 1..tmax} binary;		# x[i,j,t] = 1 iff a signal is sent via the arc (i,j) in time t			
var c{i in 1..tmax} binary;			# c[i] = 1 iff a signal is transmitted as late as in time t

# Objective function:
minimize objval: sum{i in 1..tmax} c[i];	# Minimize broadcast time					

# Constraints:

# Sources broadcast in time 1 - not necessary 
#subject to sourceFirst {v in S}:		
#	sum{i in N[v]} x[v,i,1] <= 1;

# All nodes except the sources receive the signal at some time step
subject to allReceive {u in (V diff S)}:
	sum{t in 1 .. tmax, v in N[u]} x[v,u,t] = 1;

# Each node can send a signal to at most one neighbor each time step
subject to oneAtATime {t in 1..tmax, u in V}:	
	sum{v in N[u]} x[u,v,t] <= 1;
#	sum{v in N[u]} x[u,v,t] <= c[t];

# A node transmits in time t only if it received earlier 
subject to inIfOut {u in (V diff S), t in 2..tmax}:					
	sum{v in N[u]} x[u,v,t] <= sum{l in 1..t-1, w in N[u]} x[w,u,l];

# Non-source does not transmit in time 1
subject to noFirst{(u,v) in A: u not in S}:						
	x[u,v,1] = 0;

# Relation between variables x and c - the same as oneAtATime if it uses c[t]
subject to xcrel {u in V, t in 1..tmax}:			
	sum{v in N[u]} x[u,v,t] <= c[t];

# If transmissin takes place in time t, it also takes place in time t-1
subject to timing {t in 2..tmax}:							
	c[t] <= c[t-1];
