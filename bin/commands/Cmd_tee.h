#ifndef CMD_TEE_H
#define CMD_TEE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_tee : public Command {
public:
    std::string getName() const override { return "tee"; }
    std::string getDescription() const override { return "Read from stdin"; }
    std::string getUsage() const override { return "tee <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
