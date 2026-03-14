#ifndef CMD_ALIAS_H
#define CMD_ALIAS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_alias : public Command {
public:
    std::string getName() const override { return "alias"; }
    std::string getDescription() const override { return "Create command alias"; }
    std::string getUsage() const override { return "alias [name='command']"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
