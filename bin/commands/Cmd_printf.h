#ifndef CMD_PRINTF_H
#define CMD_PRINTF_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_printf : public Command {
public:
    std::string getName() const override { return "printf"; }
    std::string getDescription() const override { return "Formatted output"; }
    std::string getUsage() const override { return "printf <format> [args]"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
