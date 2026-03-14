#ifndef CMD_CAT_H
#define CMD_CAT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_cat : public Command {
public:
    std::string getName() const override { return "cat"; }
    std::string getDescription() const override { return "Display file contents"; }
    std::string getUsage() const override { return "cat <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
