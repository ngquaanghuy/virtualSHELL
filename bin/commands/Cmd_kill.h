#ifndef CMD_KILL_H
#define CMD_KILL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_kill : public Command {
public:
    std::string getName() const override { return "kill"; }
    std::string getDescription() const override { return "Terminate process"; }
    std::string getUsage() const override { return "kill [-signal] <pid>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
