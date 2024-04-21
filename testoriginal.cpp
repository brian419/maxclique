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

const int POP_SIZE = 100;
const int GENERATIONS = 50;
const double MUTATION_RATE = 0.2;
const int TOURNAMENT_SIZE = 20;

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

void twoOptSwap(vector<bool>& individual) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, N - 1);

    int index1 = distrib(gen);
    int index2 = distrib(gen);
    while (index1 == index2) {
        index2 = distrib(gen);
    }

    swap(individual[index1], individual[index2]);
}

vector<bool> calculateVertexCover() {
    complementGraph(); // Complement the graph

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
            twoOptSwap(child1); // Apply 2-opt swap
            twoOptSwap(child2); // Apply 2-opt swap
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
    cout << "Vertex cover size: " << count(bestIndividual.begin(), bestIndividual.end(), true) << endl;

    return bestIndividual;
}

void independentSetAndClique() {
    vector<bool> vertexCover = calculateVertexCover(); // Get the vertex cover of the complemented graph
    vector<bool> independentSet(N, true); // Initialize all vertices as part of independent set

    for (int i = 0; i < N; ++i) {
        if (vertexCover[i]) { // If a vertex is in the cover, remove it from the independent set
            independentSet[i] = false;
        }
    }

    cout << "Maximum clique size: " << count(independentSet.begin(), independentSet.end(), true) << endl;

    cout << "Vertices in the maximum clique: ";
    for (int i = 0; i < N; ++i) {
        if (independentSet[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

int main() {
    readGraph("Q15V100.adjlist");

    independentSetAndClique();

    return 0;
}
