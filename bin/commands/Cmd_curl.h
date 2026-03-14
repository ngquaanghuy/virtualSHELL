#ifndef CMD_CURL_H
#define CMD_CURL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_curl : public Command {
public:
    std::string getName() const override { return "curl"; }
    std::string getDescription() const override { return "Transfer data"; }
    std::string getUsage() const override { return "curl [-o] <url>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
