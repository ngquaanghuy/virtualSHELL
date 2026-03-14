#ifndef SHELLCORE_H
#define SHELLCORE_H

#include <string>
#include <iostream>
#include <memory>

class SandboxManager;
class CommandManager;

class ShellCore {
public:
    ShellCore();
    ~ShellCore();

    void run();
    void prompt();
    bool isRunning() const;
    void stop();

    void loadState();
    void saveState();

private:
    std::unique_ptr<SandboxManager> sandbox;
    std::unique_ptr<CommandManager> cmdManager;
    bool running;

    std::string getPrompt() const;
    std::string expandPrompt(const std::string& prompt) const;
    void handleInput(const std::string& input);
    void printWelcome();
    void printVersion();
};

#endif
