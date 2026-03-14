#ifndef CMD_READ_H
#define CMD_READ_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_read : public Command {
public:
    std::string getName() const override { return "read"; }
    std::string getDescription() const override { return "Read input"; }
    std::string getUsage() const override { return "read [-p] <var>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
