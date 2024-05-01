#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <cstdarg>

enum class LogLevel {
    LOG_NONE,
    LOG_ERROR,
    LOG_INFO,
    LOG_VERBOSE,
    LOG_DEBUG,
};

constexpr auto LOG_NONE = LogLevel::LOG_NONE;
constexpr auto LOG_ERROR = LogLevel::LOG_ERROR;
constexpr auto LOG_INFO = LogLevel::LOG_INFO;
constexpr auto LOG_VERBOSE = LogLevel::LOG_VERBOSE;
constexpr auto LOG_DEBUG = LogLevel::LOG_DEBUG;

class Logger {
public:
    static void setLogLevel(LogLevel level);
    static std::string formatTimestamp();
    static void log(LogLevel level, const char *format, ...);

private:
    static LogLevel currentLogLevel;
};

#endif // LOGGER_H
