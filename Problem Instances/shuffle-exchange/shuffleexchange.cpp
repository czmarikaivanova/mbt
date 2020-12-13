#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//#include iostream.h;

using namespace std; 

int rol(int val, int n) {
    int pow = log2(n);
    int left_rotated = ((val << 1) | (val >> pow-1));
    return left_rotated < n ? left_rotated : left_rotated - n;
}
  
// function to check whether the two numbers 
// differ at the least significant bit position only 
bool differOnlyAtLeastSignificantBit(unsigned int a, 
                       unsigned int b) 
{ 
    return ((a ^ b) == 1);
}

//function to create number of vertices
int createNumberOfVertices(int d) {
    return pow(2, d);
}

vector<vector<int>> createEdges(int n) {
    vector<vector<int>> edges;
    
    for (int i = 0; i < n; i++) 
    {
        vector<int> edge;
        edges.push_back(edge);
        //Exchange edges
        for (int j = i + 1; j < n; j++ ) 
        {
            if (differOnlyAtLeastSignificantBit(i, j))
            {
                edges[i].push_back(j);
            }
        }

        //Shuffle edges
        int leftShifted = rol(i, n);
        
        //Removing self-loops
        if (i != leftShifted)
        {
            //Checking if edge already exist
            bool exists = false;
            for (int j = 0; j < edges[i].size(); ++j)
            {
                if (leftShifted < i && edges[leftShifted][j] == i)
                {
                    exists = true;
                    break;
                }    
            }
            //Adding new edges
            if (!exists)
            {
                edges[i].push_back(leftShifted);
            }
        }
    }
    return edges;
}

void print(string filename, int n, vector<vector<int>> edges) 
{
    ofstream outputFile;
    outputFile.open (filename);
    int nbOfEdges = 0;

    for (int i = 0; i < edges.size(); i++)
    {
        nbOfEdges += edges[i].size();
    }

    //First line: number of vertices, number of edges, number of sources (default 1)
    outputFile << n << "\t" << nbOfEdges << "\t" << 1 << "\n"; 
    
    for (int i = 0; i < edges.size(); ++i)
    {
        for (int j = 0; j < edges[i].size(); ++j)
        {
            outputFile << i << "\t" << edges[i][j] << '\n';
        }
        
    }
    //Ending edges
    outputFile << 0 << "\n";

    //Default source
    outputFile << 0 << "\n";
    outputFile.close();

    cout << "Number of edges: " << nbOfEdges << '\n';
}
  
// Driver program to test above
int main(int argc, char** argv) 
{ 
    int dim = stoi(argv[1]);
    int n = createNumberOfVertices(dim);

    vector<vector<int>> edges = createEdges(n);

    string filename = "shuffle_exchange" + to_string(dim) + ".txt";
    print(filename, n, edges);
    return 0; 
} 