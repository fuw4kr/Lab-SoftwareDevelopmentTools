#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <map>
#include <vector>
#include <set>
#include <queue>

using namespace std;

class GraphAlgorithms {
protected:
    map<int, vector<int>> adjacencyList;

public:
    GraphAlgorithms();

    void buildGraph(const vector<pair<int, int>>& edges);

    map<int, int> breadthFirstSearch(int start);
    bool isConnected(int start, int totalVertices);
    map<int, int> dijkstra(int start);
    map<int, double> computeDegreeCentrality();
    bool hasCycle();
    vector<vector<int>> findTriangles();
    bool hasPath(int from, int to);

private:
    bool hasCycleUtil(int v, int parent, set<int>& visited);
};

#endif // GRAPH_ALGORITHMS_H