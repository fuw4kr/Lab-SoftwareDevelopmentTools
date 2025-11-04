/**
 * @file Logger.h
 * @brief Defines the Logger class for unified console and file logging.
 * @details Provides thread-safe logging with adjustable log levels, timestamps,
 * and optional colored console output. Used throughout the project for
 * diagnostics and runtime tracking.
 *
 * @date 04.11.2025
 * @version 1.0
 * @author
 * Kristina Zakharchenko
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <iostream>
using namespace std;

/**
 * @enum LogLevel
 * @brief Represents logging severity levels.
 */
enum class LogLevel {
    DEBUG, ///< Detailed diagnostic messages for debugging.
    INFO,  ///< General informational messages.
    WARN,  ///< Warnings about potentially problematic situations.
    ERROR  ///< Critical errors that require attention.
};

/**
 * @class Logger
 * @brief Thread-safe singleton logger for file and console output.
 * @details Provides logging capabilities with timestamps, colored console output,
 * and adjustable minimum log levels.
 *
 * @example
 * Logger::get().init("log.txt", LogLevel::DEBUG, true);
 * LOG_INFO("Application started");
 * LOG_ERROR("File not found");
 * Logger::get().shutdown();
 */
class Logger {
private:
    ofstream file;          ///< Output file stream for log entries.
    mutex mtx;              ///< Mutex for thread safety.
    LogLevel minLevel;      ///< Minimum log level to record.
    bool consoleOutput;     ///< Determines whether to print logs to the console.

    /**
     * @brief Private constructor for singleton pattern.
     */
    Logger();

    /**
     * @brief Destructor closes the file if open.
     */
    ~Logger();

    /**
     * @brief Generates a formatted timestamp string.
     * @return Current time in format YYYY-MM-DD HH:MM:SS.
     */
    string timestamp() const;

    /**
     * @brief Converts log level to string (e.g., "INFO", "DEBUG").
     * @param level Log level.
     * @return Corresponding string representation.
     */
    string levelToString(LogLevel level) const;

    /**
     * @brief Returns ANSI color code for given log level.
     * @param level Log level.
     * @return ANSI color escape sequence string.
     */
    string levelToColor(LogLevel level) const;

public:
    Logger(const Logger&) = delete;             ///< Deleted copy constructor.
    Logger& operator=(const Logger&) = delete;  ///< Deleted assignment operator.

    /**
     * @brief Retrieves the singleton instance of Logger.
     * @return Reference to Logger instance.
     */
    static Logger& get();

    /**
     * @brief Initializes the logger.
     * @param filename Path to the output log file.
     * @param level Minimum log level to display and store.
     * @param console Whether to print logs to console (default: true).
     */
    void init(const string& filename,
        LogLevel level = LogLevel::INFO,
        bool console = true);

    /**
     * @brief Closes the log file and writes a termination message.
     */
    void shutdown();

    /**
     * @brief Checks if the given log level is enabled.
     * @param level Log level to check.
     * @return True if the log level is equal or above current minimum level.
     */
    bool isEnabled(LogLevel level) const;

    /**
     * @brief Writes a log message to console and/or file.
     * @param level Logging level (DEBUG, INFO, etc.)
     * @param msg The message to log.
     * @param fileName The name of the source file that called the logger.
     * @param line The line number in the source file.
     */
    void log(LogLevel level, const string& msg,
        const char* fileName, int line);
};

/**
 * @brief Logs a DEBUG message.
 */
#define LOG_DEBUG(msg) Logger::get().log(LogLevel::DEBUG, msg, __FILE__, __LINE__)

 /**
  * @brief Logs an INFO message.
  */
#define LOG_INFO(msg)  Logger::get().log(LogLevel::INFO,  msg, __FILE__, __LINE__)

  /**
   * @brief Logs a WARN message.
   */
#define LOG_WARN(msg)  Logger::get().log(LogLevel::WARN,  msg, __FILE__, __LINE__)

   /**
    * @brief Logs an ERROR message.
    */
#define LOG_ERROR(msg) Logger::get().log(LogLevel::ERROR, msg, __FILE__, __LINE__)

#endif // LOGGER_H
