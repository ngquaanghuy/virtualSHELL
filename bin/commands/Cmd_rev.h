#ifndef CMD_REV_H
#define CMD_REV_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_rev : public Command {
public:
    std::string getName() const override { return "rev"; }
    std::string getDescription() const override { return "Reverse lines"; }
    std::string getUsage() const override { return "rev <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
