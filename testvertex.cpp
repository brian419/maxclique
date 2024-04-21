#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <random>
#include <ctime>

using namespace std;

const int N = 100;
const int POP_SIZE = 500; // Population size for GA
const int GENERATIONS = 200; // Number of generations
const double MUTATION_RATE = 0.01; // Mutation rate
const int TOURNAMENT_SIZE = 10; // Tournament size for selection

int graph[N][N];

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

void printGraph() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

void complementGraph() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            graph[i][j] = !graph[i][j];
        }
    }
}

vector<bool> createRandomIndividual() {
    vector<bool> individual(N);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 1);

    for (int i = 0; i < N; ++i) {
        individual[i] = distrib(gen) > 0.5;
    }
    return individual;
}

int fitness(const vector<bool>& individual) {
    int coveredEdges = 0;
    set<int> coverSet;
    for (int i = 0; i < N; ++i) {
        if (individual[i]) {
            coverSet.insert(i);
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (graph[i][j] && (coverSet.count(i) > 0 || coverSet.count(j) > 0)) {
                coveredEdges++;
            }
        }
    }

    return coveredEdges - coverSet.size(); // Maximize edges covered, minimize cover size
}

vector<bool> tournamentSelection(const vector<vector<bool>>& population) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, POP_SIZE - 1);

    vector<bool> best;
    int bestFit = -1;

    for (int i = 0; i < TOURNAMENT_SIZE; ++i) {
        int idx = distrib(gen);
        int fit = fitness(population[idx]);
        if (fit > bestFit) {
            bestFit = fit;
            best = population[idx];
        }
    }
    return best;
}

vector<bool> crossover(const vector<bool>& parent1, const vector<bool>& parent2) {
    vector<bool> child(N);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, N - 1);

    int crossoverPoint = distrib(gen);

    for (int i = 0; i < crossoverPoint; ++i) {
        child[i] = parent1[i];
    }
    for (int i = crossoverPoint; i < N; ++i) {
        child[i] = parent2[i];
    }
    return child;
}

void mutate(vector<bool>& individual) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distrib(0, 1);

    for (int i = 0; i < N; ++i) {
        if (distrib(gen) < MUTATION_RATE) {
            individual[i] = !individual[i];
        }
    }
}

void vertexCover() {
    vector<vector<bool>> population(POP_SIZE);
    for (int i = 0; i < POP_SIZE; ++i) {
        population[i] = createRandomIndividual();
    }

    for (int generation = 0; generation < GENERATIONS; ++generation) {
        vector<vector<bool>> newPopulation(POP_SIZE);

        for (int i = 0; i < POP_SIZE; i += 2) {
            vector<bool> parent1 = tournamentSelection(population);
            vector<bool> parent2 = tournamentSelection(population);
            vector<bool> child1 = crossover(parent1, parent2);
            vector<bool> child2 = crossover(parent1, parent2);
            mutate(child1);
            mutate(child2);
            newPopulation[i] = child1;
            if (i + 1 < POP_SIZE) {
                newPopulation[i + 1] = child2;
            }
        }

        population = newPopulation;
    }

    // Find the best individual
    vector<bool> bestIndividual = population[0];
    int bestFit = fitness(population[0]);

    for (const auto& individual : population) {
        int currentFit = fitness(individual);
        if (currentFit > bestFit) {
            bestFit = currentFit;
            bestIndividual = individual;
        }
    }

    cout << "Vertex cover found by GA: ";
    for (int i = 0; i < N; ++i) {
        if (bestIndividual[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
    // Count vertices in the best cover
    cout << "Vertex cover size: " << count(bestIndividual.begin(), bestIndividual.end(), true) << endl;
}

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
    vector<int> independentSet;
    for (int i = 0; i < N; ++i) {
        if (vertexCoverSet.count(i) == 0) {
            independentSet.push_back(i);
        }
    }



    cout << "Independent set: ";
    for (int v : independentSet) {
        cout << v << " ";
    }
    cout << endl;
    //print the number of vertices in the independent set
    cout << "Independent set size: " << independentSet.size() << endl;

}

int main() {
    readGraph("Q15V100.adjlist");

    printGraph();
    complementGraph();
    vertexCover();
    independentSet();

    return 0;
}
