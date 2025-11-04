/**
 * @file GraphTests.cpp
 * @brief Unit tests for the Graph class and its core operations.
 * @details Verifies vertex and edge management, type casting,
 * neighbor detection, DOT export, and cleanup using Google Test.
 *
 * @date 04.11.2025
 * @version 1.0
 * @author
 * Kristina Zakharchenko
 */

#include "gtest/gtest.h"
#include "Graph.h"
#include "User.h"
#include <fstream>
#include <sstream>
using namespace std;

/**
 * @brief Simple test vertex implementation for Graph testing.
 */
class TestVertex : public Vertex {
public:
    explicit TestVertex(int id) : Vertex(id) {}
    void print() const override {}
};

/**
 * @brief Simple test edge implementation for Graph testing.
 */
class TestEdge : public Edge {
public:
    TestEdge(int f, int t) : Edge(f, t) {}
    void print() const override {}
};

/**
 * @test Verifies adding and removing vertices from the graph.
 */
TEST(GraphTest, AddRemoveVertex) {
    Graph g;

    auto v1 = new TestVertex(1);
    g.addVertex(v1);
    EXPECT_EQ(g.getAllVertices().size(), 1);
    EXPECT_EQ(g.getVertex(1), v1);

    g.addVertex(nullptr); // should not crash
    EXPECT_EQ(g.getAllVertices().size(), 1);

    g.removeVertex(1);
    EXPECT_EQ(g.getAllVertices().size(), 0);
    EXPECT_EQ(g.getVertex(1), nullptr);

    g.removeVertex(999); // removing non-existent vertex
}

/**
 * @test Verifies adding and removing edges between vertices.
 */
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

    g.removeEdge(1, 2); // removing again should be safe
}

/**
 * @test Checks neighbor retrieval for a vertex with multiple connections.
 */
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
    EXPECT_NE(find(neighbors.begin(), neighbors.end(), 2), neighbors.end());
    EXPECT_NE(find(neighbors.begin(), neighbors.end(), 3), neighbors.end());
}

/**
 * @test Ensures dynamic_cast works correctly for edge subclasses.
 */
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

/**
 * @test Checks that exporting to DOT format produces a valid file.
 */
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

/**
 * @test Ensures vertex removal also deletes all connected edges.
 */
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

/**
 * @test Verifies that duplicate vertex IDs are ignored or replaced safely.
 */
TEST(GraphTest, AddDuplicateVertex) {
    Graph g;
    auto v1 = new TestVertex(1);
    auto v2 = new TestVertex(1);
    g.addVertex(v1);
    size_t before = g.getAllVertices().size();
    g.addVertex(v2);
    EXPECT_EQ(g.getAllVertices().size(), before)
        << "Graph should reject duplicate vertex IDs";
}

/**
 * @test Ensures adding edges with non-existent endpoints is handled gracefully.
 */
TEST(GraphTest, AddEdgeWithMissingEndpoints) {
    Graph g;
    auto v1 = new TestVertex(1);
    g.addVertex(v1);

    auto e1 = new TestEdge(1, 2);
    EXPECT_NO_THROW({ g.addEdge(e1); });

    EXPECT_EQ(g.getAllEdges().size(), 0);

    auto e2 = new TestEdge(3, 4);
    EXPECT_NO_THROW({ g.addEdge(e2); });
    EXPECT_EQ(g.getAllEdges().size(), 0);
}

/**
 * @test Checks that querying neighbors of a missing vertex returns an empty list.
 */
TEST(GraphTest, GetNeighborsOfNonExistingVertex) {
    Graph g;
    EXPECT_TRUE(g.getNeighbors(999).empty())
        << "getNeighbors for non-existing vertex should return empty vector";
}

/**
 * @test Ensures exporting an empty graph still produces a valid DOT file.
 */
TEST(GraphTest, ExportToDotEmptyGraph) {
    Graph g;
    EXPECT_NO_THROW({ g.exportToDotGraph("empty.dot"); });

    ifstream file("empty.dot");
    ASSERT_TRUE(file.is_open());
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    EXPECT_NE(content.find("graph"), string::npos)
        << "DOT file should contain 'graph' keyword even if graph is empty";
    file.close();
}

/**
 * @test Verifies proper memory cleanup on Graph destruction.
 */
TEST(GraphTest, DestructorCleanup) {
    {
        Graph g;
        auto v1 = new TestVertex(1);
        auto v2 = new TestVertex(2);
        g.addVertex(v1);
        g.addVertex(v2);
        g.addEdge(new TestEdge(1, 2));
    }
    SUCCEED(); // No crashes or leaks
}
