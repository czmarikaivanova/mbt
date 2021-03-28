from igraph import *
from math import log

# usage:
# python3 BFDBGenerator.py <degree> <instance_type> 
# instance_type is either butterfly or debruijn

def create_inst_file(my_graph, name, node_cnt):
    if n < 10:
        f = open(name + "0" + str(n) + ".txt", "w")
    else:
        f = open(name + str(n) + ".txt", "w")
    f.write(str(node_cnt) + " " + str(len(graph.get_edgelist())) + " 1\n")
    for e in graph.get_edgelist():
        f.write(str(e[0]) + " " + str(e[1]) + "\n")
    f.write("0\n")
    f.write(str(n) + " " + str(node_cnt-1) + "\n")
    f.close()


n = int(sys.argv[1])
numbers = 2**n

if sys.argv[2] == "butterfly":   # generate butterfly
    graph = Graph((n+1) * numbers)
    for i in range(n):
        for j in range(numbers):
            u = i * numbers +j
            v = (i+1) * numbers + j^(1<<(n-1-i))
            graph.add_edge(u, v)
            graph.add_edge(u, (i+1)*numbers + j)
            create_inst_file(graph, sys.argv[2], (n+1) * numbers)
else:   # generate de brujin
    graph = Graph(numbers)
    for i in range(numbers):
        u = i
        v1 = (i << 1) % numbers
        v2 = ((i << 1) + 1) % numbers
        graph.add_edge(min(u,v1), max(u,v1))
        graph.add_edge(min(u,v2), max(u,v2))
        #print(str(min(u,v1)) + " " + str(max(u,v1)))
        #print(str(min(u,v2)) + " " + str(max(u,v2)))
        create_inst_file(graph, sys.argv[2], numbers)


