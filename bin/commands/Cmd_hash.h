#ifndef CMD_HASH_H
#define CMD_HASH_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_hash : public Command {
public:
    std::string getName() const override { return "hash"; }
    std::string getDescription() const override { return "Hash commands"; }
    std::string getUsage() const override { return "hash [-r] [command]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
