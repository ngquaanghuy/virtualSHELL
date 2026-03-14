#ifndef CMD_MKDIR_H
#define CMD_MKDIR_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_mkdir : public Command {
public:
    std::string getName() const override { return "mkdir"; }
    std::string getDescription() const override { return "Create directories"; }
    std::string getUsage() const override { return "mkdir [-p] <dir>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
