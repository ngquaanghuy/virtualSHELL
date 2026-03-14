#ifndef CMD_AWK_H
#define CMD_AWK_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_awk : public Command {
public:
    std::string getName() const override { return "awk"; }
    std::string getDescription() const override { return "Pattern scanning"; }
    std::string getUsage() const override { return "awk <pattern> <file>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
