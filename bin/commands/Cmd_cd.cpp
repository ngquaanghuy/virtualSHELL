#include "Cmd_cd.h"

int Cmd_cd::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for cd
    std::string path = args.empty() ? "/home/user" : args[0];
    auto vfs = sandbox->getFileSystem();
    if (vfs->changeDirectory(path)) {
        sandbox->setWorkingDirectory(vfs->getCurrentPath());
        return 0;
    }
    std::cerr << "cd: " << path << ": No such directory\n";
    return 1;
}
