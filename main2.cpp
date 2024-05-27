#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <chrono>
#include <algorithm>

// Define the Graph type
using Graph = std::vector<std::set<int>>;

// Define the BronKerboschPivot class
class BronKerboschPivot {
public:
    BronKerboschPivot(const std::vector<std::set<int>>& graph) : graph(graph), n(graph.size()) {}

    std::set<int> findMaximumClique() {
        std::set<int> R, P, X;
        for (int i = 0; i < n; ++i) {
            P.insert(i);
        }
        bronKerbosch(R, P, X);
        return maxClique;
    }

private:
    const std::vector<std::set<int>>& graph;
    int n;
    std::set<int> maxClique;
    std::vector<int> colors;

    void colorGraph() {
        colors.resize(n, -1);
        int currentColor = 0;
        std::set<int> availableColors;
        for (int i = 0; i < n; ++i) {
            availableColors.insert(i);
        }
        for (int i = 0; i < n; ++i) {
            if (colors[i] == -1) {
                std::set<int> neighbors;
                for (int neighbor : graph[i]) {
                    if (colors[neighbor] != -1) {
                        neighbors.insert(colors[neighbor]);
                    }
                }
                if (neighbors.empty()) {
                    colors[i] = currentColor++;
                } else {
                    auto it = availableColors.begin();
                    while (neighbors.find(*it) != neighbors.end()) {
                        ++it;
                    }
                    colors[i] = *it;
                }
                availableColors.erase(colors[i]);
            }
        }
    }

    void bronKerbosch(std::set<int>& R, std::set<int>& P, std::set<int>& X) {
        if (P.empty() && X.empty()) {
            if (R.size() > maxClique.size()) {
                maxClique = R;
            }
            return;
        }
        int upperBound = R.size() + P.size();

        // If the upper bound is less than or equal to the size of the maximum clique, prune the branch
        if (!maxClique.empty() && upperBound <= maxClique.size()) {
            return;
        }
        int u = choosePivot(P, X);
        std::set<int> PwithoutNeighborsOfU;
        for (int v : P) {
            if (graph[u].find(v) == graph[u].end()) {
                PwithoutNeighborsOfU.insert(v);
            }
        }

        for (int v : PwithoutNeighborsOfU) {
            R.insert(v);
            std::set<int> P_intersect, X_intersect;
            std::set_intersection(P.begin(), P.end(), graph[v].begin(), graph[v].end(), std::inserter(P_intersect, P_intersect.begin()));
            std::set_intersection(X.begin(), X.end(), graph[v].begin(), graph[v].end(), std::inserter(X_intersect, X_intersect.begin()));

            R.erase(v);
            P.erase(v);
            X.insert(v);
        }
    }

    int choosePivot(const std::set<int>& P, const std::set<int>& X) {
        std::set<int> unionPX(P.begin(), P.end());
        unionPX.insert(X.begin(), X.end());
        int maxNeighborCount = -1;
        int pivot = -1;
        for (int u : unionPX) {
            int neighborCount = std::count_if(P.begin(), P.end(), [&](int v) {
                return graph[u].find(v) != graph[u].end();
            });
            if (neighborCount > maxNeighborCount) {
                maxNeighborCount = neighborCount;
                pivot = u;
            }
        }
        return pivot;
    }
};


// Function to read the graph from file
Graph readGraph(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file." << std::endl;
        exit(1);
    }

    int n;
    infile >> n;

    Graph graph(n);
    int nodo, vecino;
    while (infile >> nodo >> vecino) {
        graph[nodo].insert(vecino); // Insert into set
        graph[vecino].insert(nodo); // If the graph is undirected
    }

    return graph;
}

int main() {
    std::string filename = "graph_7_0.800000_3.txt";

    Graph graph = readGraph(filename);

    BronKerboschPivot bk(graph);
    auto start = std::chrono::high_resolution_clock::now();
    std::set<int> maxClique = bk.findMaximumClique();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (maxClique.empty()){

        std::cout << "No hay solucion para este grafo" << std::endl;
    }

    else {

    
    std::cout << "Clique Maximo: ";
    for (int v : maxClique) {
        std::cout << v << " ";
    }
    std::cout << "Tamaño del Clique:" << maxClique.size() << "Nodos\n";
    std::cout << "\nTiempo de ejecucion: " << duration.count() << " segundos\n";
    }

    return 0;
}
