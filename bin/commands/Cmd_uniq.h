#ifndef CMD_UNIQ_H
#define CMD_UNIQ_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_uniq : public Command {
public:
    std::string getName() const override { return "uniq"; }
    std::string getDescription() const override { return "Remove duplicates"; }
    std::string getUsage() const override { return "uniq <file>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
