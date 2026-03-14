#ifndef CMD_WC_H
#define CMD_WC_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_wc : public Command {
public:
    std::string getName() const override { return "wc"; }
    std::string getDescription() const override { return "Word count"; }
    std::string getUsage() const override { return "wc [-l] <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
