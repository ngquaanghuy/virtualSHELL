#include "Cmd_uname.h"

int Cmd_uname::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for uname
    bool all = false;
    for (const auto& arg : args) {
        if (arg == "-a") all = true;
    }
    if (all) {
        std::cout << "Linux virtualshell 5.0.0-virtual #1 SMP Mon Jan 1 00:00:00 UTC 2026 x86_64 GNU/Linux\n";
    } else {
        std::cout << "Linux\n";
    }
    return 0;
}
