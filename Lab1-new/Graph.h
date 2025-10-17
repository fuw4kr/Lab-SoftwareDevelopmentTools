#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

template <typename Derived>
class VertexBase {
protected:
    int id;
public:
    VertexBase(int id) : id(id) {}
    int getId() const { return id; }

    void Static() const {
        static_cast<const Derived*>(this)->printImpl();
    }
};

class Vertex {
protected:
    int id;
public:
    Vertex(int id) : id(id) {}
    virtual ~Vertex() {}
    int getId() const { return id; }
    virtual void print() const = 0;
};

class Edge {
protected:
    int from;
    int to;
public:
    Edge(int f, int t) : from(f), to(t) {}
    virtual ~Edge() {}
    int getFrom() const { return from; }
    int getTo() const { return to; }
    virtual void print() const = 0;
};


class Graph {
protected:
    map<int, Vertex*> vertices;
    vector<Edge*> edges;

    virtual ~Graph();

    virtual void addVertex(Vertex* v);
    virtual void addEdge(Edge* e);
    virtual void removeVertex(int id);
    virtual void removeEdge(int from, int to);

    virtual Vertex* getVertex(int id) const;
    virtual vector<int> getNeighbors(int id) const;
    virtual vector<Vertex*> getAllVertices() const;
    virtual vector<Edge*> getAllEdges() const;

    virtual void print() const;
    virtual void exportToDotGraph(const string& filename) const;

public:
    template <typename Func>
    void forEachVertex(Func f) const {
        for (auto& p : vertices)
            f(p.second);
    }

    template <typename Func>
    void forEachEdge(Func f) const {
        for (auto* e : edges)
            f(e);
    }

    template <typename T>
    vector<T*> getVerticesOfType() const {
        vector<T*> result;
        for (auto& p : vertices)
            if (auto* t = dynamic_cast<T*>(p.second))
                result.push_back(t);
        return result;
    }

    template <typename T>
    vector<T*> getEdgesOfType() const {
        vector<T*> result;
        for (auto* e : edges)
            if (auto* t = dynamic_cast<T*>(e))
                result.push_back(t);
        return result;
    }

    template <typename T>
    void printAll(const vector<T*>& items) const {
        for (const auto& item : items)
            if (item) item->print();
    }

    template <typename T>
    int countType(const vector<Edge*>& allEdges) const {
        int count = 0;
        for (auto* e : allEdges)
            if (dynamic_cast<T*>(e)) ++count;
        return count;
    }

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

