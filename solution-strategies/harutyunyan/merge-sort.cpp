#include <iostream>
#include <vector>
#include "merge-sort.hpp"

void MergeSort::merge(std::vector<int> *vp, int l, int m, int r, std::vector<Vertex> *vertices)
{
    int i, j, k;
    int n1 = m - l + 1; //n1 and n2 sizes of the
    int n2 = r - m;     //subarrays

    //Create temp arrays
    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (i = 0; i < n1; ++i)
    {
        L[i] = (*vp)[l + i];
    }

    for (j = 0; j < n2; ++j)
    {
        R[j] = (*vp)[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if ((*vertices)[L[i]].nbChildren >= (*vertices)[R[j]].nbChildren)
        {
            (*vp)[k] = L[i];
            i++;
        }
        else
        {
            (*vp)[k] = R[j];
            j++;
        }
        k++;
    }
    //Copy remaining elements of L[]
    while (i < n1)
    {
        (*vp)[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        (*vp)[k] = R[j];
        j++;
        k++;
    }
}

void MergeSort::mergeSort(std::vector<int> *vp, int l, int r,std::vector<Vertex>* vertices)
{
    if (l < r)
    {
        //Same as (l+r)/2, but avoids overflow for
        //large l and r
        int m = l + (r - l) / 2;

        mergeSort(vp, l, m, vertices);
        mergeSort(vp, m + 1, r, vertices);

        merge(vp, l, m, r, vertices);
    }
}
