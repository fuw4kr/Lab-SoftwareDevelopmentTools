#include "gtest/gtest.h"
#include "SocialNetwork.h"
#include <algorithm>
using namespace std;

TEST(SocialNetworkTest, AddAndGetUser) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");

    network.addUser(u1);
    network.addUser(u2);

    EXPECT_EQ(network.getUser(1), u1);
    EXPECT_EQ(network.getUser(2), u2);
    EXPECT_EQ(network.getUser(999), nullptr); 
}

TEST(SocialNetworkTest, DuplicateUserId) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(1, "Bob", "bob@mail.com"); 

    network.addUser(u1);
    network.addUser(u2); 

    EXPECT_EQ(network.getUser(1), u1); 
    EXPECT_EQ(network.getUser(1)->getName(), "Alice");
}

TEST(SocialNetworkTest, RemoveUser) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");

    network.addUser(u1);
    network.addUser(u2);

    network.removeUser(1);
    EXPECT_EQ(network.getUser(1), nullptr);
    network.removeUser(999); 
}

TEST(SocialNetworkTest, FriendsOfUser) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");
    auto u3 = new RegularUser(3, "Charlie", "charlie@mail.com");

    network.addUser(u1);
    network.addUser(u2);
    network.addUser(u3);

    network.addFriendship(1, 2);
    network.addFriendship(2, 3);

    auto friends2 = network.getFriendsOfUser(2);
    vector<int> friendIds;
    for (auto* f : friends2) friendIds.push_back(f->getId());

    EXPECT_NE(find(friendIds.begin(), friendIds.end(), 1), friendIds.end());
    EXPECT_NE(find(friendIds.begin(), friendIds.end(), 3), friendIds.end());
}

TEST(SocialNetworkTest, MutualFriends) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");
    auto u3 = new RegularUser(3, "Charlie", "charlie@mail.com");

    network.addUser(u1);
    network.addUser(u2);
    network.addUser(u3);

    network.addFriendship(1, 2);
    network.addFriendship(2, 3);

    auto mutual = network.findMutualFriends(1, 3);
    ASSERT_EQ(mutual.size(), 1);
    EXPECT_EQ(mutual[0]->getId(), 2);
}

TEST(SocialNetworkTest, CloseFriends) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");
    auto u3 = new RegularUser(3, "Charlie", "charlie@mail.com");

    network.addUser(u1);
    network.addUser(u2);
    network.addUser(u3);

    network.addFriendship(1, 2);
    network.addFriendship(2, 3);

    auto close = network.findCloseFriends(1);
    vector<int> closeIds;
    for (auto* u : close) closeIds.push_back(u->getId());

    EXPECT_NE(find(closeIds.begin(), closeIds.end(), 3), closeIds.end());
}

TEST(SocialNetworkTest, AddNullUser) {
    SocialNetwork network;
    network.addUser(nullptr); 
}

TEST(SocialNetworkTest, AddFriendshipInvalidIds) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    network.addUser(u1);

    network.addFriendship(1, 999);  
    network.addFriendship(999, 1); 
}

TEST(SocialNetworkTest, RemoveFriendshipInvalidIds) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    network.addUser(u1);

    network.removeFriendship(1, 999);  
}

TEST(SocialNetworkTest, SendMessageInvalidUsers) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    network.addUser(u1);

    network.sendMessage(1, 999, "Hi"); 
    network.sendMessage(999, 1, "Hi"); 
}

TEST(SocialNetworkTest, AddPostInvalidUser) {
    SocialNetwork network;
    network.addPost(999, "Hello");  
}

TEST(SocialNetworkTest, AreConnectedAndDistance) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");
    auto u3 = new RegularUser(3, "Charlie", "charlie@mail.com");

    network.addUser(u1);
    network.addUser(u2);
    network.addUser(u3);

    network.addFriendship(1, 2);
    network.addFriendship(2, 3);

    EXPECT_TRUE(network.areConnected(1, 3));
    EXPECT_EQ(network.distanceBetween(1, 3), 2);
    EXPECT_EQ(network.distanceBetween(1, 999), -1); 
}

TEST(SocialNetworkTest, CommonSubscriptions) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");

    network.addUser(u1);
    network.addUser(u2);

    network.addSubscription(1, 2);

    auto commonSubs = network.findCommonSubscriptions(1, 2);
    EXPECT_EQ(commonSubs.size(), 0);
}

TEST(SocialNetworkTest, DetectFriendGroups) {
    SocialNetwork network;
    auto u1 = new RegularUser(1, "Alice", "alice@mail.com");
    auto u2 = new RegularUser(2, "Bob", "bob@mail.com");
    auto u3 = new RegularUser(3, "Charlie", "charlie@mail.com");

    network.addUser(u1);
    network.addUser(u2);
    network.addUser(u3);

    network.addFriendship(1, 2);
    network.addFriendship(2, 3);
    network.addFriendship(3, 1);

    auto groups = network.detectFriendGroups();
    EXPECT_EQ(groups.size(), 1); 
}
