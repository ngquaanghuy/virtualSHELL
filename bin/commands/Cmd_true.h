#ifndef CMD_TRUE_H
#define CMD_TRUE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_true : public Command {
public:
    std::string getName() const override { return "true"; }
    std::string getDescription() const override { return "Do nothing"; }
    std::string getUsage() const override { return "true"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
