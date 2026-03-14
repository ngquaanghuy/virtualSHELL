#ifndef CMD_TYPE_H
#define CMD_TYPE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_type : public Command {
public:
    std::string getName() const override { return "type"; }
    std::string getDescription() const override { return "Command type"; }
    std::string getUsage() const override { return "type <command>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
