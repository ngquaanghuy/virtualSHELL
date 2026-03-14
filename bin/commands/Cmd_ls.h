#ifndef CMD_LS_H
#define CMD_LS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ls : public Command {
public:
    std::string getName() const override { return "ls"; }
    std::string getDescription() const override { return "List directory contents"; }
    std::string getUsage() const override { return "ls [-la] [path]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
