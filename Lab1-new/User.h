#ifndef USER_H
#define USER_H

#include "Graph.h"
#include <string>
#include <ctime>
#include <vector>

class User : public Vertex {
protected:
    string name;
    string email;
    string biography;
    string location;
    string phone;
    string birthday;
    string gender;

public:
    User(int id, const string& n, const string& e);

    virtual void updateBio(const string& b);
    void setBirthday(const string& bday);
    void setPhone(const string& ph);
    void setGender(const string& g);
    void updateLocation(const string& loc);

    string getLocation() const { return location; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getBio() const { return biography; }

    virtual void print() const;
};

class RegularUser : public User {
protected:
    int reputation;
    int followers;
    int following;
    int postsCount;
    int messagesSent;
    int messagesReceived;
    time_t lastLogin;

public:
    RegularUser(int id, const string& n, const string& e);

    virtual void addFollower();
    virtual void addFollowing();
    virtual void addPost();
    virtual void sendMessage();
    virtual void receiveMessage();
    virtual void changeReputation(int delta);

    void updateLastLogin();
    time_t getLastLogin() const;

    void print() const override;
};

class PremiumUser : public RegularUser {
    int premiumPoints;
    vector<string> checkmark;
public:
    PremiumUser(int id, const string& n, const string& e, int points = 100);

    void addCheckmark(const string& checkmark);
    void spendPremiumPoints(int amount);
    void bonusReputation();

    void print() const;
};


class Friendship : public Edge {
public:
    Friendship(int f, int t);
    void print() const override;
};

class Subscription : public Edge {
public:
    Subscription(int f, int t);
    void print() const override;
};

class Message : public Edge {
    string text;
public:
    Message(int f, int t, string msg);
    void print() const override;
    const string& getText() const { return text; }
};

class Post : public Edge {
    string content;
public:
    Post(int f, string c);
    void print() const override;
    const string& getContent() const { return content; }
};

#endif // USER_H