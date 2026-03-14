#ifndef CMD_TAIL_H
#define CMD_TAIL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_tail : public Command {
public:
    std::string getName() const override { return "tail"; }
    std::string getDescription() const override { return "Display last lines"; }
    std::string getUsage() const override { return "tail [-n] <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
