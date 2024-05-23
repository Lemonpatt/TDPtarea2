#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <chrono>
#include <set>
// Graph represented as an adjacency list
using Graph = std::vector<std::vector<int>>;


class Nodo {
public:
    int valor;
    int Grado;

    Nodo(int valor, int Grado) : valor(valor), Grado(Grado) {}
};


// Function to read the graph from a file
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
        graph[nodo].push_back(vecino);
    }

    return graph;
}


// Function to print the graph
void printGraph(const Graph &graph) {
    for (int i = 0; i < graph.size(); ++i) {
        std::cout << i << ": ";
        for (int j = 0; j < graph[i].size(); ++j) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Utility function to color the graph using greedy coloring
std::vector<int> greedyColoring(const Graph &graph) {
    int n = graph.size();
    std::vector<int> color(n, -1);
    std::vector<bool> available(n, true);

    for (int u = 0; u < n; ++u) {
        for (int v : graph[u]) {
            if (color[v] != -1) {
                available[color[v]] = false;
            }
        }

        int cr;
        for (cr = 0; cr < n; ++cr) {
            if (available[cr]) {
                break;
            }
        }

        color[u] = cr;

        for (int v : graph[u]) {
            if (color[v] != -1) {
                available[color[v]] = true;
            }
        }
    }

    return color;
}

// Manual find function to replace std::find
bool contains(const std::vector<int> &vec, int value) {
    for (int element : vec) {
        if (element == value) {
            return true;
        }
    }
    return false;
}

// Helper function to find the maximum clique using Bron-Kerbosch with pivot and greedy coloring
void bronKerboschPivot(const Graph &graph, std::set<int> &R, std::set<int> &P, std::set<int> &X, std::vector<int> &maxClique) {
    if (P.empty() && X.empty()) {
        if (R.size() > maxClique.size()) {
            maxClique.assign(R.begin(), R.end());
        }
        return;
    }
    // Calcular la cota superior para el tamaño del clique en esta rama
    int upperBound = R.size() + P.size();

    // Si la cota superior es menor o igual al tamaño del clique máximo, podar la rama
    if (!maxClique.empty() && upperBound <= maxClique.size()) {
        return;
    }

    // Continuar con el algoritmo normal si P no está vacío
    int pivot = *P.begin();
    for (int v : P){
        if (graph[v].size() > graph[pivot].size()){
            pivot = v;
        }
    }
        
    std::set<int> P_minus_neighbors;



    if(graph.size()*0.6 < P.size()){
        for (int nodo : P) {
            if (!contains(graph[pivot], nodo) || graph[pivot].size()-1 <= graph[nodo].size()) {
                P_minus_neighbors.insert(nodo);
            }
        }
    }
    else {
        for (int nodo : P) {
            if (!contains(graph[pivot], nodo) || graph[pivot].size() <= graph[nodo].size()) {
                P_minus_neighbors.insert(nodo);
            }
        }
    }
    

    for (int nodo : P_minus_neighbors) {
        std::set<int> R_new = R;
        R_new.insert(nodo);

        std::set<int> P_new, X_new;
        for (int vecino : graph[nodo]) {
            if (P.count(vecino)) {
                P_new.insert(vecino);
            }
            if (X.count(vecino)) {
                X_new.insert(vecino);
            }
        }

        bronKerboschPivot(graph, R_new, P_new, X_new, maxClique);
            
        P.erase(nodo);
        X.insert(nodo);
    }
}


// Main function to find the maximum clique in a graph
std::vector<int> findMaximumClique(const Graph &graph) {
    int n = graph.size();
    std::vector<int> maxClique;



    // Initialize sets
    std::set<int> P, R, X;
    for (int i = 0; i < n; ++i) {
        P.insert(i);
    }

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
