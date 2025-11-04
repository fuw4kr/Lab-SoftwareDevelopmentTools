/**
 * @file Graph.h
 * @brief Declarations of graph data structures and operations.
 * @details Contains the definitions of the Graph class, Vertex, Edge,
 * and supporting templates used for vertex and edge management.
 * Provides interfaces for creating, modifying, and traversing graphs.
 * @author
 * Kristina Zakharchenko
 * @date
 * 04.11.2025
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

/**
 * @brief Base template class for a graph vertex providing static polymorphism.
 * @tparam Derived Derived class implementing the printImpl() method.
 */
template <typename Derived>
class VertexBase {
protected:
    int id; ///< Unique vertex identifier.
public:
    /**
     * @brief Constructor for the VertexBase class.
     * @param id Vertex identifier.
     */
    VertexBase(int id) : id(id) {}

    /**
     * @brief Gets the vertex identifier.
     * @return Integer representing the vertex ID.
     */
    int getId() const { return id; }

    /**
     * @brief Calls the printImpl() implementation in the derived class.
     * @example
     * VertexBase<MyVertex> v(1);
     * v.Static(); // Calls MyVertex::printImpl()
     */
    void Static() const {
        static_cast<const Derived*>(this)->printImpl();
    }
};

/**
 * @brief Abstract class representing a graph vertex.
 */
class Vertex {
protected:
    int id; ///< Unique vertex identifier.
public:
    /**
     * @brief Constructor for a vertex.
     * @param id Vertex identifier.
     */
    Vertex(int id) : id(id) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~Vertex() {}

    /**
     * @brief Gets the vertex ID.
     * @return Vertex ID.
     */
    int getId() const { return id; }

    /**
     * @brief Abstract method for printing vertex information.
     */
    virtual void print() const = 0;
};

/**
 * @brief Abstract class representing a graph edge.
 */
class Edge {
protected:
    int from; ///< Source vertex ID.
    int to;   ///< Target vertex ID.
public:
    /**
     * @brief Constructor for an edge.
     * @param f Source vertex ID.
     * @param t Target vertex ID.
     */
    Edge(int f, int t) : from(f), to(t) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~Edge() {}

    /**
     * @brief Gets the source vertex ID.
     * @return ID of the source vertex.
     */
    int getFrom() const { return from; }

    /**
     * @brief Gets the target vertex ID.
     * @return ID of the target vertex.
     */
    int getTo() const { return to; }

    /**
     * @brief Abstract method for printing edge information.
     */
    virtual void print() const = 0;
};

/**
 * @brief Class representing a directed or undirected graph.
 * @details Stores vertices and edges, provides methods for adding,
 * removing, iterating, and exporting graph data.
 */
class Graph {
protected:
    map<int, Vertex*> vertices; ///< Container of all vertices.
    vector<Edge*> edges;        ///< Container of all edges.

    /**
     * @brief Graph destructor that releases memory.
     */
    virtual ~Graph();

    /**
     * @brief Adds a vertex to the graph.
     * @param v Pointer to a vertex.
     */
    virtual void addVertex(Vertex* v);

    /**
     * @brief Adds an edge to the graph.
     * @param e Pointer to an edge.
     */
    virtual void addEdge(Edge* e);

    /**
     * @brief Removes a vertex from the graph.
     * @param id Vertex identifier.
     */
    virtual void removeVertex(int id);

    /**
     * @brief Removes an edge between two vertices.
     * @param from Source vertex ID.
     * @param to Target vertex ID.
     */
    virtual void removeEdge(int from, int to);

    /**
     * @brief Gets a vertex by ID.
     * @param id Vertex identifier.
     * @return Pointer to the vertex or nullptr if not found.
     */
    virtual Vertex* getVertex(int id) const;

    /**
     * @brief Gets a list of neighboring vertex IDs.
     * @param id Vertex identifier.
     * @return Vector of neighboring vertex IDs.
     */
    virtual vector<int> getNeighbors(int id) const;

    /**
     * @brief Gets all vertices of the graph.
     * @return Vector of vertex pointers.
     */
    virtual vector<Vertex*> getAllVertices() const;

    /**
     * @brief Gets all edges of the graph.
     * @return Vector of edge pointers.
     */
    virtual vector<Edge*> getAllEdges() const;

    /**
     * @brief Prints the graph to the console.
     */
    virtual void print() const;

    /**
     * @brief Exports the graph to DOT format (for Graphviz).
     * @param filename Output filename.
     * @throws std::runtime_error If the file cannot be opened.
     */
    virtual void exportToDotGraph(const string& filename) const;

public:
    /**
     * @brief Calls a function for each vertex in the graph.
     * @param f Function that accepts a Vertex*.
     */
    template <typename Func>
    void forEachVertex(Func f) const {
        for (auto& p : vertices)
            f(p.second);
    }

    /**
     * @brief Calls a function for each edge in the graph.
     * @param f Function that accepts an Edge*.
     */
    template <typename Func>
    void forEachEdge(Func f) const {
        for (auto* e : edges)
            f(e);
    }

    /**
     * @brief Gets all vertices of a specific type.
     * @tparam T Vertex type.
     * @return Vector of vertices of type T.
     */
    template <typename T>
    vector<T*> getVerticesOfType() const {
        vector<T*> result;
        for (auto& p : vertices)
            if (auto* t = dynamic_cast<T*>(p.second))
                result.push_back(t);
        return result;
    }

    /**
     * @brief Gets all edges of a specific type.
     * @tparam T Edge type.
     * @return Vector of edges of type T.
     */
    template <typename T>
    vector<T*> getEdgesOfType() const {
        vector<T*> result;
        for (auto* e : edges)
            if (auto* t = dynamic_cast<T*>(e))
                result.push_back(t);
        return result;
    }

    /**
     * @brief Prints all provided items to the console.
     * @tparam T Item type (Vertex or Edge).
     * @param items Vector of pointers to items.
     */
    template <typename T>
    void printAll(const vector<T*>& items) const {
        for (const auto& item : items)
            if (item) item->print();
    }

    /**
     * @brief Counts the number of edges of a given type.
     * @tparam T Edge type.
     * @param allEdges Vector of all edges.
     * @return Number of edges of type T.
     */
    template <typename T>
    int countType(const vector<Edge*>& allEdges) const {
        int count = 0;
        for (auto* e : allEdges)
            if (dynamic_cast<T*>(e)) ++count;
        return count;
    }

    /**
     * @brief Removes edges of a certain type between two vertices.
     * @tparam T Edge type.
     * @param allEdges Vector of all edges.
     * @param from Source vertex ID.
     * @param to Target vertex ID.
     */
    template <typename T>
    void removeConnectionType(const vector<Edge*>& allEdges, int from, int to) {
        vector<Edge*> toRemove;
        for (auto* e : allEdges) {
            if (!e) continue;
            if (auto* t = dynamic_cast<T*>(e)) {
                if ((t->getFrom() == from && t->getTo() == to) ||
                    (t->getFrom() == to && t->getTo() == from))
                    toRemove.push_back(e);
            }
        }
        for (auto* e : toRemove)
            removeEdge(e->getFrom(), e->getTo());
    }
};

#endif // GRAPH_H
