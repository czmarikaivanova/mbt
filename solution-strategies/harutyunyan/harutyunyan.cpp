#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "graph.hpp"

//Output - layered graph
int createLayerGraph(/*a graph*/)
{
    //Step 1 - BFS - mark all unused edges inactive
    //Step 2 - Assume that G_L has k layers, label all vertices 
    return 0;
}

//Output - spanning tree
int match(std::vector<int> vCurrentLayer, std::vector<int> vNextLayer) {
    //Step 1 - Order the parents by decreasing number of children
    //         and denote the parents by p1, p2, ...pn
    //Step 2 - Start with an empty matching
    //Step 3 - Add p1 and all its children to the matching
    //Step 4 - Match p1 with all its children
    //Step 5 - See article
     
     return 0;
}

//Output - a broadcast scheme in the spanning tree with rounds required
//         to broadcast the message throughout the tree.
int broadcast(/*a spanning tree, plus inactive edges between siblings*/) {
    return 0;
}

int find_index(std::string line)
{
    auto index = line.find("\t");
    if (index == std::string::npos)
    {
       index = line.find(" ");
    }

    return index;
}

//Arg 1 - path to input graph file
int main (int argc,  char** argv)
//int main() 
{
    Graph graph;
    int source;
    std::string line;
    std::ifstream graphInstance (argv[1]);
    //std::ifstream graphInstance("C:/PhD/MBT/experiments/data/existing/V160E240/xisnt-160-240-02.txt");
    //std::ifstream graphInstance ("C:/PhD/MBT/Problem instances/shuffle-exchange/shuffle_exchange7.txt");
    //std::ifstream graphInstance ("C:/PhD/MBT/Problem instances/hypercube/hypercube3.txt");
    if (graphInstance.is_open())
    {
        //Parse to get number of vertices n, number of edges m and number of sources s
        getline(graphInstance, line); 

        auto index1 = find_index(line);
        auto index2 = line.find_last_of("\t");
        if (index2 == std::string::npos)
        {
            index2 = line.find_last_of(" ");
        }

        int n = stoi(line.substr(0, index1));
        int m = stoi(line.substr(index1 + 1, index2 - index1));
        int s = stoi(line.substr(index2 + 1, line.size() - index2));

        graph.initialize(n);
        
        //Parse to get edges
        int edgeNumber = 1;
        int source = -1;
        int target = -1;
        while (edgeNumber <= m)
        {
            getline(graphInstance, line);    
            index1 = find_index(line);
            if (index1 != std::string::npos)
            {
                source = stoi(line.substr(0, index1));
                target = stoi(line.substr(index1 + 1, line.size() - index1));
                graph.add_edge(source, target);
            }
            edgeNumber +=1;
        }
        
        //Skip 0 after edges
        getline(graphInstance, line);

        //Parse to get source
        getline(graphInstance, line);
        index1 = find_index(line);
        if (index1 != std::string::npos)
        {
            source = stoi(line.substr(0, index1));
        }
        else
        {
            source = stoi(line.substr(0, line.size()));
        }
        
        graphInstance.close();
        std::cout << source << std::endl;
        graph.solve(source);
    }
    else 
    {
        std::cout << "Unable to open file";
    }

    // Step 1 - Create layer graph using breath first - check
    // Step 2 - Number all vertices of layer k - 1 with EB(v_i)
    // Step 3 - For each layer (from k - 2 to 1) call Matching
    // Step 4 - Perform procedure Broadcast and Optimize on the
    //          resulting spanning tree
    // Step 5 - The broadcast time of o in G is the number of
    //          rounds returned by procedure B and O
}



