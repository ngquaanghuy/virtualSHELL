#ifndef CMD_SSH_H
#define CMD_SSH_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ssh : public Command {
public:
    std::string getName() const override { return "ssh"; }
    std::string getDescription() const override { return "Secure shell"; }
    std::string getUsage() const override { return "ssh [user@]host"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
