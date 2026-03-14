#ifndef CMD_TEST_H
#define CMD_TEST_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_test : public Command {
public:
    std::string getName() const override { return "test"; }
    std::string getDescription() const override { return "Test condition"; }
    std::string getUsage() const override { return "test [expr]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
