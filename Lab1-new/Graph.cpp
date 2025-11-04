/**
 * @file Graph.cpp
 * @brief Implementation of the Graph class and its methods.
 * @details Contains definitions for graph operations, including
 * vertex/edge management, logging, and exporting to DOT format for visualization.
 * @author
 * Kristina Zakharchenko
 * @date
 * 04.11.2025
 */

#include "Logger.h"
#include "Graph.h"
#include "GraphAlgorithms.h"
#include "User.h"
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <iostream>
using namespace std;

/**
 * @brief Destructor of Graph — releases memory by deleting all vertices and edges.
 */
Graph::~Graph() {
    for (auto& p : vertices) delete p.second;
    for (auto* e : edges) delete e;
}

/**
 * @brief Adds a vertex to the graph.
 * @param v Pointer to the vertex.
 */
void Graph::addVertex(Vertex* v) {
    if (!v) {
        LOG_WARN("Attempted to add null vertex");
        return;
    }
    vertices[v->getId()] = v;
    LOG_INFO("Added vertex ID=" + to_string(v->getId()));
}

/**
 * @brief Adds an edge to the graph.
 * @param e Pointer to the edge.
 */
void Graph::addEdge(Edge* e) {
    if (!e) {
        LOG_WARN("Attempted to add null edge");
        return;
    }
    edges.push_back(e);
    LOG_DEBUG("Added edge from " + to_string(e->getFrom()) + " to " + to_string(e->getTo()));
}

/**
 * @brief Removes a vertex by its ID along with all connected edges.
 * @param id Vertex identifier.
 */
void Graph::removeVertex(int id) {
    LOG_DEBUG("Attempting to remove vertex ID=" + to_string(id));
    if (vertices.count(id)) {
        auto it = remove_if(edges.begin(), edges.end(), [id](Edge* e) {
            if (e->getFrom() == id || e->getTo() == id) {
                delete e;
                return true;
            }
            return false;
            });
        auto removedEdges = edges.end() - it;
        edges.erase(it, edges.end());

        delete vertices[id];
        vertices.erase(id);

        LOG_INFO("Removed vertex ID=" + to_string(id) + " and " + to_string(removedEdges) + " related edges");
    }
    else {
        LOG_WARN("Attempted to remove non-existent vertex ID=" + to_string(id));
    }
}

/**
 * @brief Removes an edge between two vertices.
 * @param from Source vertex ID.
 * @param to Target vertex ID.
 */
void Graph::removeEdge(int from, int to) {
    LOG_DEBUG("Attempting to remove edge from " + to_string(from) + " to " + to_string(to));
    auto it = remove_if(edges.begin(), edges.end(), [from, to](Edge* e) {
        if (e->getFrom() == from && e->getTo() == to) {
            delete e;
            return true;
        }
        return false;
        });
    if (it != edges.end()) {
        edges.erase(it, edges.end());
        LOG_INFO("Removed edge from " + to_string(from) + " to " + to_string(to));
    }
    else {
        LOG_WARN("No edge found from " + to_string(from) + " to " + to_string(to));
    }
}

/**
 * @brief Retrieves a vertex by ID.
 * @param id Vertex identifier.
 * @return Pointer to the vertex or nullptr if not found.
 */
Vertex* Graph::getVertex(int id) const {
    auto it = vertices.find(id);
    if (it != vertices.end()) {
        LOG_DEBUG("Vertex found ID=" + to_string(id));
        return it->second;
    }
    else {
        LOG_DEBUG("Vertex not found ID=" + to_string(id));
        return nullptr;
    }
}

/**
 * @brief Retrieves the list of neighbors for a given vertex.
 * @param id Vertex identifier.
 * @return Vector of neighboring vertex IDs.
 */
vector<int> Graph::getNeighbors(int id) const {
    vector<int> neighbors;
    for (auto* e : edges) {
        if (e->getFrom() == id) neighbors.push_back(e->getTo());
    }
    LOG_DEBUG("Found " + to_string(neighbors.size()) + " neighbors for vertex ID=" + to_string(id));
    return neighbors;
}

/**
 * @brief Retrieves all vertices of the graph.
 * @return Vector of vertex pointers.
 */
vector<Vertex*> Graph::getAllVertices() const {
    LOG_DEBUG("Retrieving all vertices (" + to_string(vertices.size()) + ")");
    vector<Vertex*> result;
    for (auto& p : vertices) result.push_back(p.second);
    return result;
}

/**
 * @brief Retrieves all edges of the graph.
 * @return Vector of edge pointers.
 */
vector<Edge*> Graph::getAllEdges() const {
    LOG_DEBUG("Retrieving all edges (" + to_string(edges.size()) + ")");
    return edges;
}

/**
 * @brief Prints the graph structure (list of vertices and edges) to the console.
 */
void Graph::print() const {
    LOG_DEBUG("Printing graph structure");
    cout << "Vertices" << endl;
    for (auto& p : vertices) p.second->print();
    cout << "Edges" << endl;
    for (auto* e : edges) e->print();
}

/**
 * @brief Exports the graph to DOT format for Graphviz visualization.
 * @param filename File name to save to.
 * @throws std::runtime_error If the file cannot be opened.
 */
void Graph::exportToDotGraph(const string& filename) const {
    LOG_INFO("Exporting graph to DOT file: " + filename);
    ofstream file(filename);
    if (!file.is_open()) {
        LOG_ERROR("Unable to open file for writing: " + filename);
        return;
    }

    file << "digraph G {\n";
    file << "  rankdir=LR;\n";

    for (const auto& p : vertices) {
        int id = p.first;
        file << "  " << id << " [label=\"User id " << id << "\"];\n";
    }

    for (auto* e : edges) {
        if (dynamic_cast<const Friendship*>(e)) {
            file << "  " << e->getFrom() << " -> " << e->getTo()
                << " [color=blue, label=\"friend\"];\n";
        }
        else if (dynamic_cast<const Subscription*>(e)) {
            file << "  " << e->getFrom() << " -> " << e->getTo()
                << " [color=green, label=\"follow\"];\n";
        }
        else if (dynamic_cast<const Message*>(e)) {
            file << "  " << e->getFrom() << " -> " << e->getTo()
                << " [color=red, label=\"msg\"];\n";
        }
        else if (dynamic_cast<const Post*>(e)) {
            file << "  " << e->getFrom() << " -> " << e->getTo()
                << " [color=yellow, label=\"post\"];\n";
        }
        else {
            file << "  " << e->getFrom() << " -> " << e->getTo() << ";\n";
        }
    }

    file << "}\n";
    file.close();

    LOG_INFO("Graph successfully exported to " + filename);
}
