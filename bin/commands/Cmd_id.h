#ifndef CMD_ID_H
#define CMD_ID_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_id : public Command {
public:
    std::string getName() const override { return "id"; }
    std::string getDescription() const override { return "Print user identity"; }
    std::string getUsage() const override { return "id [user]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
