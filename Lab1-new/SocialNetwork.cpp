#include "SocialNetwork.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>    
#include <ctime>
using namespace std;

void SocialNetwork::addUser(User* user) {
    if (!user) {
        LOG_ERROR("Attempted to add null user");
        return;
    }
    addVertex(user);
    LOG_INFO("Added user ID=" + to_string(user->getId()) + " name=" + user->getName());
}

void SocialNetwork::removeUser(int userId) {
    LOG_INFO("Removing user ID=" + to_string(userId));
    removeVertex(userId);
}

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

void SocialNetwork::addFriendship(int userA, int userB) {
    if (!getUser(userA) || !getUser(userB)) {
        LOG_ERROR("Invalid friendship IDs: " + to_string(userA) + ", " + to_string(userB));
        return;
    }
    addEdge(new Friendship(userA, userB));
    addEdge(new Friendship(userB, userA));
    LOG_INFO("Created friendship between " + to_string(userA) + " and " + to_string(userB));
}

void SocialNetwork::removeFriendship(int userA, int userB) {
    LOG_INFO("Removing friendship between users " + to_string(userA) + " and " + to_string(userB));
    removeEdge(userA, userB);
    removeEdge(userB, userA);
}

void SocialNetwork::addSubscription(int followerId, int followeeId) {
    LOG_INFO("Adding subscription: " + to_string(followerId) + " -> " + to_string(followeeId));
    addEdge(new Subscription(followerId, followeeId));

    if (auto* f = dynamic_cast<RegularUser*>(getUser(followerId)))
        f->addFollowing();
    if (auto* fe = dynamic_cast<RegularUser*>(getUser(followeeId)))
        fe->addFollower();
}

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

int SocialNetwork::distanceBetween(int userA, int userB) {
    LOG_INFO("Calculating distance between " + to_string(userA) + " and " + to_string(userB));
    auto dist = GraphAlgorithms::breadthFirstSearch(userA);
    int result = dist.count(userB) ? dist[userB] : -1;
    LOG_DEBUG("Distance result: " + to_string(result));
    return result;
}

map<int, int> SocialNetwork::shortestPathsFrom(int startId) {
    LOG_INFO("Computing shortest paths from user ID=" + to_string(startId));
    vector<pair<int, int>> edgesList;
    for (auto* e : getAllEdges())
        edgesList.push_back({ e->getFrom(), e->getTo() });
    buildGraph(edgesList);
    return GraphAlgorithms::dijkstra(startId);
}

map<int, double> SocialNetwork::userCentrality() {
    LOG_INFO("Computing user centrality for network");
    vector<pair<int, int>> edgesList;
    for (auto* e : getAllEdges())
        edgesList.push_back({ e->getFrom(), e->getTo() });
    buildGraph(edgesList);
    return GraphAlgorithms::computeDegreeCentrality();
}

vector<vector<int>> SocialNetwork::detectFriendGroups() {
    LOG_INFO("Detecting friend groups (triangles)");
    vector<pair<int, int>> edgesList;
    for (auto* e : getAllEdges())
        edgesList.push_back({ e->getFrom(), e->getTo() });
    buildGraph(edgesList);
    auto result = GraphAlgorithms::findTriangles();
    LOG_DEBUG("Detected " + to_string(result.size()) + " friend groups");
    return result;
}

void SocialNetwork::generateRandomUsers(SocialNetwork& network, int n, bool withRelations) {
    LOG_INFO("Generating " + to_string(n) + " random users");
    srand(static_cast<unsigned>(time(0)));

    vector<string> names = {
        "Alice", "Bob", "Sophia", "Diana", "Eve", "James", "Mia",
        "Robert", "Ivan", "Judy", "Kevin", "Allison", "Liza", "Nina", "Gregory"
    };

    vector<string> locations = {
        "Kyiv", "Lviv", "Odesa", "Kharkiv", "Dnipro", "Vinnytsia", "Sumy"
    };

    for (int i = 0; i < n; ++i) {
        string name = names[rand() % names.size()] + to_string(i + 1);
        string email = name + "@mail.com";
        User* u = new User(i, name, email);
        u->updateLocation(locations[rand() % locations.size()]);
        u->setGender((rand() % 2 == 0) ? "Male" : "Female");
        u->setBirthday("199" + to_string(rand() % 10) + "-0" + to_string(rand() % 9 + 1) + "-1" + to_string(rand() % 9));
        network.addVertex(u);
    }

    if (withRelations) {
        for (int i = 0; i < n * 1.5; ++i) {
            int u1 = rand() % n;
            int u2 = rand() % n;
            int u3 = rand() % n;
            int u4 = rand() % n;
            int u5 = rand() % n;
            int u6 = rand() % n;

            if (u1 != u2) network.addEdge(new Friendship(u1, u2));
            if (u3 != u4) network.addEdge(new Subscription(u3, u4));
            network.addEdge(new Post(rand() % n, "post"));
            if (u5 != u6) network.addEdge(new Message(u5, u6, "message"));
        }
    }

    LOG_INFO(to_string(n) + " random users created successfully");
}

void SocialNetwork::saveToTextFile(const string& filename) const {
    LOG_INFO("Saving social network data to file: " + filename);
    ofstream file(filename);
    if (!file.is_open()) {
        LOG_ERROR("Cannot open file for writing: " + filename);
        return;
    }

    file << "USERS\n";
    for (auto* v : getAllVertices()) {
        auto* u = dynamic_cast<RegularUser*>(v);
        if (u) {
            file << "User ID: " << u->getId()
                << ", Name: " << u->getName()
                << ", Email: " << u->getEmail()
                << ", Bio: " << u->getBio()
                << ", Location: " << u->getLocation()
                << "\n";
        }
    }

    file << "\nRELATIONSHIPS\n";
    for (auto* e : getAllEdges()) {
        if (auto* f = dynamic_cast<Friendship*>(e)) {
            file << "Friendship: " << f->getFrom() << " <-> " << f->getTo() << "\n";
        }
        else if (auto* s = dynamic_cast<Subscription*>(e)) {
            file << "Subscription: " << s->getFrom() << " -> " << s->getTo() << "\n";
        }
        else if (auto* m = dynamic_cast<Message*>(e)) {
            file << "Message: " << m->getFrom() << " -> " << m->getTo()
                << " : " << m->getText() << "\n";
        }
        else if (auto* p = dynamic_cast<Post*>(e)) {
            file << "Post by User " << p->getFrom() << ": " << p->getContent() << "\n";
        }
    }

    file.close();
    LOG_INFO("Social network data saved successfully to " + filename);
}

void SocialNetwork::exportToDot(const string& filename) const {
    LOG_INFO("Exporting graph to DOT file: " + filename);
    exportToDotGraph(filename);
}

void SocialNetwork::printNetwork() const {
    LOG_DEBUG("Printing entire social network");
    cout << "USERS" << endl;
    for (auto* v : getAllVertices()) {
        v->print();
    }
    cout << "FRIENDSHIPS" << endl;
    for (auto* e : getAllEdges()) {
        e->print();
    }
}

void SocialNetwork::printStatistics() {
    LOG_INFO("Collecting template-based network statistics");

    cout << "\nNETWORK STATISTICS\n";

    auto allUsers = getAllVertices();
    auto allEdges = getAllEdges();

    LOG_DEBUG("Counting users by type");
    int regularCount = getVerticesOfType<RegularUser>().size();
    int premiumCount = getVerticesOfType<PremiumUser>().size();

    LOG_DEBUG("Counting edges by type");
    int friendships = countType<Friendship>(allEdges);
    int subs = countType<Subscription>(allEdges);
    int messages = countType<Message>(allEdges);
    int posts = countType<Post>(allEdges);

    cout << "Users total: " << allUsers.size() << endl;
    cout << "Regular users: " << regularCount << endl;
    cout << "Premium users: " << premiumCount << endl;

    cout << "\nConnections total: " << allEdges.size() << endl;
    cout << "Friendships: " << friendships << endl;
    cout << "Subscriptions: " << subs << endl;
    cout << "Messages: " << messages << endl;
    cout << "Posts: " << posts << endl;

    LOG_DEBUG("Printing vertices using forEachVertex template");
    forEachVertex([](Vertex* v) {
        if (auto* u = dynamic_cast<User*>(v)) {
            LOG_DEBUG("User: " + u->getName());
        }
        });
    LOG_INFO("Network statistics completed successfully");
}
