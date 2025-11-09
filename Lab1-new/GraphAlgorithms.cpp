/**
 * @file GraphAlgorithms.cpp
 * @brief Implementation of graph algorithms for traversal and analysis.
 * @details Provides definitions for graph operations such as BFS, Dijkstra,
 * connectivity check, cycle detection, degree centrality computation, and
 * triangle finding. Uses an adjacency list structure for efficient traversal.
 *
 * @see GraphAlgorithms
 *
 * @author
 * Kristina Zakharchenko
 * @date
 * 04.11.2025
 */

#include "GraphAlgorithms.h"
#include <algorithm>
#include <limits>
#include <set>
#include <functional>

 /**
  * @brief Default constructor initializing an empty graph structure.
  */
GraphAlgorithms::GraphAlgorithms() {}

/**
 * @brief Builds the graph from a list of edges and creates an adjacency list.
 * @param edges Vector of edge pairs (u, v).
 */
void GraphAlgorithms::buildGraph(const vector<pair<int, int>>& edges) {
    adjacencyList.clear();
    for (const auto& e : edges) {
        adjacencyList[e.first].push_back(e.second);
        adjacencyList[e.second].push_back(e.first);
    }
    for (auto& kv : adjacencyList) {
        sort(kv.second.begin(), kv.second.end());
    }
}

/**
 * @brief Performs Breadth-First Search (BFS) from a given starting vertex.
 * @param start Starting vertex ID.
 * @return Map of vertex IDs and their distances from the starting vertex.
 */
map<int, int> GraphAlgorithms::breadthFirstSearch(int start) {
    map<int, int> dist;
    if (!adjacencyList.count(start)) return dist;

    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adjacencyList[u]) {
            if (!dist.count(v)) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

/**
 * @brief Checks if the graph is connected using BFS.
 * @param start Starting vertex ID.
 * @param totalVertices Expected total number of vertices.
 * @return True if the graph is connected, otherwise false.
 */
bool GraphAlgorithms::isConnected(int start, int totalVertices) {
    auto dist = breadthFirstSearch(start);
    return dist.size() == totalVertices;
}

/**
 * @brief Executes Dijkstra’s algorithm to find the shortest path distances.
 * @param start Starting vertex ID.
 * @return Map of vertex IDs to shortest path distances.
 */
map<int, int> GraphAlgorithms::dijkstra(int start) {
    map<int, int> dist;
    for (const auto& kv : adjacencyList) {
        dist[kv.first] = numeric_limits<int>::max();
    }
    if (dist.count(start) == 0) return {};
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int d = top.first, u = top.second;
        if (d > dist[u]) continue;

        for (int v : adjacencyList[u]) {
            if (dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                pq.push({ dist[v], v });
            }
        }
    }
    return dist;
}

/**
 * @brief Computes degree centrality for each vertex.
 * @return Map of vertex IDs to degree centrality values.
 */
map<int, double> GraphAlgorithms::computeDegreeCentrality() {
    map<int, double> degree;
    for (const auto& kv : adjacencyList) {
        degree[kv.first] = static_cast<int>(kv.second.size());
    }
    return degree;
}

/**
 * @brief Checks whether the graph contains a cycle.
 * @return True if a cycle is detected, otherwise false.
 */
bool GraphAlgorithms::hasCycle() {
    set<int> visited;
    for (const auto& kv : adjacencyList) {
        int node = kv.first;
        if (!visited.count(node)) {
            if (hasCycleUtil(node, -1, visited)) return true;
        }
    }
    return false;
}

/**
 * @brief Utility function for DFS-based cycle detection.
 * @param v Current vertex.
 * @param parent Parent vertex in the DFS traversal.
 * @param visited Set of visited vertices.
 * @return True if a cycle is detected, otherwise false.
 */
bool GraphAlgorithms::hasCycleUtil(int v, int parent, set<int>& visited) {
    visited.insert(v);
    for (int u : adjacencyList[v]) {
        if (!visited.count(u)) {
            if (hasCycleUtil(u, v, visited)) return true;
        }
        else if (u != parent) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Finds all triangles (three interconnected vertices) in the graph.
 * @return Vector of triples representing triangles (u, v, w).
 */
vector<vector<int>> GraphAlgorithms::findTriangles() {
    vector<vector<int>> triangles;
    map<int, set<int>> adjSet;
    for (auto& kv : adjacencyList) {
        adjSet[kv.first] = set<int>(kv.second.begin(), kv.second.end());
    }

    for (auto& ku : adjacencyList) {
        int u = ku.first;
        for (int v : ku.second) {
            if (v <= u) continue;
            for (int w : adjacencyList[v]) {
                if (w <= v) continue;
                if (adjSet[w].count(u)) {
                    triangles.push_back({ u, v, w });
                }
            }
        }
    }
    return triangles;
}

/**
 * @brief Checks if there is a path between two vertices using BFS.
 * @param from Source vertex ID.
 * @param to Target vertex ID.
 * @return True if a path exists, otherwise false.
 */
bool GraphAlgorithms::hasPath(int from, int to) {
    if (!adjacencyList.count(from) || !adjacencyList.count(to)) return false;
    auto dist = breadthFirstSearch(from);
    return dist.count(to) > 0;
}
