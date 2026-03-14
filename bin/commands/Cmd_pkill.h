#ifndef CMD_PKILL_H
#define CMD_PKILL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_pkill : public Command {
public:
    std::string getName() const override { return "pkill"; }
    std::string getDescription() const override { return "Kill by pattern"; }
    std::string getUsage() const override { return "pkill <pattern>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
