#ifndef CMD_TYPESET_H
#define CMD_TYPESET_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_typeset : public Command {
public:
    std::string getName() const override { return "typeset"; }
    std::string getDescription() const override { return "Type variable"; }
    std::string getUsage() const override { return "typeset [options] [name=value]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
