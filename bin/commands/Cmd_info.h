#ifndef CMD_INFO_H
#define CMD_INFO_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_info : public Command {
public:
    std::string getName() const override { return "info"; }
    std::string getDescription() const override { return "Show info"; }
    std::string getUsage() const override { return "info [command]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
