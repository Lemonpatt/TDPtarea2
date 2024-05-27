#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <chrono>
#include <set>
#include <algorithm>




using Graph = std::vector<std::set<int>>;

// Reads the graph from a file
Graph readGraph(const std::string &filename) {
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
    }

    return graph;
}

// Prints the graph
void printGraph(const Graph &graph) {
    for (int i = 0; i < graph.size(); ++i) {
        std::cout << i << ": ";
        for (int j : graph[i]) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

// Helper function to check if a set contains a value
bool contains(const std::set<int> &s, int value) {
    return s.find(value) != s.end();
}

// Helper function to find the maximum clique using Bron-Kerbosch with pivot and greedy coloring
void bronKerboschPivot(const Graph &graph, std::set<int> &R, std::vector<std::pair<int, int>> &P, std::set<int> &X, std::vector<int> &maxClique) {
    if (P.empty()) {
        if (R.size() > maxClique.size()) {
            
            maxClique.assign(R.begin(), R.end());
        }
        return;
    }

    // Calculate the upper bound for the clique size in this branch
    int upperBound = R.size() + P.size();

    // If the upper bound is less than or equal to the size of the maximum clique, prune the branch
    if (upperBound <= maxClique.size()) {
        return;
    }



    int pivot = P.begin()->first; // Start with the first element of P

    std::vector<std::pair<int, int>> P_minus_neighbors;


    if (graph.size() * 0.8 < P.size()) {
        for (const auto& nodo : P) {

            if (graph[pivot].size() == nodo.second && !contains(graph[pivot], nodo.first)) {
                P_minus_neighbors.push_back(nodo);
            }
        }
    }
    else if (graph.size() * 0.2 < P.size()) {
        for (const auto& nodo : P) {
            if (P_minus_neighbors.size() == 2){
                break;
            }
            if (graph[pivot].size() -1 <= nodo.second) {
                P_minus_neighbors.push_back(nodo);
            }
        }
    } else {
        for (const auto& nodo : P) {

            if (graph[pivot].size() == nodo.second && !contains(graph[pivot], nodo.first)) {
                P_minus_neighbors.push_back(nodo);
            }
        }
    }

    for (const auto &nodo : P_minus_neighbors) {
        std::set<int> R_new = R;
        R_new.insert(nodo.first);

        std::vector<std::pair<int, int>> P_new;
        std::set<int> X_new;
        for (int vecino : graph[nodo.first]) {
            if (std::find_if(P.begin(), P.end(), [&vecino](const std::pair<int, int> &p) { return p.first == vecino; }) != P.end()) {
                P_new.emplace_back(vecino, graph[vecino].size());
            }
            if (X.count(vecino)) {
                X_new.insert(vecino);
            }
        }

        bronKerboschPivot(graph, R_new, P_new, X_new, maxClique);

        P.erase(std::remove_if(P.begin(), P.end(), [&nodo](const std::pair<int, int> &p) { return p.first == nodo.first; }), P.end());
        X.insert(nodo.first);
    }
}

// Main function to find the maximum clique in a graph
std::vector<int> findMaximumClique(const Graph &graph) {
    int n = graph.size();
    std::vector<int> maxClique;



    // Initialize sets
    std::vector<std::pair<int, int>> P;
    std::set<int> R, X;

    for (int i = 0; i < n; ++i) {
        P.emplace_back(i, graph[i].size());
    }

    auto compareDegrees = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second < b.second; // Sort in ascending order of degrees
    };

    bronKerboschPivot(graph, R, P, X, maxClique);
    return maxClique;
}


int main() {

    std::string filename = "graph_33_0.979994_4.txt";


    Graph graph = readGraph(filename);
    printGraph(graph);


    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> maxClique = findMaximumClique(graph);
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
