/**
 * @file GraphAlgorithms.h
 * @brief Declarations of algorithms for graph analysis and traversal.
 * @details Defines the GraphAlgorithms class that provides implementations
 * for common graph operations such as BFS, Dijkstra’s algorithm, cycle detection,
 * connectivity checking, triangle finding, and centrality computation.
 * Uses adjacency list representation internally for efficiency.
 *
 * @author
 * Kristina Zakharchenko
 * @date
 * 04.11.2025
 */

#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <map>
#include <vector>
#include <set>
#include <queue>

using namespace std;

/**
 * @brief Provides various algorithms for graph traversal and analysis.
 * @details Includes implementations of BFS, Dijkstra’s algorithm, cycle detection,
 * degree centrality, triangle detection, and path existence checking.
 */
class GraphAlgorithms {
protected:
    map<int, vector<int>> adjacencyList; ///< Graph representation using adjacency list.

public:
    /**
     * @brief Default constructor initializing an empty graph.
     */
    GraphAlgorithms();

    /**
     * @brief Builds an adjacency list representation from a list of edges.
     * @param edges Vector of edge pairs (u, v).
     * @example
     * vector<pair<int,int>> e = {{1,2}, {2,3}};
     * buildGraph(e);
     */
    void buildGraph(const vector<pair<int, int>>& edges);

    /**
     * @brief Performs Breadth-First Search (BFS) starting from a given vertex.
     * @param start Starting vertex ID.
     * @return Map of vertex IDs to their distances from the start vertex.
     */
    map<int, int> breadthFirstSearch(int start);

    /**
     * @brief Checks if the graph is connected from a given start vertex.
     * @param start Starting vertex ID.
     * @param totalVertices Total number of vertices expected in the graph.
     * @return True if all vertices are reachable, otherwise false.
     */
    bool isConnected(int start, int totalVertices);

    /**
     * @brief Executes Dijkstra’s algorithm to find the shortest paths from a start vertex.
     * @param start Starting vertex ID.
     * @return Map of vertex IDs to their shortest path distances.
     */
    map<int, int> dijkstra(int start);

    /**
     * @brief Computes the degree centrality for each vertex.
     * @return Map of vertex IDs to their degree centrality values.
     */
    map<int, double> computeDegreeCentrality();

    /**
     * @brief Checks whether the graph contains any cycles.
     * @return True if a cycle is detected, otherwise false.
     */
    bool hasCycle();

    /**
     * @brief Finds all triangles (3-cliques) in the graph.
     * @return Vector of vectors, each containing three vertex IDs forming a triangle.
     */
    vector<vector<int>> findTriangles();

    /**
     * @brief Checks if there exists a path between two vertices.
     * @param from Source vertex ID.
     * @param to Target vertex ID.
     * @return True if a path exists, otherwise false.
     */
    bool hasPath(int from, int to);

private:
    /**
     * @brief Utility method for detecting cycles using DFS.
     * @param v Current vertex.
     * @param parent Parent vertex in the DFS tree.
     * @param visited Set of visited vertices.
     * @return True if a cycle is found, otherwise false.
     */
    bool hasCycleUtil(int v, int parent, set<int>& visited);
};

#endif // GRAPH_ALGORITHMS_H
