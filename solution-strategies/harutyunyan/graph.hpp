#include <list>
#include <vector>
#include "vertex.hpp"

class Graph
{
    private:
    //Number of vertices
    int n;
    
    public:
    //Id of source (currently only one source)
    int source;
    std::vector<Vertex> vertices;
    //Matrix of vertices on the different levels of the level graph
    std::vector<std::vector<int>> *layer;
    //Matching to store the actual edges in the graph
    std::vector<int> *matching;
    //Adjacency vector of all edges in graph
    std::vector<int> *edges;
    //Matrix of direct children for vertices
    std::vector<int> *children;
    //Matrix of siblings were edges exist between verticies the same level
    std::vector<int> *siblings;

    Graph();
    void initialize(int n);
    void solve(int s);
    int get_source();
    void set_source(int s);
    std::vector<std::vector<int>> getLayerGraph();
    void add_edge(Vertex v, Vertex u);
    void createLayerGraph(int s);
    void getChildren();
    void performMatching(int l);
    int estimateBroadcast(int index);
    std::vector<int> removeChildren(int index);
    bool hasCommonChildren(std::vector<int> childOfA, std::vector<int> childOfB);
    void generateSiblings();
    int broadcast(int s);
};