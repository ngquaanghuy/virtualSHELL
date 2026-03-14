#ifndef CMD_SUDO_H
#define CMD_SUDO_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_sudo : public Command {
public:
    std::string getName() const override { return "sudo"; }
    std::string getDescription() const override { return "Execute as superuser"; }
    std::string getUsage() const override { return "sudo <command>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 999; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
