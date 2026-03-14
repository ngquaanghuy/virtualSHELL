#ifndef CMD_HISTORY_H
#define CMD_HISTORY_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_history : public Command {
public:
    std::string getName() const override { return "history"; }
    std::string getDescription() const override { return "Command history"; }
    std::string getUsage() const override { return "history [-c]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
