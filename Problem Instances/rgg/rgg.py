import graph_tool.all as gt
import numpy as np
import math
import matplotlib
import sys

#input args and default params
ofile = "output.txt"      #output file
n = 400                 #number of vertices
rad = 0.25               #edges will be added to vertices within the radius of eachother

med = 0                 #median of the normal dist
std = 0.1               #standard deviation of the normal dist


ug = gt.Graph()

#Create random points on the unit sphere
points = []
for x in range(n):
    s = np.random.normal(med, std, 3)
    dot = np.sqrt(np.dot(s, s))
    points.append(s/dot)

#Create graph
ug, pos = gt.graph_tool.generation.geometric_graph(points, rad)
ug.set_directed(False)

visited = []
edges  = []
not_visited = ug.get_vertices()
visited.append(not_visited[0])
not_visited = np.delete(not_visited, 0)
edges.extend(ug.get_all_edges(visited[0]))

while len(visited) < len(ug.get_vertices()):
    current_edges = edges
    edges = []
    if len(current_edges) > 0:
        for e in current_edges:
            if len(e) > 0:
                if e[0] in not_visited:
                    edges.extend(ug.get_all_edges(e[0]))
                    visited.append(e[0])
                    not_visited = np.delete(not_visited, np.where(not_visited == e[0]))
                if e[1] in not_visited:
                    edges.extend(ug.get_all_edges(e[1]))
                    visited.append(e[1])
                    not_visited = np.delete(not_visited, np.where(not_visited == e[1]))
    if len(edges) == 0 and len(not_visited) > 0:
        ug.add_edge(visited[len(visited)-1], not_visited[0])
        edges.append([visited[len(visited)-1], not_visited[0]])

#Verifying that the graph is connected
tree = gt.min_spanning_tree(ug)
min_tree_edges = 0
for t in tree:
    min_tree_edges += t 

print("Edges #: " + str(len(ug.get_edges())))

if min_tree_edges == len(ug.get_vertices()) - 1:
    #Min and max degree vertices
    out_degs = ug.get_out_degrees(ug.get_vertices())
    max_value = np.amax(out_degs)
    max_indices = np.where(out_degs == np.amax(out_degs))
    min_value = np.amin(out_degs)
    min_indices = np.where(out_degs == np.amin(out_degs))

    #Write to file
    with open (ofile, "w") as filehandle:
        filehandle.write(str(len(ug.get_vertices())) + "\t" + str(len(ug.get_edges())) + "\t" + "1" + "\n")

        for e in ug.edges():
            filehandle.write(str(e.source()) + "\t" + str(e.target()) + "\n")

        filehandle.write("0" + "\n")
        filehandle.write(str(math.ceil(math.log(len(ug.get_vertices()), 2))) + "\t" + str(len(ug.get_vertices())) + "\n")
        filehandle.write("Max deg: " + str(max_value) + " " + str(max_indices[0])+ "\n")
        filehandle.write("Min deg: " + str(min_value) + " " + str(min_indices[0])+ "\n")

#Plot grap
#gt.graph_draw(ug, pos=pos, output="geometric.pdf")