#include "Cmd_ls.h"

int Cmd_ls::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Implementation for ls
    bool showAll = false;
    bool longFormat = false;
    std::string path = sandbox->getWorkingDirectory();

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i][0] == '-') {
            for (char c : args[i]) {
                if (c == 'a') showAll = true;
                if (c == 'l') longFormat = true;
            }
        } else {
            path = args[i];
        }
    }

    auto vfs = sandbox->getFileSystem();
    auto files = vfs->listDirectory(path);

    if (longFormat) {
        for (const auto& f : files) {
            if (!showAll && f[0] == '.') continue;
            std::string fullPath = vfs->getAbsolutePath(path) + "/" + f;
            std::cout << (vfs->isDirectory(fullPath) ? "d" : "-") << "rwxr-xr-x ";
            std::cout << "1 user user ";
            std::cout << std::setw(8) << vfs->getFileSize(fullPath) << " ";
            std::cout << f << (vfs->isDirectory(fullPath) ? "/" : "") << "\n";
        }
    } else {
        for (const auto& f : files) {
            if (!showAll && f[0] == '.') continue;
            std::cout << f << (vfs->isDirectory(vfs->getAbsolutePath(path) + "/" + f) ? "/" : "") << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
