
#include "vertex.hpp"

Vertex::Vertex(int vertexId)
{
    id = vertexId;
    nbChildren = 0;
    estBroadcast = 0;
    visited = false;
    matched = false;
    broadcasted = false;
    informed = false;
    childIndex = 0;
    siblingIndex = 0;
}