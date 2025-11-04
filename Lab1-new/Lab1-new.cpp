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
#include "gtest/gtest.h"
#include <string>

/**
 * @brief Main function � entry point of the Social Network application.
 * @details Initializes the logger, creates a SocialNetwork instance,
 * runs the menu interface, and performs clean shutdown.
 *
 * @return Exit status code (0 if the program terminated successfully).
 * @throws std::runtime_error If logger initialization or shutdown fails.
 
 */
    Logger::get().init("socialnetwork.log", LogLevel::DEBUG, true);
    LOG_INFO("Application started");

    bool runTests = false;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--test") {
            runTests = true;
            break;
        }
    }

    if (runTests) {
        LOG_INFO("Running unit tests");
        ::testing::InitGoogleTest(&argc, argv);
        int testResult = RUN_ALL_TESTS();
        if (testResult != 0) {
            LOG_INFO("Unit tests failed. Menu will not start.");
            Logger::get().shutdown();
            return testResult;
        }
        LOG_INFO("All unit tests passed. Starting menu...");
    }

    SocialNetwork net;
    LOG_INFO("Created SocialNetwork instance");
    runMenu(net);

    LOG_INFO("Application terminated normally");
    Logger::get().shutdown();
    return 0;
}
