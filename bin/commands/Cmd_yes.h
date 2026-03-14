#ifndef CMD_YES_H
#define CMD_YES_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_yes : public Command {
public:
    std::string getName() const override { return "yes"; }
    std::string getDescription() const override { return "Repeated output"; }
    std::string getUsage() const override { return "yes [string]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
