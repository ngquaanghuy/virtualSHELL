#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

class Logger {
public:
    enum class Level {
        INFO,
        SUCCESS,
        WARNING,
        ERROR,
        LOAD
    };

    static void log(Level level, const std::string& message);
    static void logLoad(const std::string& path, bool success);
    static void setVerbose(bool verbose);
    static std::string getTimestamp();

private:
    static std::string levelToString(Level level);
    static std::string getColor(Level level);
    static std::string resetColor();
    static bool verbose;
};

#endif
