#ifndef CMD_CUT_H
#define CMD_CUT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_cut : public Command {
public:
    std::string getName() const override { return "cut"; }
    std::string getDescription() const override { return "Cut columns"; }
    std::string getUsage() const override { return "cut [-d] [-f] <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 3; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
