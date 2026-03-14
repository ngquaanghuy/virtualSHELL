#ifndef CMD_TR_H
#define CMD_TR_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_tr : public Command {
public:
    std::string getName() const override { return "tr"; }
    std::string getDescription() const override { return "Character translation"; }
    std::string getUsage() const override { return "tr <set1> [set2]"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
