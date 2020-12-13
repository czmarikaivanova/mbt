
#include <vector>
#include "Vertex.hpp"

class QuickSort
{
    private:
    void swap(std::vector<int> *vp, int a, int b);
    int partition(std::vector<int> *vp, int low, int high, std::vector<Vertex>* vertices );
    
    public:
    void quickSort(std::vector<int> *vp, int low, int high, std::vector<Vertex>* vertices);
};