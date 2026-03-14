#ifndef CMD_CHMOD_H
#define CMD_CHMOD_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_chmod : public Command {
public:
    std::string getName() const override { return "chmod"; }
    std::string getDescription() const override { return "Change permissions"; }
    std::string getUsage() const override { return "chmod <perm> <file>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
