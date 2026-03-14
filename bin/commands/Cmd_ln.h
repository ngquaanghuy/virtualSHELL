#ifndef CMD_LN_H
#define CMD_LN_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ln : public Command {
public:
    std::string getName() const override { return "ln"; }
    std::string getDescription() const override { return "Create links"; }
    std::string getUsage() const override { return "ln -s <target> <link>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 3; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
