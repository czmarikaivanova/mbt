#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//#include iostream.h;

using namespace std; 
  
// function to check if x is power of 2 
bool isPowerOfTwo(unsigned int x) 
{ 
    // First x in the below expression is 
    // for the case when x is 0 
    return x && (!(x & (x - 1))); 
} 
  
// function to check whether the two numbers 
// differ at one bit position only 
bool differAtOneBitPos(unsigned int a, 
                       unsigned int b) 
{ 
    return isPowerOfTwo(a ^ b); 
}

//function to create number of vertices
int createNumberOfVertices(int d) {
    return pow(2, d);
}

vector<vector<int>> createHCEdges(int n) {
    vector<vector<int>> edges;
    for (int i = 0; i < n; i++) 
    {
        for (int j = i + 1; j < n; j++ ) 
        {
            if (differAtOneBitPos(i, j))
            {
                vector<int> edge{i, j};
                edges.push_back(edge);
            }
        }
    }
    return edges;
}

vector<vector<int>> createCCCEdges(int n, int dim, vector<vector<int>> hcEdges) {
    vector<bool> visited(n * dim, false);
    int index = 0;
    vector<vector<int>> cccedges;
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < dim; j++)
        {
            if (j + 1 < dim)
            {
                vector<int> edge{i * dim + j, i * dim + j + 1};  
                cccedges.push_back(edge);    
            }
            else
            {
                vector<int> edge{i * dim + j, i * dim};
                cccedges.push_back(edge);     
            }

            if (!visited[i * dim + j])
            {
                int source = hcEdges[index][0];
                int target = hcEdges[index][1];
                if (source == i)
                {
                    if (!visited[target * dim + j])
                    {
                        vector<int> edge{source * dim + j, target * dim + j};
                        cccedges.push_back(edge); 
                    }
                    index += 1;
                    visited[i * dim + j] = true;
                    visited[target * dim + j] = true;
                }
            }
        }


    }
    return cccedges;
}

void print(string filename, int n, vector<vector<int>> edges) 
{
    ofstream outputFile;
    outputFile.open (filename);
    //First line: number of vertices, number of edges, number of sources (default 1)
    outputFile << n << "\t" << edges.size() << "\t" << 1 << "\n"; 
    
    vector<vector<int>>::iterator it;
    for (it = edges.begin() ; it != edges.end(); ++it)
    {
        outputFile << (*it)[0] << "\t" << (*it)[1] << '\n';
    }
    //Default source
    outputFile << 0 << "\n";

    //Default bounds
    outputFile << ceil(log2(n)) << "\t" << n  << "\n";
    outputFile.close();
}
  
int main(int argc, char** argv) 
{ 
    int dim = stoi(argv[1]);

    string filename = "cubeconnectedcycles" + to_string(dim) + ".txt";

    //Creating the hypercube
    int n = createNumberOfVertices(dim);
    vector<vector<int>> hcedges = createHCEdges(n);

    //Creating new edges
     vector<vector<int>> cccedges = createCCCEdges(n, dim, hcedges);  
    //Mapping point indexes to int

    //Number of vertices in the cycled-connected cube
    n = n * dim;

    //Writing solution to file 
    cout << "Number of edges: " << cccedges.size() << '\n';
    print(filename, n, cccedges);
    return 0; 
} 