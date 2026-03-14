#ifndef CMD_WGET_H
#define CMD_WGET_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_wget : public Command {
public:
    std::string getName() const override { return "wget"; }
    std::string getDescription() const override { return "Download files"; }
    std::string getUsage() const override { return "wget <url>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
