#ifndef CMD_PASSWD_H
#define CMD_PASSWD_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_passwd : public Command {
public:
    std::string getName() const override { return "passwd"; }
    std::string getDescription() const override { return "Change password"; }
    std::string getUsage() const override { return "passwd [user]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
