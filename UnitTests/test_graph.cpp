#include "gtest/gtest.h"
#include "Graph.h"
#include "User.h"
#include <fstream>
#include <sstream>
using namespace std;

class TestVertex : public Vertex {
public:
    TestVertex(int id) : Vertex(id) {}
    void print() const override {} 
};

class TestEdge : public Edge {
public:
    TestEdge(int f, int t) : Edge(f, t) {}
    void print() const override {} 
};

TEST(GraphTest, AddRemoveVertex) {
    Graph g;

    auto v1 = new TestVertex(1);
    g.addVertex(v1);
    EXPECT_EQ(g.getAllVertices().size(), 1);
    EXPECT_EQ(g.getVertex(1), v1);

    g.addVertex(nullptr); 
    EXPECT_EQ(g.getAllVertices().size(), 1);

    g.removeVertex(1);
    EXPECT_EQ(g.getAllVertices().size(), 0);
    EXPECT_EQ(g.getVertex(1), nullptr);

    g.removeVertex(999); 
}

TEST(GraphTest, AddRemoveEdge) {
    Graph g;
    auto v1 = new TestVertex(1);
    auto v2 = new TestVertex(2);
    g.addVertex(v1);
    g.addVertex(v2);

    auto e1 = new TestEdge(1, 2);
    g.addEdge(e1);
    EXPECT_EQ(g.getAllEdges().size(), 1);

    g.removeEdge(1, 2);
    EXPECT_EQ(g.getAllEdges().size(), 0);

    g.removeEdge(1, 2); 
}

TEST(GraphTest, GetNeighbors) {
    Graph g;
    auto v1 = new TestVertex(1);
    auto v2 = new TestVertex(2);
    auto v3 = new TestVertex(3);
    g.addVertex(v1);
    g.addVertex(v2);
    g.addVertex(v3);

    g.addEdge(new TestEdge(1, 2));
    g.addEdge(new TestEdge(1, 3));

    auto neighbors = g.getNeighbors(1);
    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_TRUE(find(neighbors.begin(), neighbors.end(), 2) != neighbors.end());
    EXPECT_TRUE(find(neighbors.begin(), neighbors.end(), 3) != neighbors.end());
}

TEST(GraphTest, TypeCastingEdges) {
    Graph g;
    auto v1 = new TestVertex(1);
    auto v2 = new TestVertex(2);
    g.addVertex(v1);
    g.addVertex(v2);

    auto f = new Friendship(1, 2);
    auto s = new Subscription(1, 2);
    g.addEdge(f);
    g.addEdge(s);

    auto friendships = g.getEdgesOfType<Friendship>();
    auto subscriptions = g.getEdgesOfType<Subscription>();

    EXPECT_EQ(friendships.size(), 1);
    EXPECT_EQ(subscriptions.size(), 1);

    auto allEdges = g.getAllEdges();
    EXPECT_EQ(g.countType<Friendship>(allEdges), 1);
    EXPECT_EQ(g.countType<Subscription>(allEdges), 1);
}

TEST(GraphTest, ExportToDot) {
    Graph g;
    auto v1 = new TestVertex(1);
    auto v2 = new TestVertex(2);
    g.addVertex(v1);
    g.addVertex(v2);

    g.addEdge(new Friendship(1, 2));
    string filename = "test_graph.dot";
    g.exportToDotGraph(filename);

    ifstream file(filename);
    EXPECT_TRUE(file.is_open());

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    EXPECT_NE(content.find("1 -> 2"), string::npos);
    EXPECT_NE(content.find("friend"), string::npos);

    file.close();
    remove(filename.c_str());
}

TEST(GraphTest, RemoveVertexWithEdges) {
    Graph g;
    auto v1 = new TestVertex(1);
    auto v2 = new TestVertex(2);
    g.addVertex(v1);
    g.addVertex(v2);
    g.addEdge(new TestEdge(1, 2));
    g.addEdge(new TestEdge(2, 1));

    g.removeVertex(1);
    EXPECT_EQ(g.getAllVertices().size(), 1);
    EXPECT_EQ(g.getAllEdges().size(), 0); 
}
