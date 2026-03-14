#ifndef CMD_TOP_H
#define CMD_TOP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_top : public Command {
public:
    std::string getName() const override { return "top"; }
    std::string getDescription() const override { return "Process monitor"; }
    std::string getUsage() const override { return "top"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
