#include "Cmd_whoami.h"

int Cmd_whoami::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for whoami
    std::cout << sandbox->getUser() << "\n";
    return 0;
}
