/**
 * @file SocialNetwork.cpp
 * @brief Implementation of the SocialNetwork class.
 * @details Contains definitions of methods for managing users,
 * friendships, subscriptions, messages, posts, and performing
 * network analytics such as connectivity, distance, and centrality.
 *
 * @see SocialNetwork
 * @see Graph
 * @see GraphAlgorithms
 *
 * @date 04.11.2025
 * @version 1.0
 * @autor Kristina Zakharchenko
 */

#include "SocialNetwork.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
using namespace std;

/**
 * @brief Adds a new user to the social network.
 * @param user Pointer to the User object.
 */
void SocialNetwork::addUser(User* user) {
    if (!user) {
        LOG_ERROR("Attempted to add null user");
        return;
    }
    addVertex(user);
    LOG_INFO("Added user ID=" + to_string(user->getId()) + " name=" + user->getName());
}

/**
 * @brief Removes a user by their ID.
 * @param userId ID of the user to remove.
 */
void SocialNetwork::removeUser(int userId) {
    LOG_INFO("Removing user ID=" + to_string(userId));
    removeVertex(userId);
}

/**
 * @brief Retrieves a user by their ID.
 * @param userId ID of the user to retrieve.
 * @return Pointer to the User or nullptr if not found.
 */
User* SocialNetwork::getUser(int userId) const {
    Vertex* v = getVertex(userId);
    if (!v) {
        LOG_WARN("User not found: ID=" + to_string(userId));
        return nullptr;
    }
    User* u = dynamic_cast<User*>(v);
    if (u)
        LOG_DEBUG("User found: ID=" + to_string(userId));
    else
        LOG_WARN("Vertex is not a User: ID=" + to_string(userId));
    return u;
}

/**
 * @brief Creates a mutual friendship between two users.
 * @param userA First user's ID.
 * @param userB Second user's ID.
 */
void SocialNetwork::addFriendship(int userA, int userB) {
    if (!getUser(userA) || !getUser(userB)) {
        LOG_ERROR("Invalid friendship IDs: " + to_string(userA) + ", " + to_string(userB));
        return;
    }
    addEdge(new Friendship(userA, userB));
    addEdge(new Friendship(userB, userA));
    LOG_INFO("Created friendship between " + to_string(userA) + " and " + to_string(userB));
}

/**
 * @brief Removes a friendship between two users.
 * @param userA First user's ID.
 * @param userB Second user's ID.
 */
void SocialNetwork::removeFriendship(int userA, int userB) {
    LOG_INFO("Removing friendship between users " + to_string(userA) + " and " + to_string(userB));
    removeEdge(userA, userB);
    removeEdge(userB, userA);
}

/**
 * @brief Adds a subscription (follow relationship) between two users.
 * @param followerId ID of the follower.
 * @param followeeId ID of the user being followed.
 */
void SocialNetwork::addSubscription(int followerId, int followeeId) {
    LOG_INFO("Adding subscription: " + to_string(followerId) + " -> " + to_string(followeeId));
    addEdge(new Subscription(followerId, followeeId));

    if (auto* f = dynamic_cast<RegularUser*>(getUser(followerId)))
        f->addFollowing();
    if (auto* fe = dynamic_cast<RegularUser*>(getUser(followeeId)))
        fe->addFollower();
}

/**
 * @brief Sends a message from one user to another.
 * @param senderId Sender's ID.
 * @param receiverId Receiver's ID.
 * @param text Message content.
 */
void SocialNetwork::sendMessage(int senderId, int receiverId, const string& text) {
    if (!getUser(senderId) || !getUser(receiverId)) {
        LOG_ERROR("Cannot send message — user not found");
        return;
    }
    addEdge(new Message(senderId, receiverId, text));

    if (auto* s = dynamic_cast<RegularUser*>(getUser(senderId)))
        s->sendMessage();
    if (auto* r = dynamic_cast<RegularUser*>(getUser(receiverId)))
        r->receiveMessage();

    LOG_INFO("Message: " + to_string(senderId) + " to " + to_string(receiverId) + " | " + text);
}

/**
 * @brief Adds a post created by a user.
 * @param authorId Author's ID.
 * @param content Post content.
 */
void SocialNetwork::addPost(int authorId, const string& content) {
    if (!getUser(authorId)) {
        LOG_ERROR("Cannot add post — user not found: " + to_string(authorId));
        return;
    }
    addEdge(new Post(authorId, content));
    if (auto* u = dynamic_cast<RegularUser*>(getUser(authorId)))
        u->addPost();
    LOG_INFO("User " + to_string(authorId) + " posted: " + content);
}

/**
 * @brief Retrieves a list of friends for a given user.
 * @param userId ID of the user.
 * @return Vector of User pointers representing friends.
 */
vector<User*> SocialNetwork::getFriendsOfUser(int userId) {
    LOG_DEBUG("Getting friends of user ID=" + to_string(userId));
    vector<User*> friends;
    for (int friendId : getNeighbors(userId)) {
        if (auto* u = getUser(friendId))
            friends.push_back(u);
    }
    LOG_DEBUG("User ID=" + to_string(userId) + " has " + to_string(friends.size()) + " friends");
    return friends;
}

/**
 * @brief Finds mutual friends between two users.
 * @param userA First user's ID.
 * @param userB Second user's ID.
 * @return Vector of User pointers representing mutual friends.
 */
vector<User*> SocialNetwork::findMutualFriends(int userA, int userB) {
    LOG_INFO("Finding mutual friends between " + to_string(userA) + " and " + to_string(userB));
    auto friendsA = getFriendsOfUser(userA);
    auto friendsB = getFriendsOfUser(userB);
    vector<User*> mutual;
    for (auto* ua : friendsA)
        for (auto* ub : friendsB)
            if (ua->getId() == ub->getId())
                mutual.push_back(ua);
    LOG_DEBUG("Mutual friends found: " + to_string(mutual.size()));
    return mutual;
}

/**
 * @brief Finds close friends (friends of friends) for a user.
 * @param userId ID of the user.
 * @return Vector of User pointers representing close friends.
 */
vector<User*> SocialNetwork::findCloseFriends(int userId) {
    LOG_INFO("Finding close friends for user ID=" + to_string(userId));
    set<int> closeSet;
    auto friends = getFriendsOfUser(userId);
    for (auto* f : friends) {
        for (auto* ff : getFriendsOfUser(f->getId())) {
            if (ff->getId() != userId)
                closeSet.insert(ff->getId());
        }
    }
    vector<User*> result;
    for (int id : closeSet)
        if (auto* u = getUser(id))
            result.push_back(u);
    LOG_DEBUG("Close friends found: " + to_string(result.size()));
    return result;
}

/**
 * @brief Finds users located in the specified location.
 * @param location City or region.
 * @return Vector of User pointers located in the specified area.
 */
vector<User*> SocialNetwork::findUsersByLocation(const string& location) {
    LOG_INFO("Searching users by location: " + location);
    vector<User*> result;
    for (auto* v : getAllVertices()) {
        if (auto* u = dynamic_cast<User*>(v)) {
            if (u->getLocation() == location)
                result.push_back(u);
        }
    }
    LOG_DEBUG("Users found in " + location + ": " + to_string(result.size()));
    return result;
}

/**
 * @brief Finds users followed by both given users.
 * @param userA First user's ID.
 * @param userB Second user's ID.
 * @return Vector of User pointers representing shared subscriptions.
 */
vector<User*> SocialNetwork::findCommonSubscriptions(int userA, int userB) {
    LOG_INFO("Finding common subscriptions between " + to_string(userA) + " and " + to_string(userB));
    set<int> subsA, subsB;
    for (auto* e : getAllEdges()) {
        if (auto* s = dynamic_cast<Subscription*>(e)) {
            if (s->getFrom() == userA) subsA.insert(s->getTo());
            if (s->getFrom() == userB) subsB.insert(s->getTo());
        }
    }
    vector<User*> res;
    for (int id : subsA)
        if (subsB.count(id) && getUser(id))
            res.push_back(getUser(id));
    LOG_DEBUG("Common subscriptions found: " + to_string(res.size()));
    return res;
}

/**
 * @brief Checks whether two users are connected by any path in the network.
 * @param userA First user's ID.
 * @param userB Second user's ID.
 * @return True if connected, otherwise false.
 */
bool SocialNetwork::areConnected(int userA, int userB) {
    LOG_INFO("Checking if users " + to_string(userA) + " and " + to_string(userB) + " are connected");
    vector<pair<int, int>> edgesList;
    for (auto* e : getAllEdges())
        edgesList.push_back({ e->getFrom(), e->getTo() });
    buildGraph(edgesList);
    bool connected = GraphAlgorithms::hasPath(userA, userB);
    LOG_DEBUG("Users " + to_string(userA) + " and " + to_string(userB) +
        (connected ? " are connected" : " are NOT connected"));
    return connected;
}

/**
 * @brief Calculates the distance (number of edges) between two users.
 * @param userA First user's ID.
 * @param userB Second user's ID.
 * @return Distance or -1 if no path exists.
 */
int SocialNetwork::distanceBetween(int userA, int userB) {
    LOG_INFO("Calculating distance between " + to_string(userA) + " and " + to_string(userB));
    auto dist = GraphAlgorithms::breadthFirstSearch(userA);
    int result = dist.count(userB) ? dist[userB] : -1;
    LOG_DEBUG("Distance result: " + to_string(result));
    return result;
}

/**
 * @brief Computes the shortest paths from a given user.
 * @param startId ID of the starting user.
 * @return Map of user IDs to shortest path distances.
 */
map<int, int> SocialNetwork::shortestPathsFrom(int startId) {
    LOG_INFO("Computing shortest paths from user ID=" + to_string(startId));
    vector<pair<int, int>> edgesList;
    for (auto* e : getAllEdges())
        edgesList.push_back({ e->getFrom(), e->getTo() });
    buildGraph(edgesList);
    return GraphAlgorithms::dijkstra(startId);
}

/**
 * @brief Computes degree centrality for all users.
 * @return Map of user IDs and centrality scores.
 */
map<int, double> SocialNetwork::userCentrality() {
    LOG_INFO("Computing user centrality for network");
    vector<pair<int, int>> edgesList;
    for (auto* e : getAllEdges())
        edgesList.push_back({ e->getFrom(), e->getTo() });
    buildGraph(edgesList);
    return GraphAlgorithms::computeDegreeCentrality();
}

/**
 * @brief Detects small friend groups (triangles) within the network.
 * @return Vector of triangles (groups of three user IDs).
 */
vector<vector<int>> SocialNetwork::detectFriendGroups() {
    LOG_INFO("Detecting friend groups (triangles)");
    vector<pair<int, int>> edgesList
