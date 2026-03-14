#ifndef CMD_VERSION_H
#define CMD_VERSION_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_version : public Command {
public:
    std::string getName() const override { return "version"; }
    std::string getDescription() const override { return "Show version"; }
    std::string getUsage() const override { return "version"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
