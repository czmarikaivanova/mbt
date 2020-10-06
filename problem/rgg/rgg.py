import graph_tool.all as gt
import numpy as np
import matplotlib
import sys

#input args and default params
ofile = "output.txt"      #output file
n = 200                 #number of vertices
rad = 0.4               #edges will be added to vertices within the radius of eachother
trial_max = 25          #number of attempts to create a connected graph
                        #if the graph isn't created increase n or rad

med = 0                 #median of the normal dist
std = 0.1               #standard deviation of the normal dist


ug = gt.Graph()

trials = 0
valid = False
while not valid:
    trials += 1
    print("Trial #: " + str(trials))

    #Create random points on the unit sphere
    points = []
    for x in range(n):
        s = np.random.normal(med, std, 3)
        dot = np.sqrt(np.dot(s, s))
        points.append(s/dot)

    #Create graph
    ug, pos = gt.graph_tool.generation.geometric_graph(points, rad)
    ug.set_directed(False)

    #Verifying that the graph is connected
    tree = gt.min_spanning_tree(ug)
    min_tree_edges = 0
    for t in tree:
        min_tree_edges += t 

    #Termination criteria - valid graph or > max_trials
    if min_tree_edges == len(ug.get_vertices())-1 or trials > trial_max :
        valid = True

if len(ug.get_vertices()) > 0:
    #Min and max degree vertices
    out_degs = ug.get_out_degrees(ug.get_vertices())
    max_value = np.amax(out_degs)
    max_indices = np.where(out_degs == np.amax(out_degs))
    min_value = np.amin(out_degs)
    min_indices = np.where(out_degs == np.amin(out_degs))

    #Write to file
    with open (ofile, "w") as filehandle:
        filehandle.write(str(len(ug.get_vertices())) + "\n")
        filehandle.write(str(len(ug.get_edges())) + "\n")
        filehandle.write("Max deg: " + str(max_value) + " " + str(max_indices[0])+ "\n")
        filehandle.write("Min deg: " + str(min_value) + " " + str(min_indices[0])+ "\n")
        for e in ug.edges():
            filehandle.write(str(e.source()) + " " + str(e.target()) + "\n")

    #Plot grap
    gt.graph_draw(ug, pos=pos, output="geometric.pdf")