
#include "MaxClique.h"
#include <chrono>

int main() {
    string option;
    string filename;

    while (true) {
        cout << "\nEscoge el numero de opcion: \n\n1)Encontrar Clique Maximo \n2)Salir): ";
        cin >> option;

        if (option == "1") {
            cout << "Enter the filename: ";
            cin >> filename;

            Graph graph(filename);
            graph.printGraph();

            auto start = chrono::high_resolution_clock::now();
            MaxClique maxCliqueToFind(&graph);
            vector<int> maxClique = maxCliqueToFind.findMaximumClique();
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> duration = end - start;

            if (maxClique.empty()) {
                cout << "No hay solucion para este grafo" << endl;
            } else {
                cout << "Clique Maximo: ";
                for (int v : maxClique) {
                    cout << v << " ";
                }
                cout << "Tamaño del Clique:" << maxClique.size() << "Nodos\n";
                cout << "\nTiempo de ejecucion: " << duration.count() << " segundos\n";
            }
        } else if (option == "2") {
            cout << "Exiting the program." << endl;
            break;
        } else {
            cout << "Invalid option. Please choose 'find' or 'exit'." << endl;
        }
    }

    return 0;
}

