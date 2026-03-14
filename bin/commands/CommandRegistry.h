#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include <map>
#include <string>
#include <memory>
#include "../include/SandboxManager.h"

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

using CommandPtr = std::unique_ptr<Command>;

class CommandRegistry {
public:
    static CommandRegistry& getInstance();
    
    void registerCommand(CommandPtr cmd);
    Command* getCommand(const std::string& name) const;
    std::vector<std::string> getCommandNames() const;
    std::string getCommandDescription(const std::string& name) const;
    void printHelp() const;
    void printHelp(const std::string& command) const;
    void loadAllCommands();
    void loadCommand(const std::string& commandName);
    bool isCommandLoaded(const std::string& cmdName) const;
    std::vector<std::string> getLoadedCommands() const;
    std::vector<std::string> getUnloadedCommands() const;
    void unloadCommand(const std::string& cmdName);
    void unloadAllCommands();

private:
    CommandRegistry();
    ~CommandRegistry() = default;
    CommandRegistry(const CommandRegistry&) = delete;
    CommandRegistry& operator=(const CommandRegistry&) = delete;

    std::map<std::string, CommandPtr> commands;
    std::map<std::string, bool> loadedCommands;

    void registerAllCommands();
    void registerFileCommands();
    void registerSystemCommands();
    void registerNetworkCommands();
    void registerTextCommands();
    void registerProcessCommands();
    void registerMiscCommands();
};

#define REGISTER_COMMAND(classname) \
    class classname : public Command { \
    public: \
        std::string getName() const override; \
        std::string getDescription() const override; \
        std::string getUsage() const override; \
        int getMinArgs() const override; \
        int getMaxArgs() const override; \
        bool requiresRoot() const override; \
        int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override; \
    };

#endif
