#include "Logger.h"
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <cstdarg>

LogLevel Logger::currentLogLevel = LogLevel::LOG_ERROR;

void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

std::string Logger::formatTimestamp() {
    auto now=std::chrono::system_clock::now();
    auto time_t_in_seconds= std::chrono::system_clock::to_time_t(now);
   	std::ostringstream ss;
    ss << std::put_time(std::localtime(&time_t_in_seconds), "%FT%TZ");


    return ss.str();
}
void Logger::log(LogLevel level, const char* format, ...) {
    if (level <= currentLogLevel) {
        va_list args;
        va_start(args, format);

        auto now = std::chrono::system_clock::now();
        auto now_sec = std::chrono::system_clock::to_time_t(now);
        std::string timestamp=formatTimestamp();
        std::cerr << "[" << timestamp << "] ";

        switch (level) {
            case LogLevel::LOG_ERROR:
                std::cerr << "[ERROR] ";
                break;
            case LogLevel::LOG_INFO:
                std::cerr << "[INFO] ";
                break;
            case LogLevel::LOG_VERBOSE:
                std::cerr << "[VERBOSE] ";
                break;
            case LogLevel::LOG_DEBUG:
                std::cerr << "[DEBUG] ";
                break;
            default:
                break;
        }

        vfprintf(stderr, format, args);
        va_end(args);
        std::cerr << std::endl;
    }
}
