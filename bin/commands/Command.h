#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class SandboxManager;

class Command {
public:
    virtual ~Command() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getUsage() const = 0;
    virtual int getMinArgs() const = 0;
    virtual int getMaxArgs() const = 0;
    virtual bool requiresRoot() const = 0;
    virtual int execute(const std::vector<std::string>& args, SandboxManager* sandbox) = 0;
};

#endif
