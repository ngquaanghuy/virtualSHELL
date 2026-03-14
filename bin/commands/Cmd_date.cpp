#include "Cmd_date.h"

int Cmd_date::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for date
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&time), "%a %b %d %H:%M:%S %Z %Y") << "\n";
    return 0;
}
