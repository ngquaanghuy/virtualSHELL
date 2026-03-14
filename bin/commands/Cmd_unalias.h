#ifndef CMD_UNALIAS_H
#define CMD_UNALIAS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_unalias : public Command {
public:
    std::string getName() const override { return "unalias"; }
    std::string getDescription() const override { return "Remove alias"; }
    std::string getUsage() const override { return "unalias <name>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
