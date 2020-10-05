# Parameters:
param k; 			# Not used here, only in the straightforward model
param n; 			# Total number of satellites
param s; 			# Number of source satellites

param tmax default n - 1;	# Upper bound on broadcast time

# Sets:
set V = 0 .. n - 1; 						# Set of all nodes
set S = 0 .. s - 1; 						# Set of sources
set E within {(i,j) in V cross V: i<j}; 			# Set of edges (communication links) 
set A={(i,j) in V cross V: (i,j) in E || (j,i) in E}; 		# set of arcs (directed communication links)
set N{i in V} within V = {j in V: (i,j) in E || (j,i) in E}; 	# N[i] is a set of all neighbors of i

param spg {S, V};
param spt {1..2}; # not needed here, but in runfile we set param spt

# Variables:
var x{(i,j) in A, t in 1..tmax} binary;		# x[i,j,t] = 1 iff a signal is sent via the arc (i,j) in time t			

#set SS = {(u,v) in E: u in S and v in S};      # Probably can be deleted

# Objective function:
maximize objval: sum{v in V, t in 1..tmax, u in N[v]: v not in S} x[u,v,t];	# Maximize the number of informed nodes within a given deadline tmax

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

# A node transmits in time t only if it received earlier 
subject to inIfOut {u in (V diff S), t in 2..tmax}:
	sum{v in N[u]} x[u,v,t] <= sum{l in 1..t-1, w in N[u]} x[w,u,l];

# Non-source does not transmit in time 1
subject to noFirst{(u,v) in A: u not in S}:
	x[u,v,1] = 0;

# Sources do not receive any signal - probably not necessary, optimization takes care of it
subject to sourceNoReceive {u in S}:
	sum{t in 1 .. tmax, v in N[u]} x[v,u,t] = 0;


#subject to inIfOutSparser {u in (V diff S), t in 2..tmax}:
#	sum{v in N[u]} (x[u,v,t] - x[v,u,t-1] - x[u,v,t-1]) <= 0;


