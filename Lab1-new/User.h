/**
 * @file User.h
 * @brief Defines user-related classes and relationship types for the social network.
 * @details Contains class definitions for User, RegularUser, PremiumUser, and
 * relationship edges such as Friendship, Subscription, Message, and Post.
 * These classes extend graph components to represent people and their interactions.
 *
 * @see Graph
 * @see Vertex
 * @see Edge
 *
 * @date 04.11.2025
 * @version 1.0
 * @autor Kristina Zakharchenko
 */

#ifndef USER_H
#define USER_H

#include "Graph.h"
#include <string>
#include <ctime>
#include <vector>

 /**
  * @brief Represents a generic user in the social network.
  * @details Stores user profile data and provides methods for updating it.
  */
class User : public Vertex {
protected:
    string name;       ///< User's display name.
    string email;      ///< User's email address.
    string biography;  ///< User bio or description.
    string location;   ///< Current location.
    string phone;      ///< Contact phone number.
    string birthday;   ///< Birthday date.
    string gender;     ///< Gender of the user.

public:
    /**
     * @brief Constructs a User object.
     * @param id User ID.
     * @param n User name.
     * @param e User email.
     */
    User(int id, const string& n, const string& e);

    /**
     * @brief Updates the user's biography.
     * @param b New biography text.
     */
    virtual void updateBio(const string& b);

    /**
     * @brief Sets the user's birthday.
     * @param bday Birthday string in YYYY-MM-DD format.
     */
    void setBirthday(const string& bday);

    /**
     * @brief Sets the user's phone number.
     * @param ph Phone number.
     */
    void setPhone(const string& ph);

    /**
     * @brief Sets the user's gender.
     * @param g Gender string.
     */
    void setGender(const string& g);

    /**
     * @brief Updates the user's location.
     * @param loc Location string.
     */
    void updateLocation(const string& loc);

    string getLocation() const { return location; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getBio() const { return biography; }
    string getPhone() const { return phone; }
    string getBirthday() const { return birthday; }
    string getGender() const { return gender; }

    /**
     * @brief Prints detailed information about the user.
     */
    virtual void print() const;
};

/**
 * @brief Represents a standard (non-premium) user with activity tracking.
 * @details Includes additional fields for statistics such as followers,
 * following count, posts, messages, and reputation.
 */
class RegularUser : public User {
protected:
    int reputation;        ///< User reputation score.
    int followers;         ///< Number of followers.
    int following;         ///< Number of accounts followed.
    int postsCount;        ///< Number of posts created.
    int messagesSent;      ///< Messages sent count.
    int messagesReceived;  ///< Messages received count.
    time_t lastLogin;      ///< Timestamp of the last login.

public:
    /**
     * @brief Constructs a RegularUser object.
     * @param id User ID.
     * @param n User name.
     * @param e User email.
     */
    RegularUser(int id, const string& n, const string& e);

    /** @brief Increments the number of followers. */
    virtual void addFollower();

    /** @brief Increments the number of followed users. */
    virtual void addFollowing();

    /** @brief Increments the post count. */
    virtual void addPost();

    /** @brief Increments the sent message counter. */
    virtual void sendMessage();

    /** @brief Increments the received message counter. */
    virtual void receiveMessage();

    /**
     * @brief Adjusts the user's reputation.
     * @param delta Change amount (positive or negative).
     */
    virtual void changeReputation(int delta);

    /**
     * @brief Updates the last login timestamp.
     */
    int getReputation() const { return reputation; }
    int getPostsCount() const { return postsCount; }
    int getMessagesSent() const { return messagesSent; }
    int getMessagesReceived() const { return messagesReceived; }
    int getFollowers() const { return followers; }
    int getFollowing() const { return following; }

    void updateLastLogin();

    /**
     * @brief Retrieves the last login time.
     * @return Time in UNIX format.
     */
    time_t getLastLogin() const;

    /**
     * @brief Prints user profile and activity statistics.
     */
    void print() const override;
};

/**
 * @brief Represents a premium user with special privileges and rewards.
 */
class PremiumUser : public RegularUser {
    int premiumPoints;              ///< Accumulated premium points.
    vector<string> checkmark;       ///< Collection of special checkmarks.
public:
    /**
     * @brief Constructs a PremiumUser.
     * @param id User ID.
     * @param n Name.
     * @param e Email.
     * @param points Initial premium points (default = 100).
     */
    PremiumUser(int id, const string& n, const string& e, int points = 100);

    /**
     * @brief Adds a verification checkmark.
     * @param checkmark Label to add.
     */
    void addCheckmark(const string& checkmark);

    /**
     * @brief Spends premium points.
     * @param amount Number of points to spend.
     */
    void spendPremiumPoints(int amount);

    /**
     * @brief Grants bonus reputation and points.
     */
    void bonusReputation();

    /**
     * @brief Prints premium user details.
     */

    int getPremiumPoints() const { return premiumPoints; }
    vector<string> getCheckmarks() const { return checkmark; }

    void print() const;
};

/**
 * @brief Represents a friendship relationship between two users.
 */
class Friendship : public Edge {
public:
    /**
     * @brief Constructs a friendship edge.
     * @param f Source user ID.
     * @param t Target user ID.
     */
    Friendship(int f, int t);

    /**
     * @brief Prints friendship info.
     */
    void print() const override;
};

/**
 * @brief Represents a one-directional subscription (follow).
 */
class Subscription : public Edge {
public:
    /**
     * @brief Constructs a subscription edge.
     * @param f Follower ID.
     * @param t Followee ID.
     */
    Subscription(int f, int t);

    /**
     * @brief Prints subscription info.
     */
    void print() const override;
};

/**
 * @brief Represents a message sent between two users.
 */
class Message : public Edge {
    string text; ///< Message content.
public:
    /**
     * @brief Constructs a message edge.
     * @param f Sender ID.
     * @param t Receiver ID.
     * @param msg Message text.
     */
    Message(int f, int t, string msg);

    /**
     * @brief Prints message details.
     */
    void print() const override;

    /**
     * @brief Retrieves the message text.
     * @return Text content.
     */
    const string& getText() const { return text; }
};

/**
 * @brief Represents a user post.
 */
class Post : public Edge {
    string content; ///< Post content.
public:
    /**
     * @brief Constructs a post edge.
     * @param f Author ID.
     * @param c Post content.
     */
    Post(int f, string c);

    /**
     * @brief Prints post details.
     */
    void print() const override;

    /**
     * @brief Retrieves post content.
     * @return Content text.
     */
    const string& getContent() const { return content; }
};

#endif // USER_H
