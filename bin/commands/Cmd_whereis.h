#ifndef CMD_WHEREIS_H
#define CMD_WHEREIS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_whereis : public Command {
public:
    std::string getName() const override { return "whereis"; }
    std::string getDescription() const override { return "Locate command and docs"; }
    std::string getUsage() const override { return "whereis <command>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
