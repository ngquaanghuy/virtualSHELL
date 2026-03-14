#ifndef CMD_FTP_H
#define CMD_FTP_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class Cmd_ftp : public Command {
public:
    std::string getName() const override { return "ftp"; }
    std::string getDescription() const override { return "FTP client"; }
    std::string getUsage() const override { return "ftp <host>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 1; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};

#endif
