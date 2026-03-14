#ifndef CMD_SORT_H
#define CMD_SORT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_sort : public Command {
public:
    std::string getName() const override { return "sort"; }
    std::string getDescription() const override { return "Sort lines"; }
    std::string getUsage() const override { return "sort [-r] <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
