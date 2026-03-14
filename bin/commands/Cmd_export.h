#ifndef CMD_EXPORT_H
#define CMD_EXPORT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_export : public Command {
public:
    std::string getName() const override { return "export"; }
    std::string getDescription() const override { return "Set environment variable"; }
    std::string getUsage() const override { return "export [NAME=value]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
