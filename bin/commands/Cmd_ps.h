#ifndef CMD_PS_H
#define CMD_PS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ps : public Command {
public:
    std::string getName() const override { return "ps"; }
    std::string getDescription() const override { return "Process status"; }
    std::string getUsage() const override { return "ps [-aux]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
