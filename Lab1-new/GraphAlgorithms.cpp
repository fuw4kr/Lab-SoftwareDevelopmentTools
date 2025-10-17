#include "GraphAlgorithms.h"
#include <algorithm>
#include <limits>
#include <set> 
#include <functional>

GraphAlgorithms::GraphAlgorithms() {}

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

bool GraphAlgorithms::isConnected(int start, int totalVertices) {
    auto dist = breadthFirstSearch(start);
    return dist.size() == totalVertices;
}

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

map<int, double> GraphAlgorithms::computeDegreeCentrality() {
    map<int, double> degree;
    for (const auto& kv : adjacencyList) {
        degree[kv.first] = static_cast<int>(kv.second.size());
    }
    return degree;
}

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

bool GraphAlgorithms::hasPath(int from, int to) {
    if (!adjacencyList.count(from) || !adjacencyList.count(to)) return false;
    auto dist = breadthFirstSearch(from);
    return dist.count(to) > 0;
}