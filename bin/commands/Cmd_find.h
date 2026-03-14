#ifndef CMD_FIND_H
#define CMD_FIND_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_find : public Command {
public:
    std::string getName() const override { return "find"; }
    std::string getDescription() const override { return "Search for files"; }
    std::string getUsage() const override { return "find <path> <name>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
