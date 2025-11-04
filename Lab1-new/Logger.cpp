/**
 * @file Logger.cpp
 * @brief Implements the Logger class for thread-safe logging operations.
 * @details Provides message formatting, timestamp generation, color-coded output,
 * and file persistence. Supports both console and file logging simultaneously.
 *
 * @date 04.11.2025
 * @version 1.0
 * @see Logger
 * @see LogLevel
 * @author
 * Kristina Zakharchenko
 */

#include "Logger.h"
#include <sstream>
#include <iomanip>
#include <ctime>

 /**
  * @brief Default constructor (private) for singleton initialization.
  */
Logger::Logger() = default;

/**
 * @brief Destructor closes the file stream if still open.
 */
Logger::~Logger() {
    if (file.is_open()) file.close();
}

/**
 * @brief Returns a reference to the singleton Logger instance.
 */
Logger& Logger::get() {
    static Logger instance;
    return instance;
}

/**
 * @brief Initializes the Logger by opening the output file and setting configurations.
 * @param filename Log file path.
 * @param level Minimum log level to capture.
 * @param console Whether to also print to console.
 */
void Logger::init(const string& filename, LogLevel level, bool console) {
    lock_guard<mutex> lock(mtx);
    minLevel = level;
    consoleOutput = console;
    file.open(filename, ios::app);
    if (file.is_open()) {
        file << "Logger started at " << timestamp() << " ---\n";
    }
}

/**
 * @brief Closes the log file and writes the shutdown message.
 */
void Logger::shutdown() {
    lock_guard<mutex> lock(mtx);
    if (file.is_open()) {
        file << "Logger stopped at " << timestamp() << " ---\n";
        file.close();
    }
}

/**
 * @brief Checks if the given log level should be processed.
 * @param level The log level to verify.
 * @return True if current minLevel allows logging of this message.
 */
bool Logger::isEnabled(LogLevel level) const {
    return static_cast<int>(level) >= static_cast<int>(minLevel);
}

/**
 * @brief Returns the current timestamp formatted as "YYYY-MM-DD HH:MM:SS".
 */
string Logger::timestamp() const {
    ostringstream oss;
    time_t now = time(nullptr);
    tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

/**
 * @brief Converts a LogLevel enum to its string representation.
 */
string Logger::levelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO:  return "INFO";
    case LogLevel::WARN:  return "WARN";
    case LogLevel::ERROR: return "ERROR";
    default:              return "UNKNOWN";
    }
}

/**
 * @brief Maps a log level to a corresponding ANSI color escape sequence.
 */
string Logger::levelToColor(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG: return "\033[36m"; // Blue
    case LogLevel::INFO:  return "\033[32m"; // Green
    case LogLevel::WARN:  return "\033[33m"; // Yellow
    case LogLevel::ERROR: return "\033[31m"; // Red
    default:              return "\033[0m";  // Reset
    }
}

/**
 * @brief Logs a message to both console and file.
 * @param level The severity level.
 * @param msg The log message text.
 * @param fileName*
