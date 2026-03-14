#ifndef CMD_WAIT_H
#define CMD_WAIT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_wait : public Command {
public:
    std::string getName() const override { return "wait"; }
    std::string getDescription() const override { return "Wait for process"; }
    std::string getUsage() const override { return "wait [pid]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
