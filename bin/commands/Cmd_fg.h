#ifndef CMD_FG_H
#define CMD_FG_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_fg : public Command {
public:
    std::string getName() const override { return "fg"; }
    std::string getDescription() const override { return "Foreground process"; }
    std::string getUsage() const override { return "fg [job]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
