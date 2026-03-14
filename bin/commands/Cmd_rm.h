#ifndef CMD_RM_H
#define CMD_RM_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_rm : public Command {
public:
    std::string getName() const override { return "rm"; }
    std::string getDescription() const override { return "Remove files"; }
    std::string getUsage() const override { return "rm [-rf] <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
