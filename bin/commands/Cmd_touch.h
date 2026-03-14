#ifndef CMD_TOUCH_H
#define CMD_TOUCH_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_touch : public Command {
public:
    std::string getName() const override { return "touch"; }
    std::string getDescription() const override { return "Create empty files"; }
    std::string getUsage() const override { return "touch <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
