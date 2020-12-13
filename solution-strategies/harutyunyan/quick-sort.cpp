#include <iostream>
#include <vector>
#include "quick-sort.hpp"

void QuickSort::swap(std::vector<int> *vp, int a, int b)
{
    int temp = (*vp)[a];
    (*vp)[a] = (*vp)[b];
    (*vp)[b] = temp;
}

int QuickSort::partition(std::vector<int> *vp, int low, int high, std::vector<Vertex>* vertices)
{
    int pivot = (*vertices)[(*vp)[high]].nbChildren; //pivot - this approach uses the last element
    int i = low - 1;         //Index of smaller element

    for (int j = low; j <= high - 1; ++j)
    {
        //If the current element is smaller than pivot
        if ((*vertices)[(*vp)[j]].nbChildren > pivot)
        {
            ++i;
            swap(vp, i, j);
        }
    }
    swap(vp, i + 1, high);
    return i + 1;
}

void QuickSort::quickSort(std::vector<int> *vp, int low, int high, std::vector<Vertex>* vertices)
{
    if (low < high)
    {
        //Partion places vec[piv], where piv is the pivot index,
        //in it's correct place.
        int piv = partition(vp, low, high, vertices);

        //Separately sort elements before and after partion
        quickSort(vp, low, piv - 1, vertices);
        quickSort(vp, piv + 1, high, vertices);
    }
}