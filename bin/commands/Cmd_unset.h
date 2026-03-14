#ifndef CMD_UNSET_H
#define CMD_UNSET_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_unset : public Command {
public:
    std::string getName() const override { return "unset"; }
    std::string getDescription() const override { return "Remove environment variable"; }
    std::string getUsage() const override { return "unset <name>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
