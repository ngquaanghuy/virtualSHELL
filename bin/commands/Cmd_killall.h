#ifndef CMD_KILLALL_H
#define CMD_KILLALL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_killall : public Command {
public:
    std::string getName() const override { return "killall"; }
    std::string getDescription() const override { return "Kill processes by name"; }
    std::string getUsage() const override { return "killall <name>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
