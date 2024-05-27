#include "MaxClique.h"
#include <algorithm>

MaxClique::MaxClique(Graph * graph) : graph(graph), adjacencyList(graph->getGraph()) {}

void MaxClique::bronKerboschPivot(set<int>* R, vector<pair<int, int>>* P, set<int>* X, vector<int>& maxClique) {
    
    if (P->empty()) {
        if (R->size() > maxClique.size()) {
            maxClique.assign(R->begin(), R->end());
        }
        return;
    }

    int upperBound = R->size() + P->size();

    if (upperBound <= maxClique.size()) {
        return;
    }

    int pivot = P->begin()->first;
    vector<pair<int, int>> P_minus_neighbors;

    if (adjacencyList.size() * 0.7 < P->size()) {
        for (const auto& nodo : *P) {
            if (adjacencyList[pivot].size() <= nodo.second) {
                P_minus_neighbors.push_back(nodo);
            }
        }
    } else if (adjacencyList.size() * 0.2 < P->size()) {
        for (const auto& nodo : *P) {
            if (P_minus_neighbors.size() == 2){
                break;
            }
            if (adjacencyList[pivot].size() - 1 <= nodo.second) {
                P_minus_neighbors.push_back(nodo);
            }
        }
    } else {
        for (const auto& nodo : *P) {
            if (adjacencyList[pivot].size() == nodo.second && !graph->contains(adjacencyList[pivot], nodo.first)) {
                P_minus_neighbors.push_back(nodo);
            }
        }
    }

    for (const auto &nodo : P_minus_neighbors) {
        set<int> R_new = *R;
        R_new.insert(nodo.first);

        vector<pair<int, int>> P_new;
        set<int> X_new;
        for (int vecino : adjacencyList[nodo.first]) {
            if (find_if(P->begin(), P->end(), [&vecino](const pair<int, int> &p) { return p.first == vecino; }) != P->end()) {
                P_new.emplace_back(vecino, adjacencyList[vecino].size());
            }
            if (X->count(vecino)) {
                X_new.insert(vecino);
            }
        }

        bronKerboschPivot(&R_new, &P_new, &X_new, maxClique);

        P->erase(remove_if(P->begin(), P->end(), [&nodo](const pair<int, int> &p) { return p.first == nodo.first; }), P->end());
        X->insert(nodo.first);
    }
}

vector<int> MaxClique::findMaximumClique() {
    int n = adjacencyList.size();
    vector<int> maxClique;

    vector<pair<int, int>> P;
    set<int> R, X;

    for (int i = 0; i < n; ++i) {
        P.emplace_back(i, adjacencyList[i].size());
    }

    bronKerboschPivot(&R, &P, &X, maxClique);
    return maxClique;
}
