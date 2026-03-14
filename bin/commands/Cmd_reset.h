#ifndef CMD_RESET_H
#define CMD_RESET_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_reset : public Command {
public:
    std::string getName() const override { return "reset"; }
    std::string getDescription() const override { return "Reset terminal"; }
    std::string getUsage() const override { return "reset"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
