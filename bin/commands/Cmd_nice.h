#ifndef CMD_NICE_H
#define CMD_NICE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_nice : public Command {
public:
    std::string getName() const override { return "nice"; }
    std::string getDescription() const override { return "Run with priority"; }
    std::string getUsage() const override { return "nice -n <priority> <command>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
