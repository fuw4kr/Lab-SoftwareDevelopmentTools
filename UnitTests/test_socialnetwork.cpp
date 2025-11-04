/**
 * @file SocialNetworkTests.cpp
 * @brief Unit tests for the SocialNetwork class and its user, friendship, and messaging logic.
 * @details These tests cover user creation, friendship management, messaging,
 * posting, relationship handling, and various edge cases.
 *
 * @date 04.11.2025
 * @version 1.0
 * @author
 * Kristina Zakharchenko
 */

#include "gtest/gtest.h"
#include "SocialNetwork.h"
#include <algorithm>
#include <vector>
using namespace std;

/**
 * @class SocialNetworkTest
 * @brief Google Test fixture providing a reusable setup for SocialNetwork tests.
 */

class SocialNetworkTest : public ::testing::Test {
protected:
    SocialNetwork network;
    RegularUser* u1, * u2, * u3;
    vector<RegularUser*> createdUsers;

    void SetUp() override {
        ASSERT_NO_FATAL_FAILURE(([&] {
            u1 = new RegularUser(1, "Alice", "alice@mail.com");
            u2 = new RegularUser(2, "Bob", "bob@mail.com");
            u3 = new RegularUser(3, "Charlie", "charlie@mail.com");
            createdUsers = { u1, u2, u3 };
            for (auto* u : createdUsers)
                network.addUser(u);
            })());
            createdUsers = {u1, u2, u3};
            for (auto* u : createdUsers) 
                network.addUser(u);       
        })());
    }

    void TearDown() override {
        for (auto* u : createdUsers)
            delete u;
        createdUsers.clear();
    }
};

/**
 * @test Ensures users can be added and retrieved correctly.
 */

TEST_F(SocialNetworkTest, AddAndGetUser) {
    ASSERT_NE(network.getUser(1), nullptr);
    ASSERT_NE(network.getUser(2), nullptr);

    EXPECT_EQ(network.getUser(1), u1);
    EXPECT_EQ(network.getUser(2), u2);
    EXPECT_EQ(network.getUser(999), nullptr);
}

/**
 * @test Checks duplicate user IDs handling (should remain consistent).
 */
TEST_F(SocialNetworkTest, DuplicateUserId) {
    ASSERT_NE(network.getUser(1), nullptr);
    EXPECT_EQ(network.getUser(1)->getName(), "Alice");
}

/**
 * @test Verifies removing existing and non-existing users works safely.
 */
TEST_F(SocialNetworkTest, DuplicateUserId) {
    ASSERT_NE(network.getUser(1), nullptr);
    EXPECT_EQ(network.getUser(1), u1);
    EXPECT_EQ(network.getUser(1)->getName(), "Alice");
}

TEST_F(SocialNetworkTest, RemoveUser) {
    ASSERT_NO_FATAL_FAILURE(network.removeUser(1));
    EXPECT_EQ(network.getUser(1), nullptr);
    EXPECT_NO_THROW(network.removeUser(999));
}

/**
 * @test Ensures friendship relationships are properly stored and retrievable.
 */

TEST_F(SocialNetworkTest, FriendsOfUser) {
    ASSERT_NO_FATAL_FAILURE({
        network.addFriendship(1, 2);
        network.addFriendship(2, 3);
        });

    auto friends2 = network.getFriendsOfUser(2);
    ASSERT_FALSE(friends2.empty());

    vector<int> friendIds;
    for (auto* f : friends2) friendIds.push_back(f->getId());
    ASSERT_FALSE(friends2.empty()) << "User 2 should have friends before checking IDs";

    vector<int> friendIds;
    for (auto* f : friends2)
        friendIds.push_back(f->getId());

    EXPECT_TRUE(find(friendIds.begin(), friendIds.end(), 1) != friendIds.end());
    EXPECT_TRUE(find(friendIds.begin(), friendIds.end(), 3) != friendIds.end());
    EXPECT_EQ(friendIds.size(), 2);
}

/**
 * @test Validates that mutual friends are detected correctly.
 */
TEST_F(SocialNetworkTest, MutualFriends) {
    ASSERT_NO_FATAL_FAILURE({
        network.addFriendship(1, 2);
        network.addFriendship(2, 3);
        });

    auto mutual = network.findMutualFriends(1, 3);
    ASSERT_FALSE(mutual.empty());

    vector<int> mutualIds;
    for (auto* u : mutual) mutualIds.push_back(u->getId());
    ASSERT_FALSE(mutual.empty()) << "Mutual friends should not be empty";

    vector<int> mutualIds;
    for (auto* u : mutual)
        mutualIds.push_back(u->getId());

    EXPECT_EQ(mutualIds.size(), 1);
    EXPECT_TRUE(find(mutualIds.begin(), mutualIds.end(), 2) != mutualIds.end());
}

/**
 * @test Confirms detection of �close friends� (friends-of-friends).
 */

TEST_F(SocialNetworkTest, CloseFriends) {
    ASSERT_NO_FATAL_FAILURE({
        network.addFriendship(1, 2);
        network.addFriendship(2, 3);
        });

    auto close = network.findCloseFriends(1);
    ASSERT_FALSE(close.empty());

    vector<int> closeIds;
    for (auto* u : close) closeIds.push_back(u->getId());
    ASSERT_FALSE(close.empty()) << "Close friends list should not be empty";

    vector<int> closeIds;
    for (auto* u : close)
        closeIds.push_back(u->getId());

    EXPECT_TRUE(find(closeIds.begin(), closeIds.end(), 3) != closeIds.end());
}

/**
 * @test Adding null users should not cause crashes or alter the graph.
 */
TEST_F(SocialNetworkTest, AddNullUser_NoCrash) {
    EXPECT_NO_THROW(network.addUser(nullptr));

TEST_F(SocialNetworkTest, AddNullUser_NoCrash) {
    EXPECT_NO_THROW(network.addUser(nullptr));

    EXPECT_EQ(network.getUser(1), u1);
    EXPECT_EQ(network.getUser(999), nullptr);
}

/**
 * @test Adding friendships with invalid user IDs should not crash.
 */
TEST_F(SocialNetworkTest, AddFriendship_InvalidIds_NoCrash) {
    ASSERT_NO_FATAL_FAILURE({
        EXPECT_NO_THROW({
            network.addFriendship(1, 999);
            network.addFriendship(999, 1);
        });
        });

    EXPECT_EQ(network.getFriendsOfUser(1).size(), 0);
}

/**
 * @test Removing friendships with invalid IDs should be safe.
 */

TEST_F(SocialNetworkTest, RemoveFriendship_InvalidIds_NoCrash) {
    ASSERT_NO_FATAL_FAILURE({
        EXPECT_NO_THROW(network.removeFriendship(1, 999));
        });

    EXPECT_EQ(network.getFriendsOfUser(1).size(), 0);
}

/**
 * @test Invalid message sending attempts should not crash or create edges.
 */

TEST_F(SocialNetworkTest, SendMessage_InvalidUsers_NoCrash) {
    ASSERT_NO_FATAL_FAILURE({
        EXPECT_NO_THROW({
            network.sendMessage(1, 999, "Hi");
            network.sendMessage(999, 1, "Hi");
        });
        });
}

/**
 * @test Adding posts for invalid users should not crash.
 */

    EXPECT_TRUE(network.getMessagesOfUser(1).empty());
}

TEST_F(SocialNetworkTest, AddPost_InvalidUser_NoCrash) {
    ASSERT_NO_FATAL_FAILURE({
        EXPECT_NO_THROW(network.addPost(999, "Hello"));
        });
    EXPECT_EQ(network.getUser(999), nullptr);
}

/**
 * @test Verifies that areConnected() and distanceBetween() work correctly.
 */


    EXPECT_EQ(network.getUser(999), nullptr);
}

TEST_F(SocialNetworkTest, AreConnectedAndDistance) {
    ASSERT_NO_FATAL_FAILURE({
        network.addFriendship(1, 2);
        network.addFriendship(2, 3);
        });

    ASSERT_TRUE(network.areConnected(1, 3));
    EXPECT_EQ(network.distanceBetween(1, 3), 2);
    EXPECT_EQ(network.distanceBetween(1, 999), -1);
}

/**
 * @test Validates that users can have subscriptions, and shared ones are detected.
 */

TEST_F(SocialNetworkTest, CommonSubscriptions) {
    ASSERT_NO_FATAL_FAILURE(network.addSubscription(1, 2));
    auto commonSubs = network.findCommonSubscriptions(1, 2);
    EXPECT_EQ(commonSubs.size(), 0);
}

/**
 * @test Detects friend triangles (3-member groups).
 */

TEST_F(SocialNetworkTest, DetectFriendGroups) {
    ASSERT_NO_FATAL_FAILURE({
        network.addFriendship(1, 2);
        network.addFriendship(2, 3);
        network.addFriendship(3, 1);
        });

    auto groups = network.detectFriendGroups();
    ASSERT_FALSE(groups.empty());

    bool triangleFound = false;
    for (const auto& group : groups) {
        if (find(group.begin(), group.end(), 1) != group.end() &&
            find(group.begin(), group.end(), 2) != group.end() &&
            find(group.begin(), group.end(), 3) != group.end()) {
    ASSERT_FALSE(groups.empty()) << "No friend groups detected";

    bool triangleFound = false;
    for (const auto& group : groups) {
        vector<int> groupIds(group.begin(), group.end());
        if (find(groupIds.begin(), groupIds.end(), 1) != groupIds.end() &&
            find(groupIds.begin(), groupIds.end(), 2) != groupIds.end() &&
            find(groupIds.begin(), groupIds.end(), 3) != groupIds.end()) {
            triangleFound = true;
            break;
        }
    }
    EXPECT_TRUE(triangleFound);
}

/**
 * @test Duplicate friendships should not result in multiple edges.
 */
TEST_F(SocialNetworkTest, AddDuplicateFriendship_Idempotent) {
    ASSERT_NO_FATAL_FAILURE(network.addFriendship(1, 2));

    EXPECT_NO_THROW({
        network.addFriendship(1, 2);
        network.addFriendship(2, 1);
        });

    EXPECT_TRUE(triangleFound);
}

TEST_F(SocialNetworkTest, AddDuplicateFriendship_Idempotent) {
    ASSERT_NO_FATAL_FAILURE(network.addFriendship(1, 2));

    EXPECT_NO_THROW({ 
        network.addFriendship(1, 2);
        network.addFriendship(2, 1);
    });

    auto friends1 = network.getFriendsOfUser(1);
    auto friends2 = network.getFriendsOfUser(2);

    ASSERT_FALSE(friends1.empty());
    ASSERT_FALSE(friends2.empty());

    int countA = count_if(friends1.begin(), friends1.end(),
        [](User* u) { return u->getId() == 2; });
    int countB = count_if(friends2.begin(), friends2.end(),
        [](User* u) { return u->getId() == 1; });

    EXPECT_EQ(countA, 1);
    EXPECT_EQ(countB, 1);
}

/**
 * @test Friendship removal should be symmetric (both directions).
 */
TEST_F(SocialNetworkTest, RemoveExistingFriendship_BothDirectionsRemoved) {
    ASSERT_NO_FATAL_FAILURE({
        network.addFriendship(1, 2);
        network.removeFriendship(1, 2);
        });

    EXPECT_TRUE(network.getFriendsOfUser(1).empty());
    EXPECT_TRUE(network.getFriendsOfUser(2).empty());
}

/**
 * @test Sending a valid message should create a message edge.
 */
TEST_F(SocialNetworkTest, SendMessage_ValidUsers_CreatesMessageEdge) {
    ASSERT_NO_FATAL_FAILURE(network.addFriendship(1, 2));
    EXPECT_NO_THROW(network.sendMessage(1, 2, "Hi Bob!"));
}

/**
 * @test Adding a valid post should store it successfully.
 */
TEST_F(SocialNetworkTest, AddPost_ValidUser_StoredSuccessfully) {
    EXPECT_NO_THROW(network.addPost(1, "Hello world!"));

    EXPECT_EQ(countA, 1) << "Friendship should not duplicate for user 1";
    EXPECT_EQ(countB, 1) << "Friendship should not duplicate for user 2";
}

TEST_F(SocialNetworkTest, RemoveExistingFriendship_BothDirectionsRemoved) {
    ASSERT_NO_FATAL_FAILURE({ 
        network.addFriendship(1, 2);
        network.removeFriendship(1, 2);
    });

    auto friends1 = network.getFriendsOfUser(1);
    auto friends2 = network.getFriendsOfUser(2);

    EXPECT_TRUE(friends1.empty()) << "Friendship removal should affect both sides";
    EXPECT_TRUE(friends2.empty()) << "Friendship removal should be symmetric";
}

TEST_F(SocialNetworkTest, SendMessage_ValidUsers_CreatesMessageEdge) {
    ASSERT_NO_FATAL_FAILURE(network.addFriendship(1, 2));

    EXPECT_NO_THROW(network.sendMessage(1, 2, "Hi Bob!"));

    auto messages = network.getMessagesOfUser(1);
    ASSERT_FALSE(messages.empty()) << "Message should be stored for sender";

    bool found = any_of(messages.begin(), messages.end(), [](auto* msg) {
        return msg->getTo() == 2;
        });
    EXPECT_TRUE(found) << "Message edge should exist between sender and receiver";
}

TEST_F(SocialNetworkTest, AddPost_ValidUser_StoredSuccessfully) {
    EXPECT_NO_THROW(network.addPost(1, "Hello world!"));

    auto posts = network.getPostsOfUser(1);
    ASSERT_FALSE(posts.empty()) << "Valid user should have posts stored";

    bool postExists = any_of(posts.begin(), posts.end(), [](auto* p) {
        return p->getContent() == "Hello world!";
        });
    EXPECT_TRUE(postExists) << "Post content should match the one added";
}
