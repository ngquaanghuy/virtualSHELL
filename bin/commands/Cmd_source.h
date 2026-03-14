#ifndef CMD_SOURCE_H
#define CMD_SOURCE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_source : public Command {
public:
    std::string getName() const override { return "source"; }
    std::string getDescription() const override { return "Execute file"; }
    std::string getUsage() const override { return "source <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
