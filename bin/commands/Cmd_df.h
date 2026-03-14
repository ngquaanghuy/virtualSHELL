#ifndef CMD_DF_H
#define CMD_DF_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_df : public Command {
public:
    std::string getName() const override { return "df"; }
    std::string getDescription() const override { return "Disk filesystem"; }
    std::string getUsage() const override { return "df"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 0; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
