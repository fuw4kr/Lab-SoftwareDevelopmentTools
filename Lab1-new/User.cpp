#include "User.h"
#include <iostream>
using namespace std;

User::User(int id, const string& n, const string& e)
    : Vertex(id), name(n), email(e), biography(""), location(""), phone(""),
    birthday(""), gender("") {
}

void User::updateBio(const string& b) { biography = b; }
void User::setBirthday(const string& bday) { birthday = bday; }
void User::setPhone(const string& ph) { phone = ph; }
void User::setGender(const string& g) { gender = g; }
void User::updateLocation(const string& loc) { location = loc; }

void User::print() const {
    cout << "[User Profile]\n"
        << "Name: " << name << "\n"
        << "Email: " << email << "\n"
        << "Bio: " << biography << "\n"
        << "Location: " << location << "\n"
        << "Phone: " << phone << "\n"
        << "Gender: " << gender << "\n"
        << "Birthday: " << birthday << "\n";
}

RegularUser::RegularUser(int id, const string& n, const string& e)
    : User(id, n, e), reputation(0), followers(0), following(0),
    postsCount(0), messagesSent(0), messagesReceived(0), lastLogin(time(0)) {
}

void RegularUser::addFollower() { ++followers; }
void RegularUser::addFollowing() { ++following; }
void RegularUser::addPost() { ++postsCount; }
void RegularUser::sendMessage() { ++messagesSent; }
void RegularUser::receiveMessage() { ++messagesReceived; }
void RegularUser::changeReputation(int delta) { reputation += delta; }

void RegularUser::updateLastLogin() { lastLogin = time(nullptr); }
time_t RegularUser::getLastLogin() const { return lastLogin; }

void RegularUser::print() const {
    cout << "[RegularUser]\n"
        << "Name: " << name << "\n"
        << "Email: " << email << "\n"
        << "Reputation: " << reputation << "\n"
        << "Followers: " << followers << "\n"
        << "Following: " << following << "\n"
        << "Posts: " << postsCount << "\n"
        << "Messages sent: " << messagesSent << ", received: " << messagesReceived << "\n";
}

PremiumUser::PremiumUser(int id, const string& n, const string& e, int points)
    : RegularUser(id, n, e), premiumPoints(points) {
}

void PremiumUser::addCheckmark(const string& ch) {
    checkmark.push_back(ch);
}

void PremiumUser::spendPremiumPoints(int amount) {
    if (premiumPoints >= amount)
        premiumPoints -= amount;
    else
        cout << "Not enough premium points!\n";
}

void PremiumUser::bonusReputation() {
    reputation += 10;
    premiumPoints += 5;
}

void PremiumUser::print() const {
    cout << "[PremiumUser]\n"
        << "Name: " << name << "\n"
        << "Email: " << email << "\n"
        << "Reputation: " << reputation << " (+ бонуси)\n"
        << "Premium points: " << premiumPoints << "\n"
        << "Checkmark: ";
    for (auto& b : checkmark) cout << b << " ";
    cout << "\n";
}

Friendship::Friendship(int f, int t) : Edge(f, t) {}
void Friendship::print() const { cout << "Friendship: " << from << " <-> " << to << endl; }

Subscription::Subscription(int f, int t) : Edge(f, t) {}
void Subscription::print() const { cout << "Subscription: " << from << " -> " << to << endl; }

Message::Message(int f, int t, string msg) : Edge(f, t), text(msg) {}
void Message::print() const { cout << "Message: " << from << " -> " << to << " : " << text << endl; }

Post::Post(int f, string c) : Edge(f, -1), content(c) {}
void Post::print() const { cout << "Post: " << from << " : " << content << endl; }