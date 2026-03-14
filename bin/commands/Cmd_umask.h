#ifndef CMD_UMASK_H
#define CMD_UMASK_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_umask : public Command {
public:
    std::string getName() const override { return "umask"; }
    std::string getDescription() const override { return "File creation mask"; }
    std::string getUsage() const override { return "umask [mask]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
