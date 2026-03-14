#ifndef CMD_TRAP_H
#define CMD_TRAP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_trap : public Command {
public:
    std::string getName() const override { return "trap"; }
    std::string getDescription() const override { return "Signal handler"; }
    std::string getUsage() const override { return "trap [action] [sigs]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
