#ifndef CMD_UPTIME_H
#define CMD_UPTIME_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_uptime : public Command {
public:
    std::string getName() const override { return "uptime"; }
    std::string getDescription() const override { return "System uptime"; }
    std::string getUsage() const override { return "uptime"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
