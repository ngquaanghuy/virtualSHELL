#ifndef CMD_WATCH_H
#define CMD_WATCH_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_watch : public Command {
public:
    std::string getName() const override { return "watch"; }
    std::string getDescription() const override { return "Execute periodically"; }
    std::string getUsage() const override { return "watch <command>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
