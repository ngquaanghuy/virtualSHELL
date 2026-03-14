#ifndef CMD_HALT_H
#define CMD_HALT_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_halt : public Command {
public:
    std::string getName() const override { return "halt"; }
    std::string getDescription() const override { return "Halt system"; }
    std::string getUsage() const override { return "halt"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return true; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
