#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include "graph.hpp"
#include "quick-sort.hpp"
#include "merge-sort.hpp"

Graph::Graph()
{
    n = 0;
}

void Graph::initialize(int nb_of_v)
{
    n = nb_of_v;
    for (int i = 0; i < n; i++)
    {
        vertices.push_back(Vertex(i));
    }
    layer = new std::vector<std::vector<int>>;
    edges = new std::vector<int>[n];
    children = new std::vector<int>[n];
    matching = new std::vector<int>[n];
    siblings = new std::vector<int>[n];
}

int Graph::get_source()
{
    return source;
}

void Graph::set_source(int s)
{
    source = s;
}

std::vector<std::vector<int>> Graph::getLayerGraph()
{
    return *layer;
}

void Graph::add_edge(Vertex v, Vertex u)
{
    edges[v.id].push_back(u.id);
    edges[u.id].push_back(v.id);
}

void Graph::solve(int s)
{
    this->createLayerGraph(s);

/*     std::cout << "Children:" << std::endl;
    for (int j = 0; j < n; j++)
    {
        std::cout << "[ " << j << " ] ";
        for (int k = 0; k < children[j].size(); k++)
        {
            std::cout << children[j][k] << " ";
        }
        std::cout << std::endl;
    } */
    for (int i = layer->size() - 1; i >= 0; i--)
    {
        this->performMatching(i);
    }

    /* std::cout << "Layer:" << std::endl;
    for (int j = 0; j < (*layer).size(); j++)
    {
        std::cout << "[ " << j << " ] ";
        for (int k = 0; k < (*layer)[j].size(); k++)
        {
            std::cout << (*layer)[j][k] << " ";
        }
        std::cout << std::endl;
    } */

    /* std::cout << "Matching:" << std::endl;
    for (int j = 0; j < n; j++)
    {
        std::cout << "[ " << j << " ] ";
        for (int k = 0; k < matching[j].size(); k++)
        {
            std::cout << matching[j][k] << " ";
        }
        std::cout << std::endl;
    } */

    std::cout << "Estimated broadcast time: " << vertices[s].estBroadcast << std::endl;
    this->estBroadCast = vertices[s].estBroadcast; 
    this->generateSiblings();
    int broadcastTime = this->broadcast(s);
    this->actualBroadCast = broadcastTime;
    std::cout << "Actual broadcast time " << broadcastTime << std::endl;



    /* std::cout << "Edges:" << std::endl;
    for (int j = 0; j < n; j++)
    
        std::cout << "[ " << j << " ] ";
        for (int k = 0; k < edges[j].size(); k++)
        {
            std::cout << edges[j][k] << " ";
        }
        std::cout << std::endl;
    }
 */
/*     std::cout << "Siblings:" << std::endl;
    for (int j = 0; j < n; j++)
    {
        std::cout << "[ " << j << " ] ";
        for (int k = 0; k < siblings[j].size(); k++)
        {
            std::cout << siblings[j][k] << " ";
        }
        std::cout << std::endl;
    } */
}

void Graph::createLayerGraph(int s)
{
    //Mark all the vertices as unvisited
    bool *visited = new bool[n]{false};

    //Create a queue for BFS
    std::list<int> queue;

    vertices[s].visited = true;
    queue.push_back(s);
    std::list<int> next_tier;
    std::vector<int>::iterator i;
    std::vector<int> level_tier;

    while (!queue.empty())
    {
        s = queue.front();
        level_tier.push_back(s);
        queue.pop_front();

        for (i = edges[s].begin(); i != edges[s].end(); ++i)
        {
            if (!vertices[(*i)].visited)
            {
                vertices[(*i)].visited = true;
                next_tier.push_back(*i);
            }
        }

        if (queue.empty())
        {
            queue = next_tier;
            next_tier = std::list<int>();
            layer->push_back(level_tier);
            level_tier = std::vector<int>();
        }
    }

    /* for (int j = 0; j < layer->size(); j++)
    {
        if (layer->size() > 0)
        {
            std::cout << j << " [ ";
            std::vector<int>::iterator k;
            for (k = (*layer)[j].begin(); k != (*layer)[j].end(); k++)
            {
                std::cout << vertices[(*k)].id << " ";
            }
            std::cout << "]\n";
        }
    } */

    getChildren();
}

//Get children of a vertex. An edge exists between the parent and child and the
//child is exactly one layer below the parent in the layer graph.
void Graph::getChildren()
{
    //Starting at the lowest parent layer
    for (int i = (*layer).size() - 2; i >= 0; --i)
    {
        //For each parent
        for (int j = 0; j < (*layer)[i].size(); ++j)
        {
            //Go through the edges
            for (int k = 0; k < edges[(*layer)[i][j]].size(); ++k)
            {
                //And for each vertex on the layer below
                for (int l = 0; l < (*layer)[i + 1].size(); ++l)
                {
                    //Compare ids and add to children if success
                    if (vertices[edges[(*layer)[i][j]][k]].id == vertices[(*layer)[i + 1][l]].id)
                    {
                        children[vertices[(*layer)[i][j]].id].push_back(vertices[(*layer)[i + 1][l]].id);
                        break;
                    }
                }
            }
        }
    }

    /* std::cout << "Children:" << std::endl;
    for (int j = 0; j < n; j++)
    {
        std::cout << "[ " << j << " ] ";
        for (int k = 0; k < children[j].size(); k++)
        {
            std::cout << children[j][k] << " ";
        }
        std::cout << std::endl;
    } */
}

void Graph::performMatching(int i)
{

    //std::cout << "Parent - level: " << i << std::endl;

    for (int j = 0; j < (*layer)[i].size(); j++)
    {
        vertices[(*layer)[i][j]].nbChildren = children[vertices[(*layer)[i][j]].id].size();
    }

    //Sorting the layer after children
    //QuickSort qs = QuickSort();
    //qs.quickSort(&((*layer)[i]), 0, (*layer)[i].size() - 1, &vertices);
    /*     std::cout << "Before sort \n";
    for (int k = 0; k < (*layer)[i].size(); k++)
    {
        std::cout << (*layer)[i][k] << " ";
    }
    std::cout << "\n"; */

    MergeSort ms = MergeSort();
    ms.mergeSort(&((*layer)[i]), 0, (*layer)[i].size() - 1, &vertices);

    /* std::cout << "After sort \n";
    for (int k = 0; k < (*layer)[i].size(); k++)
    {
        std::cout << (*layer)[i][k] << " ";
    }
    std::cout << "\n";

 */
    //Matching the children of the layer with its parents
    bool swapped = true;
    for (int j = 0; j < (*layer)[i].size(); j++)
    {
        int index = (*layer)[i][j];
        for (int k = 0; k < children[index].size(); k++)
        {
            //std::cout << children[index][k] << " " << children[index][k] << std::endl;
            if (vertices[children[index][k]].matched == false)
            {
                vertices[children[index][k]].matched = true;
                //std::cout << vertices[children[index][k]].id << " " << vertices[children[index][k]].matched << std::endl;
                matching[index].push_back(children[index][k]);
            }
        }

        vertices[index].estBroadcast = estimateBroadcast(index);

        //Reposition parents on layer according to their EB
        swapped = true;
        while (swapped == true)
        {
            //std::cout << "EBT index: " << vertices[(*layer)[i][j]].estBroadcast << std::endl;
            for (int k = j - 1; k >= 0; --k)
            {
                //std::cout << "EBT k: " << k << " " << vertices[(*layer)[i][k]].estBroadcast << std::endl;
                if (vertices[(*layer)[i][k + 1]].estBroadcast > vertices[(*layer)[i][k]].estBroadcast)
                {
                    int temp = (*layer)[i][k];
                    (*layer)[i][k] = (*layer)[i][k + 1];
                    (*layer)[i][k + 1] = temp;
                }
                else
                {
                    break;
                }
            }

            //Find max(i + EB)
            //std::cout << "max(i + EB): " << i << std::endl;
            int EB_max = -1;
            int max_index = -1;
            for (int k = 0; k <= j; ++k)
            {
                //std::cout << "Order " << (*layer)[i][k] << " " << vertices[(*layer)[i][k]].nbChildren << std::endl;
                if (k + vertices[(*layer)[i][k]].estBroadcast > EB_max)
                {
                    EB_max = k + vertices[(*layer)[i][k]].estBroadcast;
                    max_index = (*layer)[i][k];
                }
            }

            //std::cout << "max(i + EB): " << EB_max << " " << max_index << std::endl;
            //If current vertex is max - remove unused children
            if (max_index == (*layer)[i][j])
            {
                children[(*layer)[i][j]] = removeChildren((*layer)[i][j]);
                swapped = false;
            }
            //If current vertex and max has no common children - remove unused children

            else if (!hasCommonChildren(children[max_index], children[(*layer)[i][j]]))
            {
                children[(*layer)[i][j]] = removeChildren((*layer)[i][j]);
                swapped = false;
            }
            //If b_max == b_vertex - remove unused children
            else if (EB_max == vertices[(*layer)[i][j]].estBroadcast + j)
            {
                children[(*layer)[i][j]] = removeChildren((*layer)[i][j]);
                swapped = false;
            }
            //Else reassign child from max to vertex
            else
            {
                bool isMatched = false;
                for (int k = 0; k < matching[max_index].size(); ++k)
                {
                    std::vector<int> equalBroadcast = std::vector<int>();
                    equalBroadcast.push_back(matching[max_index][k]);

                    for (int l = 0; l < matching[max_index].size(); ++l)
                    {
                        if (l != k)
                        {
                            if (vertices[matching[max_index][k]].estBroadcast != vertices[matching[max_index][l]].estBroadcast)
                            {
                                break;
                            }

                            equalBroadcast.push_back(matching[max_index][l]);
                        }
                    }

                    // std::cout << "Equal broadcast: " << equalBroadcast.size() << std::endl;
                    if (equalBroadcast.size() > 1)
                    {
                        //     std::cout << "EQUAL BROADCAST > 1 for j: " << (*layer)[i][j] << std::endl;
                        for (int l = 0; l < equalBroadcast.size(); ++l)
                        {
                            //         std::cout << "Broadcast id: " << equalBroadcast[l] << std::endl;
                            for (int m = 0; m < children[(*layer)[i][j]].size(); ++m)
                            {
                                //If equal swap matching from parent to child
                                //             std::cout << "child id for m: " << m << " " << children[(*layer)[i][j]][m] << std::endl;
                                if (equalBroadcast[l] == children[(*layer)[i][j]][m])
                                {
                                    //                 std::cout << "Broadcast id == child id" << std::endl;
                                    std::vector<int> temp = matching[max_index];
                                    matching[max_index].clear();
                                    for (int s = 0; s < temp.size(); ++s)
                                    {
                                        if (temp[s] != equalBroadcast[l])
                                        {
                                            matching[max_index].push_back(temp[s]);
                                        }
                                    }
                                    matching[(*layer)[i][j]].push_back(equalBroadcast[l]);

                                    //Recalculate EB?
                                    std::cout << "Org broadcast: " << (*layer)[i][j] << " " << vertices[(*layer)[i][j]].estBroadcast << std::endl;
                                    vertices[(*layer)[i][j]].estBroadcast = estimateBroadcast((*layer)[i][j]);
                                    std::cout << "New broadcast: " << (*layer)[i][j] << " " << vertices[(*layer)[i][j]].estBroadcast << std::endl;
                                    std::cout << "Org broadcast: " << max_index << " " << vertices[max_index].estBroadcast << std::endl;
                                    vertices[max_index].estBroadcast = estimateBroadcast(max_index);
                                    std::cout << "New broadcast: " << max_index << " " << vertices[max_index].estBroadcast << std::endl;

                                    isMatched = true;
                                    break;
                                }
                            }
                            if (isMatched)
                            {
                                break;
                            }
                        }
                    }
                    if (isMatched)
                    {
                        break;
                    }
                }
                swapped = false;
            }
           // std::cout << index << " " << vertices[index].id << " Broadcast: " << vertices[index].estBroadcast << std::endl;
        }

        /* std::cout << "EBT Layer: [" << i << "] " << std::endl;
        for (int j = 0; j < (*layer)[i].size(); j++)
        {
            std::cout << (*layer)[i][j] << " ";
        }
        std::cout << std::endl;
        std::cout << "EBT: " << std::endl;
        for (int j = 0; j < (*layer)[i].size(); j++)
        {
            std::cout << vertices[(*layer)[i][j]].estBroadcast << " ";
        }
        std::cout << std::endl;
 */
        /*  std::cout << "Matching:" << std::endl;
        for (int j = 0; j < n; j++)
        {
            std::cout << "[ " << j << " ] ";
            for (int k = 0; k < matching[j].size(); k++)
            {
                std::cout << matching[j][k] << " ";
            }
            std::cout << std::endl;
        } */
    }
}

int Graph::estimateBroadcast(int index)
{
    int max = 0;
    int n = matching[index].size();
    for (int i = 0; i < n; ++i)
    {
        if (vertices[matching[index][i]].estBroadcast > max)
        {
            max = vertices[matching[index][i]].estBroadcast;
        }
    }

    std::vector<int> sum = std::vector<int>(n, 0);
    std::vector<int> min = std::vector<int>(n, -1);

    //std::cout << "ETB max:" << max << std::endl;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if ((max - j) >= vertices[matching[index][i]].estBroadcast && (max - j - 1) < vertices[matching[index][i]].estBroadcast)
            {
                sum[j] += 1;

                if (min[j] == -1 || vertices[matching[index][i]].estBroadcast < min[j])
                {
                    min[j] = vertices[matching[index][i]].estBroadcast;
                }
            }
        }
    }

    //std::cout << "Sum: ";
    for (int j = 1; j < n; ++j)
    {
        //std::cout << sum[j-1] << " ";
        sum[j] += sum[j - 1];
    }
    //std::cout << std::endl;

    int ebc_max = 0;
    for (int i = 0; i < sum.size(); ++i)
    {
        //std::cout << min[i] << " ";
        if (min[i] + sum[i] > ebc_max)
        {
            ebc_max = min[i] + sum[i];
        }
    }
    //std::cout << std::endl;

    return ebc_max;
}

std::vector<int> Graph::removeChildren(int index)
{
    std::vector<int> updatedChildren = std::vector<int>();
    for (int i = 0; i < children[index].size(); ++i)
    {
        bool inMatching = false;
        for (int j = 0; j < matching[index].size(); ++j)
        {
            if (matching[index][j] == children[index][i])
            {
                inMatching = true;
                break;
            }
        }

        if (inMatching)
        {
            updatedChildren.push_back(children[index][i]);
        }
    }
    return updatedChildren;
}

bool Graph::hasCommonChildren(std::vector<int> childOfA, std::vector<int> childOfB)
{
    return std::find_first_of(childOfA.begin(), childOfA.end(),
                              childOfB.begin(), childOfB.end()) != childOfA.end();
}

void Graph::generateSiblings()
{
    for (int i = 0; i < (*layer).size(); i++)
    {
        //std::cout << "Layer: " << i << std::endl;
        for (int j = 0; j < (*layer)[i].size(); j++)
        {
            //std::cout << "1st Vertex: " << (*layer)[i][j] << std::endl;
            for (int k = j + 1; k < (*layer)[i].size(); k++)
            {
                //std::cout << "2st Vertex: " << (*layer)[i][k] << std::endl;
                for (int l = 0; l < edges[(*layer)[i][j]].size(); l++)
                {
                    //std::cout << "Edge: " << edges[(*layer)[i][j]][l] << std::endl;
                    if ((*layer)[i][k] == edges[(*layer)[i][j]][l])
                    {
                        siblings[(*layer)[i][j]].push_back((*layer)[i][k]);
                        siblings[(*layer)[i][k]].push_back((*layer)[i][j]);
                        break;
                    }
                }
            }
        }
    }
}

int Graph::broadcast(int s)
{
    //Ordering matching
    for (int i = 0; i < vertices.size(); ++i)
    {
        for (int j = matching[i].size() - 1; j > 0; --j)
        {
            for (int k = j - 1; k >= 0; --k)
            {
                if (vertices[matching[i][j]].estBroadcast > vertices[matching[i][k]].estBroadcast)
                {
                    int temp = matching[i][j];
                    matching[i][j] = matching[i][k];
                    matching[i][k] = temp;
                }
            }
        }

        /* std::cout << "[" << i << "] ";
        for (int j = 0; j < matching[i].size(); ++j)
        {
            std::cout << vertices[matching[i][j]].estBroadcast
             << " ";
        }
        std::cout << "\n"; */
    }


    int broadcastTime = 0;
    std::vector<int> informedVertices;
    informedVertices.push_back(s);

    while (informedVertices.size() < vertices.size())
    {
        int nbOfCurrentlyInformed = informedVertices.size();
        for (int i = 0; i < nbOfCurrentlyInformed; i++)
        {
            Vertex current = vertices[informedVertices[i]];
            //Skips the vertex if it has no more children or siblings
            if (!current.broadcasted)
            {
                //If children inform next
                bool hasInformed = false;
                while(current.childIndex < matching[current.id].size() && vertices[matching[current.id][current.childIndex]].informed)
                {
                    current.childIndex += 1;
                }
                if (current.childIndex < matching[current.id].size())
                {
                    informedVertices.push_back(matching[current.id][current.childIndex]);
                    vertices[matching[current.id][current.childIndex]].informed = true;
                    vertices[informedVertices[i]].childIndex += 1;
                    hasInformed = true;   
                }
                //If no children, inform next sibling
                while(current.siblingIndex < siblings[current.id].size() && vertices[siblings[current.id][current.siblingIndex]].informed)
                {
                    current.siblingIndex += 1;
                }
                if (!hasInformed && current.siblingIndex < siblings[current.id].size())
                {
                    informedVertices.push_back(siblings[current.id][current.siblingIndex]);
                    vertices[siblings[current.id][current.siblingIndex]].informed = true;
                    vertices[informedVertices[i]].siblingIndex += 1;
                    hasInformed = true;
                }
                //If no children nor siblings set vertex as broadcasted
                if (!hasInformed)
                {
                    vertices[informedVertices[i]].broadcasted = true;
                }
            }
        }
        broadcastTime += 1;
    }
    
    return broadcastTime;
}