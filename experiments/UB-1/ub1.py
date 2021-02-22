import os
import glob
import time
import sys
from scipy.sparse import csr_matrix
from scipy.sparse import lil_matrix
from scipy.sparse.csgraph import maximum_bipartite_matching


# A class to store a graph edge
class Edge:
    def __init__(self, u, v):
        self.u = u
        self.v = v


# A class to represent a graph object
class Graph:
    # Constructor
    def __init__(self, edges, sources, non_sources):
        self.sources = sources
        self.non_sources = non_sources
        # allocate memory for the adjacency list
        self.adj = [[] for _ in range(len(sources) + len(non_sources))]

        # add edges to the undirected graph
        for current in edges:
            # allocate node in adjacency list from src to dest
            self.adj[current.u].append(current.v)


# read instance file and return a graph object
def read_data(my_file):
    edge_list = []
    source_list = []
    with open(my_file) as f:
        node_cnt, edge_cnt, source_cnt = [int(word) for word in f.readline().split()]
        non_source_list = list(range(node_cnt))
        for _ in range(edge_cnt):
            u, v = map(int, f.readline().split())
            edge_list.append(Edge(u, v))
        for _ in range(source_cnt):
            node_id = int(f.readline())
            source_list.append(node_id)
            non_source_list.remove(node_id)
        return Graph(edge_list, source_list, non_source_list)


# construct a csr matrix from current bipartite graph
def construct_matrix(graph):
    my_matrix = lil_matrix((len(graph.sources), len(graph.non_sources)))
    for s in graph.sources:
        for v in graph.adj[s]:
            if v in graph.non_sources:
                my_matrix[graph.sources.index(s), graph.non_sources.index(v)] = 1
    return my_matrix.tocsr()


# loop over files given an input path (possibly with wildcards)
for file in glob.glob(os.path.join(".", str(sys.argv[1]))):
    start_time = time.time()
    graph = read_data(file)
    bt = 0
    while len(graph.non_sources) > 0:
        bt = bt + 1
        matrix = construct_matrix(graph)
        matching = maximum_bipartite_matching(matrix, perm_type='column')
        to_remove = []
        for i in range(len(matching)):
            u = graph.sources[i]
            if matching[i] >= 0:
                v = graph.non_sources[matching[i]]
                graph.sources.append(v)
                to_remove.append(v) # non_sources in the matching become sources and stop being nonsources
        graph.non_sources = [x for x in graph.non_sources if x not in to_remove]

    print(file + "\tBT: " + str(bt) + "\tRuntime: " + str(round(time.time() - start_time, 2)) + "s")
