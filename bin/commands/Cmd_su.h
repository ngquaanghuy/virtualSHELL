#ifndef CMD_SU_H
#define CMD_SU_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_su : public Command {
public:
    std::string getName() const override { return "su"; }
    std::string getDescription() const override { return "Switch user"; }
    std::string getUsage() const override { return "su [user]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
