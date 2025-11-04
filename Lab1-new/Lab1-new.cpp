/**
 * @file main.cpp
 * @brief Entry point for the Social Network application.
 * @details Initializes logging, creates the SocialNetwork instance,
 * launches the interactive menu, and ensures proper shutdown.
 *
 * This program demonstrates graph-based modeling of a social network
 * and showcases logging, user interaction, and modular system design.
 *
 * @see SocialNetwork
 * @see Logger
 * @see Menu
 *
 * @author
 * Kristina Zakharchenko
 * @date
 * 04.11.2025
 */

#include "SocialNetwork.h"
#include "Logger.h"
#include "Menu.h"
using namespace std;

/**
 * @brief Main function — entry point of the Social Network application.
 * @details Initializes the logger, creates a SocialNetwork instance,
 * runs the menu interface, and performs clean shutdown.
 *
 * @return Exit status code (0 if the program terminated successfully).
 * @throws std::runtime_error If logger initialization or shutdown fails.
 *
 * @example
 * // Typical usage:
 * int main() {
 *     Logger::get().init("socialnetwork.log", LogLevel::DEBUG, true);
 *     SocialNetwork net;
 *     runMenu(net);
 *     Logger::get().shutdown();
 *     return 0;
 * }
 */
int main() {
    Logger::get().init("socialnetwork.log", LogLevel::DEBUG, true);
    LOG_INFO("Application started");

    SocialNetwork net;
    LOG_INFO("Created SocialNetwork instance");

    runMenu(net);

    LOG_INFO("Application terminated normally");
    Logger::get().shutdown();

    return 0;
}
