#include "Logger.h"

bool Logger::verbose = true;

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::INFO:    return "INFO";
        case Level::SUCCESS: return "SUCCESS";
        case Level::WARNING: return "WARNING";
        case Level::ERROR:   return "ERROR";
        case Level::LOAD:    return "LOAD";
        default:            return "UNKNOWN";
    }
}

std::string Logger::getColor(Level level) {
    switch (level) {
        case Level::INFO:    return "\033[36m";
        case Level::SUCCESS: return "\033[32m";
        case Level::WARNING: return "\033[33m";
        case Level::ERROR:   return "\033[31m";
        case Level::LOAD:    return "\033[35m";
        default:             return "\033[0m";
    }
}

std::string Logger::resetColor() {
    return "\033[0m";
}

void Logger::setVerbose(bool v) {
    verbose = v;
}

void Logger::log(Level level, const std::string& message) {
    if (!verbose && level != Level::ERROR && level != Level::SUCCESS) {
        return;
    }

    std::string color = getColor(level);
    std::string levelStr = levelToString(level);
    std::string timestamp = getTimestamp();

    std::cout << color << "[" << levelStr << "]" << resetColor() 
              << " [" << timestamp << "] " << message << std::endl;
}

void Logger::logLoad(const std::string& path, bool success) {
    std::string color = success ? "\033[32m" : "\033[31m";
    std::string status = success ? "success!" : "failed!";
    std::string timestamp = getTimestamp();

    std::cout << color << "[Load]" << resetColor() 
              << " [" << timestamp << "] Loaded " << path << " " 
              << color << status << resetColor() << std::endl;
}
