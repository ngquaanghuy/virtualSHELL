#ifndef CMD_JOBS_H
#define CMD_JOBS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_jobs : public Command {
public:
    std::string getName() const override { return "jobs"; }
    std::string getDescription() const override { return "List jobs"; }
    std::string getUsage() const override { return "jobs"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
