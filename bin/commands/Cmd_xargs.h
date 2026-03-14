#ifndef CMD_XARGS_H
#define CMD_XARGS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_xargs : public Command {
public:
    std::string getName() const override { return "xargs"; }
    std::string getDescription() const override { return "Build command"; }
    std::string getUsage() const override { return "xargs <command>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
