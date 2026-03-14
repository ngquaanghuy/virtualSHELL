#ifndef CMD_CAL_H
#define CMD_CAL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_cal : public Command {
public:
    std::string getName() const override { return "cal"; }
    std::string getDescription() const override { return "Display calendar"; }
    std::string getUsage() const override { return "cal [month] [year]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
