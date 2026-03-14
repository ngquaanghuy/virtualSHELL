#ifndef CMD_EXIT_H
#define CMD_EXIT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_exit : public Command {
public:
    std::string getName() const override { return "exit"; }
    std::string getDescription() const override { return "Exit shell"; }
    std::string getUsage() const override { return "exit [code]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
