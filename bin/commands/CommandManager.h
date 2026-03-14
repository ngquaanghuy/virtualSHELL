#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <dlfcn.h>
#include <filesystem>
#include "Command.h"

class SandboxManager;

class CommandManager {
public:
    struct CommandInfo {
        std::string name;
        std::string description;
        std::string usage;
        int minArgs;
        int maxArgs;
        bool requiresRoot;
        std::function<int(std::vector<std::string>& args, SandboxManager* sandbox)> handler;
    };

    CommandManager(SandboxManager* sandbox);
    ~CommandManager();

    void registerCommand(const CommandInfo& cmd);
    void registerBuiltInCommands();

    int executeCommand(const std::string& cmdLine);

    std::vector<std::string> getCommandNames() const;
    std::string getCommandDescription(const std::string& cmdName) const;
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
    SandboxManager* sandbox;
    std::map<std::string, CommandInfo> commands;
    std::map<std::string, bool> loadedCommands;

    std::vector<std::string> parseCommandLine(const std::string& cmdLine) const;
    std::string expandAlias(const std::string& cmd) const;
    std::string expandVariables(const std::string& str) const;

    void loadCommandsFromBin();
};

#endif
