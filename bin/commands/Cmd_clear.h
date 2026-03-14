#ifndef CMD_CLEAR_H
#define CMD_CLEAR_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_clear : public Command {
public:
    std::string getName() const override { return "clear"; }
    std::string getDescription() const override { return "Clear screen"; }
    std::string getUsage() const override { return "clear"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
