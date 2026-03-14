#ifndef CMD_IP_H
#define CMD_IP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ip : public Command {
public:
    std::string getName() const override { return "ip"; }
    std::string getDescription() const override { return "IP command"; }
    std::string getUsage() const override { return "ip [addr|route|link]"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 3; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
