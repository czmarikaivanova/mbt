#ifndef Vertex_HPP
#define Vertex_HPP

class Vertex
{
    public:
    int id;
    int nbChildren;
    int estBroadcast;
    bool visited;
    bool matched;
    bool broadcasted;
    bool informed;
    int childIndex;
    int siblingIndex;

    Vertex(int id);
};

#endif