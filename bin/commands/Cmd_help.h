#ifndef CMD_HELP_H
#define CMD_HELP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_help : public Command {
public:
    std::string getName() const override { return "help"; }
    std::string getDescription() const override { return "Show help"; }
    std::string getUsage() const override { return "help [command]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
