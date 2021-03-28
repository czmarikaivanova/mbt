#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "graph.hpp"
#include <sys/types.h>
#include <filesystem>
 
int find_index(std::string line)
{
    auto index = line.find("\t");
    if (index == std::string::npos)
    {
       index = line.find(" ");
    }

    return index;
}

Graph read_file (std::string fileName)
{
    Graph graph;
    int source;
    std::string line;
    std::ifstream graphInstance (fileName);
    
    if (graphInstance.is_open())
    {
        getline(graphInstance, line); 

        //Parse to get number of vertices n, number of edges m and number of sources s
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
        
        graph.set_source(source);
        graphInstance.close();
        std::cout << source << std::endl;
    }
    else 
    {
        std::cout << "Unable to open file";
    }
    return graph;
}


void read_directory(const std::string& name)
{
    std::ofstream result_file(name + "_harutyunyan_results.txt");
    for (auto& p: std::filesystem::directory_iterator(name))
    {
        Graph graph = read_file(p.path().string());
        if (graph.n > 0)
        {
            graph.solve(graph.get_source());
            result_file << p.path() << ", " << graph.estBroadCast << ", " << graph.actualBroadCast << "\n";
        }
    }
}


int main (int argc,  char** argv)
{
    read_directory(argv[1]);
}



