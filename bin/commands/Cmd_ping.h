#ifndef CMD_PING_H
#define CMD_PING_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ping : public Command {
public:
    std::string getName() const override { return "ping"; }
    std::string getDescription() const override { return "Send ICMP echo"; }
    std::string getUsage() const override { return "ping [-c] <host>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
