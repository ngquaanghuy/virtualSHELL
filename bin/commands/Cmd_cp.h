#ifndef CMD_CP_H
#define CMD_CP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_cp : public Command {
public:
    std::string getName() const override { return "cp"; }
    std::string getDescription() const override { return "Copy files"; }
    std::string getUsage() const override { return "cp <src> <dst>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
