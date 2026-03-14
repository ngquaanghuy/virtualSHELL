#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <csignal>
#include "SandboxManager.h"
#include "CommandManager.h"
#include "ShellCore.h"
#include "Logger.h"

ShellCore* g_shell = nullptr;

void signalHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "\n(Use 'exit' command to quit virtualSHELL)\n";
    }
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help     Show this help message\n";
    std::cout << "  -v, --version Show version information\n";
    std::cout << "  -c <command>  Execute command and exit\n";
    std::cout << "  --verbose     Enable verbose logging\n";
}

void printVersion() {
    std::cout << "virtualSHELL version 1.0.0\n";
    std::cout << "Copyright (c) 2026\n";
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signalHandler);

    bool verbose = false;
    std::string execCommand;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }

        if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        }

        if (arg == "-c" && i + 1 < argc) {
            execCommand = argv[++i];
            continue;
        }

        if (arg == "--verbose") {
            verbose = true;
            continue;
        }

        std::cerr << "Unknown option: " << arg << "\n";
        printUsage(argv[0]);
        return 1;
    }

    Logger::setVerbose(verbose);

    if (!execCommand.empty()) {
        std::unique_ptr<SandboxManager> sandbox = std::make_unique<SandboxManager>();
        std::unique_ptr<CommandManager> cmdManager = std::make_unique<CommandManager>(sandbox.get());

        sandbox->loadState("data/save.dat");
        int exitCode = cmdManager->executeCommand(execCommand);
        sandbox->saveState("data/save.dat");
        return exitCode;
    }

    try {
        std::unique_ptr<ShellCore> shell = std::make_unique<ShellCore>();
        shell->run();
    } catch (const std::exception& e) {
        std::cerr << "\033[31mError:\033[0m " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "\033[31mError:\033[0m An unexpected error occurred\n";
        return 1;
    }

    return 0;
}
