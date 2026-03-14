#ifndef CMD_NETSTAT_H
#define CMD_NETSTAT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_netstat : public Command {
public:
    std::string getName() const override { return "netstat"; }
    std::string getDescription() const override { return "Network statistics"; }
    std::string getUsage() const override { return "netstat [-a]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
