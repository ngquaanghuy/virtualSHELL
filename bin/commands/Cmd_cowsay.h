#ifndef CMD_COWSAY_H
#define CMD_COWSAY_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_cowsay : public Command {
public:
    std::string getName() const override { return "cowsay"; }
    std::string getDescription() const override { return "Cow say"; }
    std::string getUsage() const override { return "cowsay <text>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
