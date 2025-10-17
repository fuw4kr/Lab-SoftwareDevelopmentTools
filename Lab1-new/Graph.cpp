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

Graph::~Graph() {
    for (auto& p : vertices) delete p.second;
    for (auto* e : edges) delete e;
}

void Graph::addVertex(Vertex* v) {
    if (!v) {
        LOG_WARN("Attempted to add null vertex");
        return;
    }
    vertices[v->getId()] = v;
    LOG_INFO("Added vertex ID=" + to_string(v->getId()));
}

void Graph::addEdge(Edge* e) {
    if (!e) {
        LOG_WARN("Attempted to add null edge");
        return;
    }
    edges.push_back(e);
    LOG_DEBUG("Added edge from " + to_string(e->getFrom()) + " to " + to_string(e->getTo()));
}

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
        auto removedEdges = edges.end() - it;;
        edges.erase(it, edges.end());

        delete vertices[id];
        vertices.erase(id);

        LOG_INFO("Removed vertex ID=" + to_string(id) + " and " + to_string(removedEdges) + " related edges");
    }
    else {
        LOG_WARN("Attempted to remove non-existent vertex ID=" + to_string(id));
    }
}

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

vector<int> Graph::getNeighbors(int id) const {
    vector<int> neighbors;
    for (auto* e : edges) {
        if (e->getFrom() == id) neighbors.push_back(e->getTo());
    }
    LOG_DEBUG("Found " + to_string(neighbors.size()) + " neighbors for vertex ID=" + to_string(id));
    return neighbors;
}

vector<Vertex*> Graph::getAllVertices() const {
    LOG_DEBUG("Retrieving all vertices (" + to_string(vertices.size()) + ")");
    vector<Vertex*> result;
    for (auto& p : vertices) result.push_back(p.second);
    return result;
}

vector<Edge*> Graph::getAllEdges() const {
    LOG_DEBUG("Retrieving all edges (" + to_string(edges.size()) + ")");
    return edges;
}

void Graph::print() const {
    LOG_DEBUG("Printing graph structure");
    cout << "Vertices" << endl;
    for (auto& p : vertices) p.second->print();
    cout << "Edges" << endl;
    for (auto* e : edges) e->print();
}

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