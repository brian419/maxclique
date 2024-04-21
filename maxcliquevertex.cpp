#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int N = 100;
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

//read out the original adj list line by line from the file
void printGraph() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

//complement the graph
void complementGraph() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            graph[i][j] = !graph[i][j];
        }
    }
}

//print out the complemented graph
void printComplementGraph() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

//
void vertexCover() {
    vector<int> cover;
    vector<bool> removed(N, false);
    vector<int> degree(N, 0);

    // Compute the degrees of vertices
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            degree[i] += graph[i][j];
        }
    }

    while (true) {
        int maxDegree = 0;
        int maxVertex = -1;

        // Find the vertex with maximum degree among the remaining vertices
        for (int i = 0; i < N; ++i) {
            if (!removed[i] && degree[i] > maxDegree) {
                maxDegree = degree[i];
                maxVertex = i;
            }
        }

        // If no remaining vertex with positive degree is found, break the loop
        if (maxVertex == -1) {
            break;
        }

        // Add the maxDegree vertex to the cover set
        cover.push_back(maxVertex);
        removed[maxVertex] = true;

        // Update the degrees of adjacent vertices
        for (int i = 0; i < N; ++i) {
            if (!removed[i] && graph[maxVertex][i]) {
                --degree[i];
            }
        }
    }

    cout << "Vertex cover: ";
    for (int v : cover) {
        cout << v << " ";
    }
    cout << endl;
    //print the number of vertices in the cover
    cout << "Vertex cover size: " << cover.size() << endl;

            
}


//indepdent set = vertices of G - vertex cover of G'


void independentSet() {
    vector<int> vertexList;
    for (int i = 0; i < N; ++i) {
        vertexList.push_back(i);
    }

    vector<int> cover;
    vector<bool> removed(N, false);
    vector<int> degree(N, 0);

    // Compute the degrees of vertices
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            degree[i] += graph[i][j];
        }
    }

    while (true) {
        int maxDegree = 0;
        int maxVertex = -1;

        // Find the vertex with maximum degree among the remaining vertices
        for (int i = 0; i < N; ++i) {
            if (!removed[i] && degree[i] > maxDegree) {
                maxDegree = degree[i];
                maxVertex = i;
            }
        }

        // If no remaining vertex with positive degree is found, break the loop
        if (maxVertex == -1) {
            break;
        }

        // Add the maxDegree vertex to the cover set
        cover.push_back(maxVertex);
        removed[maxVertex] = true;

        // Update the degrees of adjacent vertices
        for (int i = 0; i < N; ++i) {
            if (!removed[i] && graph[maxVertex][i]) {
                --degree[i];
            }
        }
    }

    // Find the independent set
    set<int> vertexCoverSet(cover.begin(), cover.end());
    set<int> vertexSet(vertexList.begin(), vertexList.end());
    set<int> independentSet;
    set_difference(vertexSet.begin(), vertexSet.end(), vertexCoverSet.begin(), vertexCoverSet.end(), inserter(independentSet, independentSet.begin()));

    //print the number of vertices in the independent set
    cout << "Maximum clique size: " << independentSet.size() << endl;



    cout << "Vertices in the maximum clique: ";
    for (int v : independentSet) {
        cout << v << " ";
    }
    cout << endl;
    

}


int main() {
    readGraph("Q15V100.adjlist");

    //printGraph();
    complementGraph();
    //printComplementGraph();
    vertexCover();
    independentSet();

    return 0;
}
