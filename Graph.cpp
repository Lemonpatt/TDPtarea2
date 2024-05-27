#include "Graph.h"


Graph::Graph(const string &filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file." << endl;
        exit(1);
    }

    int n;
    infile >> n;

    graph.resize(n);
    int nodo, vecino;
    while (infile >> nodo >> vecino) {
        graph[nodo].insert(vecino); // Insert into set
    }
}

void Graph::printGraph() {
    for (int i = 0; i < graph.size(); ++i) {
        cout << i << ": ";
        for (int j : graph[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
}

 vector<set<int>> Graph::getGraph() {
    return graph;
}

bool Graph::contains(set<int> s, int value) {
    return s.find(value) != s.end();
}
