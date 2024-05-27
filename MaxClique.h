
#include "Graph.h"
#include <numeric>

class MaxClique {
public:
    MaxClique( Graph* graph);
    vector<int> findMaximumClique();

    Graph *graph;
    vector<set<int>> adjacencyList;
    void bronKerboschPivot(set<int> *R, vector<pair<int, int>> *P,  set<int> *X, vector<int> &maxClique);
};




