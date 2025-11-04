/**
 * @file UserTests.cpp
 * @brief Unit tests for User, RegularUser, PremiumUser, and related Edge types.
 * @details This file verifies the correct behavior of user profile updates,
 * user statistics, premium features, and relationship edges (Friendship,
 * Subscription, Message, and Post).
 *
 * @date 04.11.2025
 * @version 1.0
 * @author
 * Kristina Zakharchenko
 */

#include "gtest/gtest.h"
#include "User.h"
#include <string>
#include <ctime>
using namespace std;

/**
 * @test Verifies that a basic User object initializes correctly
 * and that profile fields can be updated as expected.
 */

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
    EXPECT_EQ(u.getPhone(), "123456789");
    EXPECT_EQ(u.getGender(), "Female");
    EXPECT_EQ(u.getBirthday(), "1990-01-01");
}

/**
 * @test Ensures RegularUser counters (followers, posts, messages, etc.)
 * increment correctly when modified.
 */
TEST(UserTest, RegularUserCounters) {
    RegularUser u(2, "Bob", "bob@mail.com");

    EXPECT_GT(u.getLastLogin(), 0);

    u.addFollower();
    u.addFollowing();
    u.addPost();
    u.sendMessage();
    u.receiveMessage();
    u.changeReputation(5);

    EXPECT_GE(u.getFollowers(), 1);
    EXPECT_GE(u.getFollowing(), 1);
    EXPECT_GE(u.getPostsCount(), 1);
    EXPECT_GE(u.getMessagesSent(), 1);
    EXPECT_GE(u.getMessagesReceived(), 1);
    EXPECT_GE(u.getReputation(), 5);
}

/**
 * @test Confirms that RegularUser login time updates properly.
 */

TEST(UserTest, RegularUserLoginUpdate) {
    RegularUser u(3, "Charlie", "charlie@mail.com");
    time_t oldLogin = u.getLastLogin();

    u.updateLastLogin();

    EXPECT_GE(u.getLastLogin(), oldLogin)
        << "Login timestamp should not decrease after update";
}

/**
 * @test Tests PremiumUser premium points, checkmarks, and bonus system.
 */

TEST(UserTest, PremiumUserPointsAndCheckmarks) {
    PremiumUser u(4, "Diana", "diana@mail.com", 50);

    u.addCheckmark("Verified");
    u.addCheckmark("TopContributor");

    EXPECT_FALSE(u.getCheckmarks().empty());
    EXPECT_EQ(u.getCheckmarks().size(), 2);

    int before = u.getPremiumPoints();
    EXPECT_NO_THROW(u.bonusReputation());
    EXPECT_NO_THROW(u.spendPremiumPoints(30));
    EXPECT_LE(u.getPremiumPoints(), before);
}

<<<<<< feature/docs-ci
/**
 * @test Validates basic Friendship edge initialization.
 */

TEST(UserTest, FriendshipEdge) {
    Friendship f(1, 2);
    EXPECT_EQ(f.getFrom(), 1);
    EXPECT_EQ(f.getTo(), 2);
}

/**
 * @test Validates Subscription edge initialization.
 */
TEST(UserTest, SubscriptionEdge) {
    Subscription s(2, 3);
    EXPECT_EQ(s.getFrom(), 2);
    EXPECT_EQ(s.getTo(), 3);
}

/**
 * @test Checks Message edge content and sender/receiver properties.
 */
TEST(UserTest, MessageEdge) {
    Message m(1, 3, "Hello!");
    EXPECT_EQ(m.getFrom(), 1);
    EXPECT_EQ(m.getTo(), 3);
    EXPECT_EQ(m.getText(), "Hello!");
}

/**
 * @test Checks Post edge content and ensures `to` is set to -1.
 */
TEST(UserTest, PostEdge) {
    Post p(4, "My post content");
    EXPECT_EQ(p.getFrom(), 4);
    EXPECT_EQ(p.getContent(), "My post content");
    EXPECT_EQ(p.getTo(), -1);
}

/**
 * @test Verifies PremiumUser behavior when trying to spend more points
 * than available.
 */
TEST(UserTest, PremiumUserSpendTooMuch) {
    PremiumUser u(5, "Eve", "eve@mail.com", 20);
    int before = u.getPremiumPoints();

    EXPECT_NO_THROW(u.spendPremiumPoints(50));
    EXPECT_LE(u.getPremiumPoints(), before)
        << "Spending too many points should not increase total points";
}

/**
 * @test Ensures negative reputation adjustments decrease or maintain value.
 */
TEST(UserTest, ChangeReputationNegative) {
    RegularUser u(6, "Frank", "frank@mail.com");
    int before = u.getReputation();

    EXPECT_NO_THROW(u.changeReputation(-10));
    EXPECT_LE(u.getReputation(), before)
        << "Reputation should not increase when negative change applied";
}
