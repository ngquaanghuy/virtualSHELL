#ifndef CMD_SED_H
#define CMD_SED_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_sed : public Command {
public:
    std::string getName() const override { return "sed"; }
    std::string getDescription() const override { return "Stream editor"; }
    std::string getUsage() const override { return "sed <script> <file>"; }
    int getMinArgs() const override { return 2; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
