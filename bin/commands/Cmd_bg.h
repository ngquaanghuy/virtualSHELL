#ifndef CMD_BG_H
#define CMD_BG_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_bg : public Command {
public:
    std::string getName() const override { return "bg"; }
    std::string getDescription() const override { return "Background process"; }
    std::string getUsage() const override { return "bg [job]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
