#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Graph {
public:
    vector<set<int>> graph;
    
    Graph(const string &filename);
    void printGraph();
    vector<set<int>> getGraph();
    bool contains(set<int> s, int value) ;
};
