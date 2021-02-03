# Computer-Architecture-Graph-Algorithms

- Programming assignment 2 for Computer Architecture Fall 2020.
- Exact assignment details found in a pdf within pa2 folder.
- Earned 130/150 points.
- Autograder omitted.

### Skills Learned
- C programming
- Undirected / Directed Graph Representations
- BFS / DFS algorithm
- Topological sort
- Djikstra's shortest path

### Undirected Graph Representation (First)
Represented an undirected graph with an adjacency list stored lexicographically.
Implemented functions to return the degree of a specific node and return the neighbors of a node.

### Weighted Directed Graph Representation (Second)
Modifies First to a be a weighted directed graph.
Implemented functions to return incoming and outgoing degrees and reachable neighbors of a specific node

### Breadth-first Search (Third)
Using an undirected graph from First, BFS is used to traverse the entire graph starting from a specific node.
After BFS query, it returns the reachable nodes in the order they were visited in.

### Deph-first Search (Fourth)
Similar to Third, this time with a weighted directed graph and DFS.
After DFS query, it returns the reachable nodes in the order they were visited in.

### Shortest Path in a Direct Acyclic Graph (Fifth)
Within a weighted DAG, find the shortest path from node A to every other node.
Implements the pseudocode found within the assignment pdf.
Also checks whether the graph is cyclic or not.

### Djikstra's Shortest Path (Sixth)
Implements Dijkstra's shortest path algorithm to find the shortest distance to every reachable node.

