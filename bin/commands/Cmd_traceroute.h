#ifndef CMD_TRACEROUTE_H
#define CMD_TRACEROUTE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_traceroute : public Command {
public:
    std::string getName() const override { return "traceroute"; }
    std::string getDescription() const override { return "Trace route"; }
    std::string getUsage() const override { return "traceroute <host>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
