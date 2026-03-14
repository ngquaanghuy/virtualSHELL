#ifndef CMD_SET_H
#define CMD_SET_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_set : public Command {
public:
    std::string getName() const override { return "set"; }
    std::string getDescription() const override { return "Set options"; }
    std::string getUsage() const override { return "set [options]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
