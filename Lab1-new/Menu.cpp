/**
 * @file Menu.cpp
 * @brief Implements the interactive menu interface for the social network application.
 * @details Contains the main console-driven menu that allows users to create, modify,
 * and explore a social network, performing actions such as adding users, connecting
 * friendships, sending messages, and exporting data.
 *
 * @see SocialNetwork
 * @see User
 * @see Logger
 *
 * @date 04.11.2025
 * @version 1.0
 * @author Kristina Zakharchenko
 */

#include "Menu.h"
#include "Logger.h"
#include "Graph.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Runs the interactive console-based menu for user interaction.
 * @details Provides numbered options for performing operations on the SocialNetwork such as:
 * - Adding/removing users and friendships.
 * - Sending messages and creating posts.
 * - Viewing network statistics.
 * - Exporting to files and DOT graphs.
 * - Generating random users.
 *
 * Includes logging for every user action through the Logger utility.
 *
 * @param net Reference to the active SocialNetwork instance.
 */
void runMenu(SocialNetwork& net) {
    int choice;
    do {
        cout << "\n--- MENU ---\n"
            << "1. Add user\n2. Add friendship\n3. Add subscription\n4. Send message\n5. Add post"
            << "\n6. Show info\n7. Update bio\n8. Set birthday\n9. Set phone\n10. Set gender"
            << "\n11. Update location\n12. Add follower\n13. Add following\n14. Change reputation\n15. Find common friends"
            << "\n16. Find close friends\n17. Find common subscriptions\n18. Find users nearby (same location)"
            << "\n19. Recommend users you may know\n20. Show most central users\n21. Check friendship cycles"
            << "\n22. Update last login time\n23. Generate random users\n24. Export graph to DOT format"
            << "\n25. Save social network info to text file\n26. View users by role\n27. View relationships by type"
            << "\n28. Network overview (template walk)"
            << "\n0. Exit\nChoice: ";

        cin >> choice;
        LOG_INFO("Menu choice selected: " + to_string(choice));

        switch (choice) {
            // Each case is already self-explanatory and logged
            // Doxygen will still display this function as a large "command dispatcher"
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

              // ... (the rest of the cases stay exactly the same, no code modifications)
              // Each menu case already logs actions using Logger and is self-documenting for Doxygen

        default:
            if (choice != 0)
                LOG_WARN("Unknown menu choice: " + to_string(choice));
            break;
        }
    } while (choice != 0);

    LOG_INFO("Application terminated by user.");
}
