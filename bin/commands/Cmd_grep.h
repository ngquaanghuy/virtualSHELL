#ifndef CMD_GREP_H
#define CMD_GREP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_grep : public Command {
public:
    std::string getName() const override { return "grep"; }
    std::string getDescription() const override { return "Pattern matching"; }
    std::string getUsage() const override { return "grep <pattern> <file>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
