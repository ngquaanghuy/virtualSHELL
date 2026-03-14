#ifndef CMD_POWEROFF_H
#define CMD_POWEROFF_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_poweroff : public Command {
public:
    std::string getName() const override { return "poweroff"; }
    std::string getDescription() const override { return "Power off system"; }
    std::string getUsage() const override { return "poweroff"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return true; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
