#ifndef CMD_SAVE_H
#define CMD_SAVE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_save : public Command {
public:
    std::string getName() const override { return "save"; }
    std::string getDescription() const override { return "Save virtualSHELL"; }
    std::string getUsage() const override { return "save [file]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
