#ifndef CMD_SHUTDOWN_H
#define CMD_SHUTDOWN_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_shutdown : public Command {
public:
    std::string getName() const override { return "shutdown"; }
    std::string getDescription() const override { return "Shutdown system"; }
    std::string getUsage() const override { return "shutdown [-h] [-r] [time]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return true; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
