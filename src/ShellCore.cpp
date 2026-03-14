#include "ShellCore.h"
#include "SandboxManager.h"
#include "CommandManager.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <string>
#include <signal.h>

ShellCore::ShellCore() : running(true) {
    sandbox = std::make_unique<SandboxManager>();
    cmdManager = std::make_unique<CommandManager>(sandbox.get());

    printWelcome();
    loadState();
}

ShellCore::~ShellCore() {
    saveState();
}

void ShellCore::printWelcome() {
    std::cout << "\033[36m";
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║     ███████╗██████╗  █████╗  ██████╗███████╗██████╗ ██████╗ ║\n";
    std::cout << "║     ██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗██╔══██╗║\n";
    std::cout << "║     █████╗  ██████╔╝███████║██║     █████╗  ██████╔╝██║  ██║║\n";
    std::cout << "║     ██╔══╝  ██╔══██╗██╔══██║██║     ██╔══╝  ██╔══██╗██║  ██║║\n";
    std::cout << "║     ██║     ██║  ██║██║  ██║╚██████╗███████╗██║  ██║██████╔╝║\n";
    std::cout << "║     ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚══════╝╚═╝  ╚═╝╚═════╝ ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "║                   virtualSHELL v1.0.0                     ║\n";
    std::cout << "║                                                           ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
    std::cout << "\033[0m";

    std::cout << "\n\033[32m[✓]\033[0m Virtual filesystem initialized\n";
    std::cout << "\033[32m[✓]\033[0m Sandbox environment ready\n";
    std::cout << "\033[32m[✓]\033[0m Command registry loaded\n";

    auto cmdNames = cmdManager->getCommandNames();
    std::cout << "\033[32m[✓]\033[0m Loaded " << cmdNames.size() << " commands available\n";

    std::cout << "\n\033[33m[!]\033[0m Type 'help' for available commands\n";
    std::cout << "\033[33m[!]\033[0m Type 'exit' to quit\n\n";
}

void ShellCore::printVersion() {
    std::cout << "virtualSHELL version 1.0.0\n";
    std::cout << "Copyright (c) 2026\n";
    std::cout << "License: MIT\n\n";
    std::cout << "Built with C++17\n";
}

void ShellCore::loadState() {
    if (sandbox->loadState("data/save.dat")) {
        Logger::log(Logger::Level::SUCCESS, "Virtual data loaded successfully!");
    } else {
        Logger::log(Logger::Level::INFO, "Starting with fresh environment");
    }
}

void ShellCore::saveState() {
    sandbox->saveState("data/save.dat");
}

void ShellCore::run() {
    while (running) {
        prompt();

        std::string input;
        std::getline(std::cin, input);

        if (std::cin.eof()) {
            std::cout << "\nlogout\n";
            break;
        }

        if (!input.empty()) {
            handleInput(input);
        }
    }

    std::cout << "\033[32m[✓]\033[0m Saving environment state...\n";
    saveState();
    std::cout << "\033[32m[✓]\033[0m Goodbye!\n";
}

void ShellCore::prompt() {
    std::string promptStr = expandPrompt(getPrompt());
    std::cout << promptStr << " ";
}

std::string ShellCore::getPrompt() const {
    return "\\u@\\h:\\w\\$ ";
}

std::string ShellCore::expandPrompt(const std::string& prompt) const {
    std::string result = prompt;
    std::string user = sandbox->getUser();
    std::string host = sandbox->getHostname();
    std::string pwd = sandbox->getWorkingDirectory();

    size_t pos = 0;
    while ((pos = result.find("\\u", pos)) != std::string::npos) {
        result.replace(pos, 2, user);
        pos += user.length();
    }

    pos = 0;
    while ((pos = result.find("\\h", pos)) != std::string::npos) {
        result.replace(pos, 2, host);
        pos += host.length();
    }

    pos = 0;
    while ((pos = result.find("\\w", pos)) != std::string::npos) {
        result.replace(pos, 2, pwd);
        pos += pwd.length();
    }

    pos = 0;
    while ((pos = result.find("\\$", pos)) != std::string::npos) {
        if (user == "root") {
            result.replace(pos, 2, "#");
        } else {
            result.replace(pos, 2, "$");
        }
        pos++;
    }

    result = "\033[1;32m" + result + "\033[0m";

    return result;
}

void ShellCore::handleInput(const std::string& input) {
    std::string cmdLine = input;

    size_t commentPos = cmdLine.find('#');
    if (commentPos != std::string::npos) {
        cmdLine = cmdLine.substr(0, commentPos);
    }

    if (cmdLine.empty()) {
        return;
    }

    std::stringstream ss(cmdLine);
    std::string cmd;
    ss >> cmd;

    if (cmd == "exit" || cmd == "logout") {
        running = false;
        return;
    }

    int exitCode = cmdManager->executeCommand(cmdLine);

    sandbox->addToHistory(cmdLine, exitCode);
}

bool ShellCore::isRunning() const {
    return running;
}

void ShellCore::stop() {
    running = false;
}
