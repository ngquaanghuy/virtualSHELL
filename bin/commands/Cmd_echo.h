#ifndef CMD_ECHO_H
#define CMD_ECHO_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_echo : public Command {
public:
    std::string getName() const override { return "echo"; }
    std::string getDescription() const override { return "Print text"; }
    std::string getUsage() const override { return "echo [-n] [text]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
