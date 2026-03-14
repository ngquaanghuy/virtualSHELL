#ifndef CMD_MV_H
#define CMD_MV_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_mv : public Command {
public:
    std::string getName() const override { return "mv"; }
    std::string getDescription() const override { return "Move/rename files"; }
    std::string getUsage() const override { return "mv <src> <dst>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
