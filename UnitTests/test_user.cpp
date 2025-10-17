#include "gtest/gtest.h"
#include "User.h"
#include <string>
using namespace std;

TEST(UserTest, CreateUserAndUpdateFields) {
    User u(1, "Alice", "alice@mail.com");
    EXPECT_EQ(u.getName(), "Alice");
    EXPECT_EQ(u.getEmail(), "alice@mail.com");
    EXPECT_EQ(u.getBio(), "");
    EXPECT_EQ(u.getLocation(), "");

    u.updateBio("Hello world");
    u.updateLocation("Kyiv");
    u.setPhone("123456789");
    u.setGender("Female");
    u.setBirthday("1990-01-01");

    EXPECT_EQ(u.getBio(), "Hello world");
    EXPECT_EQ(u.getLocation(), "Kyiv");
}

TEST(UserTest, RegularUserCounters) {
    RegularUser u(2, "Bob", "bob@mail.com");
    EXPECT_EQ(u.getLastLogin() > 0, true);

    u.addFollower();
    u.addFollowing();
    u.addPost();
    u.sendMessage();
    u.receiveMessage();
    u.changeReputation(5);
}

TEST(UserTest, RegularUserLoginUpdate) {
    RegularUser u(3, "Charlie", "charlie@mail.com");
    time_t oldLogin = u.getLastLogin();
    u.updateLastLogin();
    EXPECT_NE(u.getLastLogin(), oldLogin);
}

TEST(UserTest, PremiumUserPointsAndCheckmarks) {
    PremiumUser u(4, "Diana", "diana@mail.com", 50);

    u.addCheckmark("Verified");
    u.addCheckmark("TopContributor");

    u.bonusReputation(); 
    u.spendPremiumPoints(30);
    u.spendPremiumPoints(100); 
}

TEST(UserTest, FriendshipEdge) {
    Friendship f(1, 2);
    EXPECT_EQ(f.getFrom(), 1);
    EXPECT_EQ(f.getTo(), 2);
}

TEST(UserTest, SubscriptionEdge) {
    Subscription s(2, 3);
    EXPECT_EQ(s.getFrom(), 2);
    EXPECT_EQ(s.getTo(), 3);
}

TEST(UserTest, MessageEdge) {
    Message m(1, 3, "Hello!");
    EXPECT_EQ(m.getFrom(), 1);
    EXPECT_EQ(m.getTo(), 3);
    EXPECT_EQ(m.getText(), "Hello!");
}

TEST(UserTest, PostEdge) {
    Post p(4, "My post content");
    EXPECT_EQ(p.getFrom(), 4);
    EXPECT_EQ(p.getContent(), "My post content");
    EXPECT_EQ(p.getTo(), -1); 
}

TEST(UserTest, PremiumUserSpendTooMuch) {
    PremiumUser u(5, "Eve", "eve@mail.com", 20);
    u.spendPremiumPoints(50); 
}

TEST(UserTest, ChangeReputationNegative) {
    RegularUser u(6, "Frank", "frank@mail.com");
    u.changeReputation(-10);
}
