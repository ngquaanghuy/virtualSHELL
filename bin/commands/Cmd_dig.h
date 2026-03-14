#ifndef CMD_DIG_H
#define CMD_DIG_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_dig : public Command {
public:
    std::string getName() const override { return "dig"; }
    std::string getDescription() const override { return "DNS lookup"; }
    std::string getUsage() const override { return "dig <domain>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
