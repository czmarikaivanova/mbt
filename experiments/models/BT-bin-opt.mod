# Set cardinalities:
param k; # Not used here, only in the straightforward model
param n; # Total number of satellites
param s; # Number of source satellites
param tmax default n - 1;
# Sets:
set V = 0 .. n - 1; # 0 is the ground antenna
set S = 0 .. s - 1;
set E within {(i,j) in V cross V: i<j};
#set E0 = {(0,i) in (V cross V): i in S};
#let E := E union E0;
set A={(i,j) in V cross V: (i,j) in E || (j,i) in E};
set N{i in V} within V = {j in V: (i,j) in E || (j,i) in E};

# Variables:
var x{(i,j) in A, t in 1..tmax} binary;
var c{i in 1..tmax} binary;

# Objective function:
minimize objval: sum{i in 1..tmax} c[i];

# Constraints:

#subject to sourceFirst {v in S}:
#	sum{i in N[v]} x[v,i,1] <= 1;

subject to allReceive {u in (V diff S)}:
	sum{t in 1 .. tmax, v in N[u]} x[v,u,t] = 1;

subject to oneAtATime {t in 1..tmax, u in V}:
	sum{v in N[u]} x[u,v,t] <= 1;

subject to inIfOut {u in (V diff S), t in 2..tmax}:
	sum{v in N[u]} x[u,v,t] <= sum{l in 1..t-1, w in N[u]} x[w,u,l];

subject to xcrel {u in V, t in 1..tmax}:
	sum{v in N[u]} x[u,v,t] <= c[t];

subject to noFirst{(u,v) in A: u not in S}:
	x[u,v,1] = 0;

subject to timing {t in 2..tmax}:
	c[t] <= c[t-1];
