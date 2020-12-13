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

vector<vector<int>> createEdges(int n) {
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
    //End of edges
    outputFile << 0 << "\n";

    //Default source
    outputFile << 0 << "\n";
    outputFile.close();
}
  
// Driver program to test above 
int main(int argc, char** argv) 
{ 
    int dim = stoi(argv[1]);
    string filename = "hypercube" + to_string(dim) + ".txt";
    int n = createNumberOfVertices(dim);
    vector<vector<int>> edges = createEdges(n);
    cout << "Number of edges: " << edges.size() << '\n';
    print(filename, n, edges);
    return 0; 
} 