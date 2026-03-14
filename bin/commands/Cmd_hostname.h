#ifndef CMD_HOSTNAME_H
#define CMD_HOSTNAME_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_hostname : public Command {
public:
    std::string getName() const override { return "hostname"; }
    std::string getDescription() const override { return "Print system hostname"; }
    std::string getUsage() const override { return "hostname [name]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
