#include "Cmd_echo.h"

int Cmd_echo::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for echo
    bool noNewline = false;
    for (const auto& arg : args) {
        if (arg == "-n") {
            noNewline = true;
            continue;
        }
        std::cout << arg << " ";
    }
    if (!noNewline) std::cout << "\n";
    return 0;
}
