#ifndef CMD_TIME_H
#define CMD_TIME_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_time : public Command {
public:
    std::string getName() const override { return "time"; }
    std::string getDescription() const override { return "Time command"; }
    std::string getUsage() const override { return "time <command>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
