#include "Cmd_pwd.h"

int Cmd_pwd::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for pwd
    std::cout << sandbox->getWorkingDirectory() << "\n";
    return 0;
}
