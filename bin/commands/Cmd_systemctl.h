#ifndef CMD_SYSTEMCTL_H
#define CMD_SYSTEMCTL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_systemctl : public Command {
public:
    std::string getName() const override { return "systemctl"; }
    std::string getDescription() const override { return "Systemd control"; }
    std::string getUsage() const override { return "systemctl <action> [service]"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
