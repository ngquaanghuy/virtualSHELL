#ifndef CMD_CHOWN_H
#define CMD_CHOWN_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_chown : public Command {
public:
    std::string getName() const override { return "chown"; }
    std::string getDescription() const override { return "Change owner"; }
    std::string getUsage() const override { return "chown <owner> <file>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return true; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
