#ifndef CMD_APT_H
#define CMD_APT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_apt : public Command {
public:
    std::string getName() const override { return "apt"; }
    std::string getDescription() const override { return "Package manager"; }
    std::string getUsage() const override { return "apt <action> [packages]"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
