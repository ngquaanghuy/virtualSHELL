#ifndef CMD_SERVICE_H
#define CMD_SERVICE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_service : public Command {
public:
    std::string getName() const override { return "service"; }
    std::string getDescription() const override { return "Run system service"; }
    std::string getUsage() const override { return "service <name> <action>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
