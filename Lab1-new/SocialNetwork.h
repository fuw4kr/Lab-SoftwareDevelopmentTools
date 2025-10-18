#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H

#include "Graph.h"
#include "User.h"
#include "GraphAlgorithms.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class SocialNetwork : public Graph, public GraphAlgorithms {
private:
    map<int, User*> users;

public:
    void addUser(User* user);
    void removeUser(int userId);
    User* getUser(int userId) const;

    void addFriendship(int userA, int userB);
    void removeFriendship(int userA, int userB);

    void addSubscription(int followerId, int followeeId);
    void sendMessage(int senderId, int receiverId, const string& text);
    void addPost(int authorId, const string& content);

    vector<User*> getFriendsOfUser(int userId);
    vector<User*> findMutualFriends(int userA, int userB);
    vector<User*> findCloseFriends(int userId);
    vector<User*> findUsersByLocation(const string& location);
    vector<User*> findCommonSubscriptions(int userA, int userB);

    bool areConnected(int userA, int userB);
    int distanceBetween(int userA, int userB);
    map<int, int> shortestPathsFrom(int startId);
    map<int, double> userCentrality();
    vector<vector<int>> detectFriendGroups();

    static void generateRandomUsers(SocialNetwork& network, int n, bool withRelations = true);

    void saveToTextFile(const string& filename) const;
    void exportToDot(const string& filename) const;

    void printNetwork() const;
    void printStatistics();
};

#endif // SOCIALNETWORK_H