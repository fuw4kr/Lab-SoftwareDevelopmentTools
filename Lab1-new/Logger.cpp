#include "Logger.h"
#include <sstream>
#include <iomanip>
#include <ctime>

Logger::Logger() = default;

Logger::~Logger() {
    if (file.is_open()) file.close();
}

Logger& Logger::get() {
    static Logger instance;
    return instance;
}

void Logger::init(const string& filename, LogLevel level, bool console) {
    lock_guard<mutex> lock(mtx);
    minLevel = level;
    consoleOutput = console;
    file.open(filename, ios::app);
    if (file.is_open()) {
        file << "Logger started at " << timestamp() << " ---\n";
    }
}

void Logger::shutdown() {
    lock_guard<mutex> lock(mtx);
    if (file.is_open()) {
        file << "Logger stopped at " << timestamp() << " ---\n";
        file.close();
    }
}

bool Logger::isEnabled(LogLevel level) const {
    return static_cast<int>(level) >= static_cast<int>(minLevel);
}

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

string Logger::levelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG: return "DEBUG";
    case LogLevel::INFO:  return "INFO";
    case LogLevel::WARN:  return "WARN";
    case LogLevel::ERROR: return "ERROR";
    default:              return "UNKNOWN";
    }
}

string Logger::levelToColor(LogLevel level) const {
    switch (level) {
    case LogLevel::DEBUG: return "\033[36m";
    case LogLevel::INFO:  return "\033[32m";
    case LogLevel::WARN:  return "\033[33m";
    case LogLevel::ERROR: return "\033[31m";
    default:              return "\033[0m";
    }
}

void Logger::log(LogLevel level, const string& msg,
    const char* fileName, int line) {
    if (!isEnabled(level)) return;

    ostringstream oss;
    oss << "[" << timestamp() << "] "
        << "[" << levelToString(level) << "] "
        << fileName << ":" << line << " "
        << msg;

    string lineMsg = oss.str();

    lock_guard<mutex> lock(mtx);

    if (consoleOutput) {
        cout << levelToColor(level) << lineMsg << "\033[0m" << endl;
    }

    if (file.is_open()) {
        file << lineMsg << endl;
    }
}
