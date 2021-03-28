#include <vector>
#include "vertex.hpp"

class MergeSort
{
    private:
    void merge(std::vector<int> *vp, int l, int m, int r, std::vector<Vertex> *vertices);

    public:
    void mergeSort(std::vector<int> *vp, int l, int r,std::vector<Vertex> *vertices);
};