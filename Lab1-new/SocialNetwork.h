/**
 * @file SocialNetwork.h
 * @brief Definition of the SocialNetwork class that models a social graph.
 * @details Combines graph and algorithmic components to represent and manage
 * a network of users, friendships, subscriptions, messages, and posts.
 * Provides high-level functionality for social interactions, analytics,
 * and exporting data for visualization or persistence.
 *
 * @see Graph
 * @see GraphAlgorithms
 * @see User
 *
 * @date 04.11.2025
 * @version 1.0
 * @author
 * Kristina Zakharchenko
 */

#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H

#include "Graph.h"
#include "User.h"
#include "GraphAlgorithms.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

/**
 * @brief Represents a complete social network system.
 * @details Extends the functionality of Graph and GraphAlgorithms
 * to include user management, relationships, content, and analysis.
 */
class SocialNetwork : public Graph, public GraphAlgorithms {
private:
    map<int, User*> users;

    /**
     * @brief Adds a new user to the network.
     * @param user Pointer to the User object.
     */
public:

    void addUser(User* user);

    /**
     * @brief Removes a user and all their connections.
     * @param userId Unique identifier of the user to remove.
     */
    void removeUser(int userId);

    /**
     * @brief Retrieves a user by ID.
     * @param userId User identifier.
     * @return Pointer to the User or nullptr if not found.
     */
    User* getUser(int userId) const;

    /**
     * @brief Creates a friendship between two users.
     * @param userA First user's ID.
     * @param userB Second user's ID.
     */
    void addFriendship(int userA, int userB);

    /**
     * @brief Removes a friendship between two users.
     * @param userA First user's ID.
     * @param userB Second user's ID.
     */
    void removeFriendship(int userA, int userB);

    /**
     * @brief Adds a one-way subscription (follow) between users.
     * @param followerId ID of the user following another.
     * @param followeeId ID of the user being followed.
     */
    void addSubscription(int followerId, int followeeId);

    /**
     * @brief Sends a message from one user to another.
     * @param senderId Sender's ID.
     * @param receiverId Receiver's ID.
     * @param text Message text.
     */
    void sendMessage(int senderId, int receiverId, const string& text);

    /**
     * @brief Adds a post created by a user.
     * @param authorId Author's ID.
     * @param content Content of the post.
     */
    void addPost(int authorId, const string& content);

    /**
     * @brief Retrieves the friends of a given user.
     * @param userId User identifier.
     * @return Vector of User pointers representing friends.
     */
    vector<User*> getFriendsOfUser(int userId);

    /**
     * @brief Finds mutual friends between two users.
     * @param userA First user's ID.
     * @param userB Second user's ID.
     * @return Vector of User pointers representing mutual friends.
     */
    vector<User*> findMutualFriends(int userA, int userB);

    /**
     * @brief Finds friends of friends (close friends) for a given user.
     * @param userId User identifier.
     * @return Vector of User pointers representing close friends.
     */
    vector<User*> findCloseFriends(int userId);

    /**
     * @brief Finds users located in a specific city or region.
     * @param location Location string.
     * @return Vector of User pointers located in the specified place.
     */
    vector<User*> findUsersByLocation(const string& location);

    /**
     * @brief Finds users followed by both given users.
     * @param userA First user's ID.
     * @param userB Second user's ID.
     * @return Vector of User pointers representing common subscriptions.
     */
    vector<User*> findCommonSubscriptions(int userA, int userB);
    vector<Message*> getMessagesOfUser(int userId) const;
    vector<Post*> getPostsOfUser(int userId) const;

    /**
     * @brief Checks whether two users are connected by any path.
     * @param userA Source user ID.
     * @param userB Target user ID.
     * @return True if connected, otherwise false.
     */
    bool areConnected(int userA, int userB);

    /**
     * @brief Calculates the shortest distance between two users.
     * @param userA First user's ID.
     * @param userB Second user's ID.
     * @return Distance in number of edges, or -1 if not reachable.
     */
    int distanceBetween(int userA, int userB);

    /**
     * @brief Computes shortest paths from a starting user using Dijkstra�s algorithm.
     * @param startId Starting user's ID.
     * @return Map of user IDs and their shortest distances.
     */
    map<int, int> shortestPathsFrom(int startId);

    /**
     * @brief Calculates degree centrality for each user in the network.
     * @return Map of user IDs to their centrality scores.
     */
    map<int, double> userCentrality();

    /**
     * @brief Detects small friend groups (triangles) within the network.
     * @return Vector of user ID triplets representing friend groups.
     */
    vector<vector<int>> detectFriendGroups();

    /**
     * @brief Generates a random set of users and optionally their relations.
     * @param network Reference to a SocialNetwork instance.
     * @param n Number of users to generate.
     * @param withRelations Whether to include friendships and other relations.
     */
    static void generateRandomUsers(SocialNetwork& network, int n, bool withRelations = true);

    /**
     * @brief Saves user and relationship data to a text file.
     * @param filename File name for saving.
     */
    void saveToTextFile(const string& filename) const;

    /**
     * @brief Exports the network to DOT format for visualization.
     * @param filename File name for export.
     */
    void exportToDot(const string& filename) const;

    /**
     * @brief Prints all users and relationships to the console.
     */
    void printNetwork() const;

    /**
     * @brief Displays statistical information about the network.
     */
    void printStatistics();
};

#endif // SOCIALNETWORK_H
