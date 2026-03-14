#ifndef CMD_WHOAMI_H
#define CMD_WHOAMI_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_whoami : public Command {
public:
    std::string getName() const override { return "whoami"; }
    std::string getDescription() const override { return "Print current user"; }
    std::string getUsage() const override { return "whoami"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
