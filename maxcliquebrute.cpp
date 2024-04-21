#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

const int N = 4; 
int graph[N][N];

/**
 * @brief Reads the graph from a file, storing it in a 2D array.
 */
void readGraph(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        exit(1);
    }

    for (int i = 0; i < N; ++i) {
        string line;
        getline(file, line);
        istringstream iss(line);
        int val;
        while (iss >> val) {
            graph[i][val] = 1; 
            graph[val][i] = 1; 
        }
    }

    file.close();
}

/**
 * @brief Checks if the given set of vertices form a clique.
 */
bool isClique(const vector<int>& vertices) {
    for (int i = 0; i < vertices.size(); ++i) {
        for (int j = i + 1; j < vertices.size(); ++j) {
            if (graph[vertices[i]][vertices[j]] == 0)
                return false;
        }
    }
    return true;
}

/**
 * @brief Finds the maximum clique using a brute-force approach.
 */
vector<int> findMaxClique() {
    vector<int> bestClique;
    int maxCliqueSize = 0;
    int maxSet = 1 << N; 
    for (int subset = 1; subset < maxSet; ++subset) {
        vector<int> currentClique;
        for (int i = 0; i < N; ++i) {
            if (subset & (1 << i)) {
                currentClique.push_back(i);
            }
        }
        
        if (isClique(currentClique) && currentClique.size() > maxCliqueSize) {
            maxCliqueSize = currentClique.size();
            bestClique = currentClique;
        }
    }

    return bestClique;
}

int main() {
    readGraph("G4.adjlist");

    vector<int> maxClique = findMaxClique();
    cout << "Maximum clique size: " << maxClique.size() << endl;
    cout << "Vertices in the maximum clique: ";
    for (int v : maxClique) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
