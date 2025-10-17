#include "SocialNetwork.h"
#include "Logger.h"
#include "Menu.h"
#include "gtest/gtest.h"
#include <string>

int main(int argc, char** argv) {
    Logger::get().init("socialnetwork.log", LogLevel::DEBUG, true);
    LOG_INFO("Application started");

    bool runTests = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
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
