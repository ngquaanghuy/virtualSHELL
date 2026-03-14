#ifndef CMD_RETURN_H
#define CMD_RETURN_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_return : public Command {
public:
    std::string getName() const override { return "return"; }
    std::string getDescription() const override { return "Return function"; }
    std::string getUsage() const override { return "return [n]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
