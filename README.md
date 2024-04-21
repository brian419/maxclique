[maxcliquebrute.cpp](maxcliquebrute.cpp)
[maxcliquevertex.cpp](maxcliquevertex.cpp)
[maxcoriginal.cpp](maxoriginal.cpp)

# Max Clique Solver

This README contains information over 3 max clique implementations for my CS470 project. 

## Table of Contents

- [Introduction](#introduction)
- [Usage](#usage)
- [Algorithms Used](#algorithms-used)
- [Code Structure](#code-structure)
- [Input File Format](#input-file-format)
- [Example Output](#example-output)

## Introduction

The Maximum Clique Problem is a classic combinatorial optimization challenge in computer science. Given an undirected graph, the goal is to find the largest possible subset of vertices where every pair of vertices is connected by an edge, forming a complete subgraph known as a "clique." This problem is known to be NP-hard, meaning that there's no known efficient algorithm to solve it for large graphs in polynomial time. It has applications in various fields such as computer science, bioinformatics, and social network analysis, where identifying tightly connected groups of elements is crucial for understanding network structures and relationships.

This README describes how I implemented brute force, vertex cover, and an original algorithm implementation for the Maximum Clique problem. 

## Usage

1. Ensure you have an adjacency list file such as 'Q15V100.adjlist' in the same directory as the executable. In the adjacency list file, each new line starts with the node number. Other other number within that same line contains the vertices that node number is connected to.
   
2. Compile the program using a C++ compiler. Choose one of the following:

   ```
   g++ -std=c++11 maxcliquebrute.cpp
   g++ -std=c++11 maxcliquevertex.cpp
   g++ -std=c++11 maxcliqueoriginal.cpp
   ```

3. Run the compiled executable:

   ```
   ./a.out
   ```

## Brute Force Implementation (`maxcliquebrute.cpp`)

This brute force approach to finding the maximum clique in a graph operates by checking every possible subset of vertices to determine if it forms a clique. Key components of the implementation include:

#### Main Function:
- Initializes the graph from the input file.
- Calls the brute force clique finding function.
- Measures the execution time.
- Outputs the results including the size of the maximum clique and the members of the clique.

#### Key Functions:
- **`bool isClique(int b[], int k)`**: Checks if the vertices in array `b[]` of size `k` form a clique.
- **`void findMaxClique()`**: Iterates through all possible vertex subsets and uses `isClique` to identify the largest clique.

## 2. Vertex Cover Implementation (`maxcliquevertex.cpp`)

This Vertex Cover algorithm employs a heuristic approach to determine a vertex cover of a graph. Key features of the implementation include:

#### Main Function:
- Reads the graph data from the input.
- Implements a heuristic to estimate the vertex cover.
- Deduces potential cliques based on the vertex cover.
- Reports the results.

#### Key Functions:
- **`void approximateVertexCover()`**: Uses an approximation algorithm to find a minimal vertex cover of the graph.
- **`void reduceToClique()`**: Attempts to reduce the vertex cover to a clique by additional checks and balances.

## 3. Original Algorithm Implementation (`maxoriginal.cpp`)

The original heuristic algorithm utilizes 2 opt swap and a genetic algorithm. 

#### Main Function:
- Sets up necessary data structures and reads input.
- Applies a genetic algorithm combined with 2-opt swaps to find cliques.
- Tracks and outputs performance metrics and results.

#### Key Functions:
- **`void geneticAlgorithm()`**: Implements the genetic algorithm to evolve a population towards a maximum clique.
- **`void twoOptSwap(int &solution)`**: Applies 2-opt swaps to refine solutions within the genetic algorithm framework.

## Code Structure

- `main()`: This is the entry point of each program. It handles the initialization processes, including reading the input data and setting up any necessary structures. It also manages the execution of the main algorithm functions and times these executions, outputting the results at the end.

- `readGraphFromFile(const std::string &filename)`: Utility function to read the graph data from a file and populate the adjacency list. Used across all implementations to ensure consistency in input handling.

- `printResults(const std::vector<int> &clique, int executionTime)`: Utility function to format and print the results. It displays the size of the clique, the vertices in the clique, and the time taken to find the solution.

- Algorithm-specific functions: Each algorithm implementation has functions tailored to its specific approach, as detailed in the sections above.


## Input File Format

The input files should be in the adjacency list format where each line starts with a vertex identifier followed by a list of vertices that it is directly connected to. Each vertex and its connections should be separated by spaces. For example:

```
0
1
2 0
3 0 1 2
```

This format makes it straightforward to parse the graph and construct the adjacency list required for the clique finding algorithms.

## Example Output for Brute Force and Original Algorithm Implementation

After running the brute force algorithm, it will output the maximum clique size, the vertices in the maximum clique, and the time taken to find the solution in milliseconds.

```
Maximum clique size: 5
Vertices in the maximum clique: 0 1 5 10 13 
Time taken: 500 milliseconds
```

For the vertex cover algorithm and the original heuristic algorithm will output as the following: 

```
Vertex cover: 56 27 97 5 29 42 53 90 12 74 1 15 18 23 32 76 89 47 4 33 7 16 71 70 37 69 34 86 50 21 52 6 75 36 84 58 99 80 38 95 68 81 94 91 98 17 44 3 10 24 46 82 83 40 54 25 51 78 85 59 88 57 19 20 31 8 73 0 55 2 14 67 63 66 49 72 41 65 87 22 48 39 61 35 93 64 
Vertex cover size: 86
Maximum clique size: 14
Vertices in the maximum clique: 9 11 13 26 28 30 43 45 60 62 77 79 92 96 
```