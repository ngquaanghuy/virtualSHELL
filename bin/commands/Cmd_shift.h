#ifndef CMD_SHIFT_H
#define CMD_SHIFT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_shift : public Command {
public:
    std::string getName() const override { return "shift"; }
    std::string getDescription() const override { return "Shift positional params"; }
    std::string getUsage() const override { return "shift [n]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
