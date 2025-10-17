#include "Menu.h"
#include "Logger.h"
#include "Graph.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

void runMenu(SocialNetwork& net) {
    int choice;
    do {
        cout << "\n--- MENU ---\n1. Add user\n2. Add friendship\n3. Add subscription\n4. Send message\n5. Add post"
            << "\n6. Show info\n7. Update bio\n8. Set birthday\n9. Set phone\n10. Set gender"
            << "\n11. Update location\n12. Add follower\n13. Add following\n14. Change reputation\n15. Find common friends"
            << "\n16. Find close friends\n17. Find common subscriptions\n18. Find users nearby (same location)"
            << "\n19. Recommend users you may know\n20. Show most central users\n21. Check friendship cycles"
            << "\n22. Update last login time\n23. Generate random users\n24. Export graph to DOT format"
            << "\n25. Save social network info to text file\n26. View users by role\n27. View relationships by type"
            << "\n28. Network overview(template walk)"
            << "\n0. Exit\nChoice: ";

        cin >> choice;
        LOG_INFO("Menu choice selected: " + to_string(choice));

        switch (choice) {
        case 1: {
            LOG_INFO("User selected: Add user");
            int id; string name, email;
            int type;
            cout << "ID: "; cin >> id; cin.ignore();
            cout << "Name: "; getline(cin, name);
            cout << "Email: "; getline(cin, email);
            cout << "Select user type (1-Regular, 2-Premium): "; cin >> type;

            if (type == 2)
                net.addUser(new PremiumUser(id, name, email));
            else
                net.addUser(new RegularUser(id, name, email));

            LOG_INFO("Added user ID=" + to_string(id) + " (" + name + ")");
            break;
        }

        case 2: {
            LOG_INFO("User selected: Add friendship");
            int a, b; cout << "User1 ID: "; cin >> a; cout << "User2 ID: "; cin >> b;
            net.addFriendship(a, b);
            break;
        }

        case 3: {
            LOG_INFO("User selected: Add subscription");
            int a, b; cout << "Follower ID: "; cin >> a; cout << "Followee ID: "; cin >> b;
            net.addSubscription(a, b);
            break;
        }

        case 4: {
            LOG_INFO("User selected: Send message");
            int a, b; string msg;
            cout << "Sender ID: "; cin >> a;
            cout << "Receiver ID: "; cin >> b;
            cout << "Message: "; cin.ignore(); getline(cin, msg);
            net.sendMessage(a, b, msg);
            LOG_DEBUG("Message from " + to_string(a) + " to " + to_string(b));
            break;
        }

        case 5: {
            LOG_INFO("User selected: Add post");
            int a; string text;
            cout << "Author ID: "; cin >> a;
            cout << "Post content: "; cin.ignore(); getline(cin, text);
            net.addPost(a, text);
            break;
        }

        case 6: {
            LOG_INFO("User selected: Show full network info");
            net.printNetwork();
            break;
        }

        case 7: {
            LOG_DEBUG("User selected: Update bio");
            int id; string bio;
            cout << "User ID: "; cin >> id; cin.ignore();
            cout << "New bio: "; getline(cin, bio);
            User* u = net.getUser(id);
            if (u) { u->updateBio(bio); LOG_INFO("Bio updated for user ID=" + to_string(id)); }
            else LOG_WARN("Attempt to update bio for non-existent user ID=" + to_string(id));
            break;
        }

        case 8: {
            LOG_DEBUG("User selected: Set birthday");
            int id; string bday;
            cout << "User ID: "; cin >> id;
            cout << "Birthday: "; cin >> bday;
            User* u = net.getUser(id);
            if (u) { u->setBirthday(bday); LOG_INFO("Birthday set for user ID=" + to_string(id)); }
            else LOG_WARN("User not found for birthday update: ID=" + to_string(id));
            break;
        }

        case 9: {
            LOG_DEBUG("User selected: Set phone");
            int id; string phone;
            cout << "User ID: "; cin >> id;
            cout << "Phone: "; cin >> phone;
            User* u = net.getUser(id);
            if (u) { u->setPhone(phone); LOG_INFO("Phone updated for user ID=" + to_string(id)); }
            else LOG_WARN("User not found for phone update: ID=" + to_string(id));
            break;
        }

        case 10: {
            LOG_DEBUG("User selected: Set gender");
            int id; string g;
            cout << "User ID: "; cin >> id;
            cout << "Gender: "; cin >> g;
            User* u = net.getUser(id);
            if (u) { u->setGender(g); LOG_INFO("Gender updated for user ID=" + to_string(id)); }
            else LOG_WARN("User not found for gender update: ID=" + to_string(id));
            break;
        }

        case 11: {
            LOG_DEBUG("User selected: Update location");
            int id; string loc;
            cout << "User ID: "; cin >> id; cin.ignore();
            cout << "Location: "; getline(cin, loc);
            User* u = net.getUser(id);
            if (u) { u->updateLocation(loc); LOG_INFO("Location updated for user ID=" + to_string(id)); }
            else LOG_WARN("User not found for location update: ID=" + to_string(id));
            break;
        }

        case 12: {
            LOG_DEBUG("User selected: Add follower");
            int id; cout << "User ID: "; cin >> id;
            if (auto* u = dynamic_cast<RegularUser*>(net.getUser(id))) {
                u->addFollower();
                LOG_DEBUG("Follower added for user ID=" + to_string(id));
            }
            else LOG_WARN("User not found or not RegularUser: ID=" + to_string(id));
            break;
        }

        case 13: {
            LOG_DEBUG("User selected: Add following");
            int id; cout << "User ID: "; cin >> id;
            if (auto* u = dynamic_cast<RegularUser*>(net.getUser(id))) {
                u->addFollowing();
                LOG_DEBUG("Following added for user ID=" + to_string(id));
            }
            else LOG_WARN("User not found or not RegularUser: ID=" + to_string(id));
            break;
        }

        case 14: {
            LOG_DEBUG("User selected: Change reputation");
            int id, delta;
            cout << "User ID: "; cin >> id;
            cout << "Change reputation by: "; cin >> delta;
            if (auto* u = dynamic_cast<RegularUser*>(net.getUser(id))) {
                u->changeReputation(delta);
                LOG_INFO("Reputation changed for user ID=" + to_string(id));
            }
            else LOG_WARN("User not found or not RegularUser: ID=" + to_string(id));
            break;
        }

        case 15: {
            LOG_INFO("User selected: Find common friends");
            int id1, id2;
            cout << "User 1 ID: "; cin >> id1;
            cout << "User 2 ID: "; cin >> id2;
            auto common = net.findMutualFriends(id1, id2);
            if (common.empty()) LOG_INFO("No common friends between " + to_string(id1) + " and " + to_string(id2));
            else {
                for (auto& u : common)  u->print();
                LOG_INFO("Found " + to_string(common.size()) + " common friends");
            }
            break;
        }

        case 16: {
            LOG_INFO("User selected: Find close friends");
            int id; cout << "User ID: "; cin >> id;
            auto close = net.findCloseFriends(id);
            if (close.empty()) LOG_INFO("No close friends found for ID=" + to_string(id));
            else {
                for (auto& c : close) c->print();
                LOG_DEBUG("Displayed close friends for ID=" + to_string(id));
            }
            break;
        }

        case 17: {
            LOG_INFO("User selected: Find common subscriptions");
            int id1, id2;
            cout << "User 1 ID: "; cin >> id1;
            cout << "User 2 ID: "; cin >> id2;
            auto commonSubs = net.findCommonSubscriptions(id1, id2);
            if (commonSubs.empty()) LOG_INFO("No common subscriptions between " + to_string(id1) + " and " + to_string(id2));
            else {
                for (auto& u : commonSubs) u->print();
                LOG_DEBUG("Displayed common subscriptions");
            }
            break;
        }

        case 18: {
            LOG_INFO("User selected: Find users nearby");
            string loc;
            cout << "Location: "; cin.ignore(); getline(cin, loc);
            auto usersNearby = net.findUsersByLocation(loc);
            if (usersNearby.empty()) LOG_INFO("No users found nearby in " + loc);
            else {
                for (auto& u : usersNearby) u->print();
                LOG_DEBUG("Displayed users nearby in " + loc);
            }
            break;
        }

        case 19: {
            LOG_INFO("User selected: Recommend users");
            int startId;
            cout << "Enter your user ID: ";
            cin >> startId;
            auto dist = net.shortestPathsFrom(startId);
            if (dist.empty()) {
                cout << "No recommendations found.\n";
                LOG_INFO("No recommendations for user ID=" + to_string(startId));
            }
            else {
                cout << "Recommended users you may known.\n";
                for (auto it = dist.begin(); it != dist.end(); ++it) {
                    cout << "User" << it->first << "connection distance:" << it->second << endl;
                }
                LOG_DEBUG("Displayed recommendations for ID=" + to_string(startId));
            }
            break;
        }


        case 20: {
            LOG_INFO("User selected: Show most central users");
            auto cent = net.userCentrality();
            if (cent.empty()) {
                LOG_WARN("Centrality computation: no users found.");
            }
            else {
                cout << "User influence ranking\n";
                for (auto it = cent.begin(); it != cent.end(); ++it) {
                    cout << "User " << it->first << "-> centrality score: " << it->second << endl;
                }
                LOG_DEBUG("Displayed user centrality scores");
            }
            break;
        }

        case 21: {
            LOG_INFO("User selected: Check friendship cycles");
            auto cycles = net.detectFriendGroups();
            if (cycles.empty()) {
                cout << "No friend cycles found.\n";
                LOG_INFO("No cycles detected in friendship graph");
            }
            else {
                cout << "Detected friend groups\n";
                for (size_t i = 0; i < cycles.size(); ++i) {
                    cout << "Group " << (i + 1) << ": (";
                    for (size_t j = 0; j < cycles[i].size(); ++j) {
                        cout << cycles[i][j];
                        if (j < cycles[i].size() - 1) cout << ", ";
                    }
                    cout << ")\n";
                }
                LOG_DEBUG("Detected " + to_string(cycles.size()) + " friend groups");
            }
            break;
        }

        case 22: {
            LOG_INFO("User selected: Update last login time");
            int id; cout << "User ID: "; cin >> id;
            if (auto* user = dynamic_cast<RegularUser*>(net.getUser(id))) {
                char buffer[26];
                time_t lastLogin = user->getLastLogin();
                ctime_s(buffer, sizeof(buffer), &lastLogin);
                cout << "Last login: " << buffer;
                LOG_DEBUG("Displayed last login for user ID=" + to_string(id));
            }
            else {
                cout << "User not found or not RegularUser.\n";
                LOG_WARN("Attempt to check last login for non-existent ID=" + to_string(id));
            }
            break;
        }

        case 23: {
            LOG_INFO("User selected: Generate random users");
            int count;
            string includeRelations;
            cout << "How many random users generate? ";
            cin >> count;

            cout << "Generate friendships, messages, etc.? (yes/no): ";
            cin >> includeRelations;
            bool withRelations = (includeRelations == "yes" || includeRelations == "Yes");

            SocialNetwork::generateRandomUsers(net, count, withRelations);
            net.printNetwork();
            LOG_DEBUG("Generated " + to_string(count) + " random users");
            break;
        }

        case 24: {
            LOG_INFO("User selected: Export graph to DOT format");
            string fname;
            cout << "Enter filename or full path to save DOT file (e.g. network.dot): ";
            cin >> fname;

            if (fname.find(".dot") == string::npos) {
                fname += ".dot";
            }

            net.exportToDot(fname);
            cout << "Attempted saving graph to " << fname << endl;
            LOG_INFO("Graph exported to " + fname);
            break;
        }


        case 25: {
            LOG_INFO("User selected: Save social network info to file");
            string fname;
            cout << "Enter filename for saving info (e.g. network_info.txt): ";
            cin >> fname;
            if (fname.find(".txt") == string::npos) {
                fname += ".txt";
            }
            net.saveToTextFile(fname);
            LOG_INFO("Network info saved to " + fname);
            break;
        }

        case 26: {
            LOG_INFO("User selected: View users by role");
            cout << "\n---Select user type---\n1. Regular users\n2. Premium users\nChoice: ";
            int t; cin >> t;

            if (t == 1) {
                auto regulars = net.getVerticesOfType<RegularUser>();
                cout << "\nRegular users (" << regulars.size() << "):\n";
                for (auto* u : regulars) u->print();
                LOG_DEBUG("Displayed all RegularUser profiles");
            }
            else if (t == 2) {
                auto premiums = net.getVerticesOfType<PremiumUser>();
                cout << "\nPremium users (" << premiums.size() << "):\n";
                for (auto* u : premiums) u->print();
                LOG_DEBUG("Displayed all PremiumUser profiles");
            }
            else {
                cout << "Invalid choice.\n";
                LOG_WARN("Invalid user type selection");
            }
            break;
        }

        case 27: {
            LOG_INFO("User selected: View relationships by type");
            cout << "\n--- Select relationship type ---\n1. Friendships\n2. Subscriptions\n3. Messages\n4. Posts\nChoice: ";
            int t; cin >> t;

            switch (t) {
            case 1: {
                auto f = net.getEdgesOfType<Friendship>();
                cout << "\nFriendships (" << f.size() << "):\n";
                for (auto* e : f) e->print();
                LOG_DEBUG("Displayed friendship edges");
                break;
            }
            case 2: {
                auto s = net.getEdgesOfType<Subscription>();
                cout << "\nSubscriptions (" << s.size() << "):\n";
                for (auto* e : s) e->print();
                LOG_DEBUG("Displayed subscription edges");
                break;
            }
            case 3: {
                auto m = net.getEdgesOfType<Message>();
                cout << "\nMessages (" << m.size() << "):\n";
                for (auto* e : m) e->print();
                LOG_DEBUG("Displayed message edges");
                break;
            }
            case 4: {
                auto p = net.getEdgesOfType<Post>();
                cout << "\nPosts (" << p.size() << "):\n";
                for (auto* e : p) e->print();
                LOG_DEBUG("Displayed post edges");
                break;
            }
            default:
                cout << "Invalid choice.\n";
                LOG_WARN("Invalid relationship type selection");
                break;
            }
            break;
        }

        case 28: {
            LOG_INFO("User selected: Explore entire social graph");
            cout << "\n--- Exploring all users (forEachVertex) ---\n";
            net.forEachVertex([](Vertex* v) {
                if (auto* u = dynamic_cast<User*>(v))
                    cout << "User " << u->getId() << ": " << u->getName() << endl;
                });
            cout << "\n--- Exploring all relationships (forEachEdge) ---\n";
            net.forEachEdge([](Edge* e) {
                e->print();
                });
            LOG_DEBUG("Social graph exploration completed");
            break;
        }
        default:
            if (choice != 0)
                LOG_WARN("Unknown menu choice: " + to_string(choice));
            break;
        }
    } while (choice != 0);

    LOG_INFO("Application terminated by user.");
}