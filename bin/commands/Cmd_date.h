#ifndef CMD_DATE_H
#define CMD_DATE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_date : public Command {
public:
    std::string getName() const override { return "date"; }
    std::string getDescription() const override { return "Print date and time"; }
    std::string getUsage() const override { return "date [+format]"; }
    int getMinArgs() const override { return 0; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
