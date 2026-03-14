#ifndef CMD_READONLY_H
#define CMD_READONLY_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_readonly : public Command {
public:
    std::string getName() const override { return "readonly"; }
    std::string getDescription() const override { return "Readonly variable"; }
    std::string getUsage() const override { return "readonly [name=value]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
