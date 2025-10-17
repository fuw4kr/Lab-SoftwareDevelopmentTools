#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <iostream>
using namespace std;

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
private:
    ofstream file;
    mutex mtx;
    LogLevel minLevel = LogLevel::INFO;
    bool consoleOutput = true;

    Logger();
    ~Logger();

    string timestamp() const;
    string levelToString(LogLevel level) const;
    string levelToColor(LogLevel level) const;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& get();

    void init(const string& filename,
        LogLevel level = LogLevel::INFO,
        bool console = true);

    void shutdown();
    bool isEnabled(LogLevel level) const;

    void log(LogLevel level, const string& msg,
        const char* fileName, int line);
};

#define LOG_DEBUG(msg) Logger::get().log(LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg)  Logger::get().log(LogLevel::INFO,  msg, __FILE__, __LINE__)
#define LOG_WARN(msg)  Logger::get().log(LogLevel::WARN,  msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::get().log(LogLevel::ERROR, msg, __FILE__, __LINE__)

#endif // LOGGER_H
