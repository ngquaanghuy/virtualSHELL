#include "CommandManager.h"
#include "SandboxManager.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace fs = std::filesystem;

CommandManager::CommandManager(SandboxManager* sand) : sandbox(sand) {
    registerBuiltInCommands();
}

CommandManager::~CommandManager() {
}

void CommandManager::registerCommand(const CommandInfo& cmd) {
    commands[cmd.name] = cmd;
    loadedCommands[cmd.name] = false;
}

void CommandManager::registerBuiltInCommands() {
    registerFileCommands();
    registerSystemCommands();
    registerNetworkCommands();
    registerTextCommands();
    registerProcessCommands();
    registerMiscCommands();
}

void CommandManager::registerFileCommands() {
    CommandInfo cmd;

    cmd = {"ls", "List directory contents", "ls [-la] [path]", 0, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool showAll = false;
            bool longFormat = false;
            std::string path = sb->getWorkingDirectory();

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

            auto vfs = sb->getFileSystem();
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
        }};
    registerCommand(cmd);

    cmd = {"cd", "Change current directory", "cd [path]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string path = args.empty() ? "/home/user" : args[0];
            auto vfs = sb->getFileSystem();
            if (vfs->changeDirectory(path)) {
                sb->setWorkingDirectory(vfs->getCurrentPath());
                return 0;
            }
            std::cerr << "cd: " << path << ": No such directory\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"pwd", "Print working directory", "pwd", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << sb->getWorkingDirectory() << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"mkdir", "Create directories", "mkdir [-p] <dir>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool recursive = false;
            std::string path;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == "-p") recursive = true;
                else path = args[i];
            }

            auto vfs = sb->getFileSystem();
            if (vfs->createDirectory(path)) {
                return 0;
            }
            std::cerr << "mkdir: cannot create directory '" << path << "'\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"rmdir", "Remove directories", "rmdir <dir>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            if (vfs->isEmpty(args[0])) {
                if (vfs->deletePath(args[0])) return 0;
            } else {
                std::cerr << "rmdir: failed to remove '" << args[0] << "': Directory not empty\n";
                return 1;
            }
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"rm", "Remove files", "rm [-rf] <file>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool recursive = false;
            bool force = false;
            std::string path;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i][0] == '-') {
                    for (char c : args[i]) {
                        if (c == 'r') recursive = true;
                        if (c == 'f') force = true;
                    }
                } else {
                    path = args[i];
                }
            }

            auto vfs = sb->getFileSystem();
            if (vfs->exists(path)) {
                if (vfs->deletePath(path)) return 0;
            } else if (!force) {
                std::cerr << "rm: cannot remove '" << path << "': No such file or directory\n";
                return 1;
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"touch", "Create empty files", "touch <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            if (vfs->createFile(args[0], "")) {
                return 0;
            }
            std::cerr << "touch: cannot create file '" << args[0] << "'\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"cat", "Display file contents", "cat <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (vfs->readFile(args[0], content)) {
                std::cout << content;
                return 0;
            }
            std::cerr << "cat: " << args[0] << ": No such file\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"cp", "Copy files", "cp <src> <dst>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            if (vfs->copyFile(args[0], args[1])) return 0;
            std::cerr << "cp: cannot copy '" << args[0] << "'\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"mv", "Move/rename files", "mv <src> <dst>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            if (vfs->moveFile(args[0], args[1])) return 0;
            std::cerr << "mv: cannot move '" << args[0] << "'\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"ln", "Create links", "ln -s <target> <link>", 2, 3, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool symbolic = false;
            std::string target, link;

            for (const auto& arg : args) {
                if (arg == "-s") symbolic = true;
                else if (target.empty()) target = arg;
                else link = arg;
            }

            if (link.empty()) {
                std::cerr << "ln: missing destination\n";
                return 1;
            }

            std::cout << "ln: created " << (symbolic ? "symbolic" : "hard") 
                      << " link: " << link << " -> " << target << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"chmod", "Change permissions", "chmod <perm> <file>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "chmod: " << args[0] << " " << args[1] << "\n";
            std::cout << "(Permissions simulated in virtual environment)\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"chown", "Change owner", "chown <owner> <file>", 2, 2, true,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (sb->getUser() != "root") {
                std::cerr << "chown: permission denied\n";
                return 1;
            }
            std::cout << "chown: " << args[0] << " " << args[1] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"tree", "Display directory tree", "tree [path]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            std::string path = args.empty() ? sb->getWorkingDirectory() : args[0];
            vfs->printTree(path);
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"find", "Search for files", "find <path> <name>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            auto files = vfs->listDirectory(args[0]);
            std::string searchName = args[1];

            for (const auto& f : files) {
                if (f.find(searchName) != std::string::npos) {
                    std::cout << args[0] << "/" << f << "\n";
                }
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"du", "Disk usage", "du [path]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            std::string path = args.empty() ? sb->getWorkingDirectory() : args[0];
            auto files = vfs->listDirectory(path);

            size_t total = 0;
            for (const auto& f : files) {
                std::string fullPath = path + "/" + f;
                size_t size = vfs->getFileSize(fullPath);
                total += size;
                std::cout << size << "\t" << f << "\n";
            }
            std::cout << "Total: " << total << " bytes\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"df", "Disk filesystem", "df", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Filesystem     1K-blocks      Used Available Use% Mounted on\n";
            std::cout << "virtualfs         1024000     51200    972800   5% /\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"stat", "Display file status", "stat <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto vfs = sb->getFileSystem();
            if (!vfs->exists(args[0])) {
                std::cerr << "stat: cannot stat '" << args[0] << "': No such file\n";
                return 1;
            }

            std::cout << "File: " << args[0] << "\n";
            std::cout << "Type: " << (vfs->isDirectory(args[0]) ? "Directory" : "Regular File") << "\n";
            std::cout << "Size: " << vfs->getFileSize(args[0]) << " bytes\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"wc", "Word count", "wc [-l] <file>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool linesOnly = false;
            std::string path;

            for (const auto& arg : args) {
                if (arg == "-l") linesOnly = true;
                else path = arg;
            }

            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(path, content)) {
                std::cerr << "wc: " << path << ": No such file\n";
                return 1;
            }

            int lines = 0, words = 0, chars = 0;
            bool inWord = false;

            for (char c : content) {
                chars++;
                if (c == '\n') lines++;
                if (std::isspace(c)) {
                    inWord = false;
                } else if (!inWord) {
                    words++;
                    inWord = true;
                }
            }

            if (linesOnly) {
                std::cout << lines << " " << path << "\n";
            } else {
                std::cout << lines << " " << words << " " << chars << " " << path << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"head", "Display first lines", "head [-n] <file>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int lines = 10;
            std::string path;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == "-n" && i + 1 < args.size()) {
                    lines = std::stoi(args[++i]);
                } else {
                    path = args[i];
                }
            }

            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(path, content)) {
                std::cerr << "head: cannot open '" << path << "'\n";
                return 1;
            }

            std::stringstream ss(content);
            std::string line;
            int count = 0;
            while (std::getline(ss, line) && count < lines) {
                std::cout << line << "\n";
                count++;
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"tail", "Display last lines", "tail [-n] <file>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int lines = 10;
            std::string path;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == "-n" && i + 1 < args.size()) {
                    lines = std::stoi(args[++i]);
                } else {
                    path = args[i];
                }
            }

            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(path, content)) {
                std::cerr << "tail: cannot open '" << path << "'\n";
                return 1;
            }

            std::stringstream ss(content);
            std::vector<std::string> allLines;
            std::string line;
            while (std::getline(ss, line)) {
                allLines.push_back(line);
            }

            int start = (allLines.size() > (size_t)lines) ? allLines.size() - lines : 0;
            for (size_t i = start; i < allLines.size(); ++i) {
                std::cout << allLines[i] << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);
}

void CommandManager::registerSystemCommands() {
    CommandInfo cmd;

    cmd = {"sudo", "Execute as superuser", "sudo <command>", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string realUser = sb->getUser();
            sb->setUser("root");

            std::cout << "[sudo] password for " << realUser << ": ";
            std::string password;
            std::getline(std::cin, password);

            if (password.empty()) {
                std::cerr << "sudo: a password is required\n";
                sb->setUser(realUser);
                return 1;
            }

            std::cout << "[root] " << args[0];
            for (size_t i = 1; i < args.size(); ++i) {
                std::cout << " " << args[i];
            }
            std::cout << "\n";

            sb->setUser(realUser);
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"apt", "Package manager", "apt <action> [packages]", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                std::cerr << "apt: missing action\n";
                return 1;
            }

            std::string action = args[0];

            if (action == "update") {
                std::cout << "Reading package lists... Done\n";
                std::cout << "Building dependency tree... Done\n";
                return 0;
            }

            if (action == "install") {
                if (args.size() < 2) {
                    std::cerr << "apt install: missing package name\n";
                    return 1;
                }
                std::cout << "Reading state information... Done\n";
                std::cout << "The following NEW packages will be installed:\n";
                for (size_t i = 1; i < args.size(); ++i) {
                    std::cout << "  " << args[i] << "\n";
                }
                std::cout << "0 upgraded, " << args.size() - 1 << " newly installed.\n";
                std::cout << "Setting up " << args[1] << "...\n";
                return 0;
            }

            if (action == "remove") {
                if (args.size() < 2) {
                    std::cerr << "apt remove: missing package name\n";
                    return 1;
                }
                std::cout << "The following packages will be REMOVED:\n";
                for (size_t i = 1; i < args.size(); ++i) {
                    std::cout << "  " << args[i] << "\n";
                }
                std::cout << args.size() - 1 << " packages removed.\n";
                return 0;
            }

            if (action == "search") {
                if (args.size() < 2) {
                    std::cerr << "apt search: missing search term\n";
                    return 1;
                }
                std::cout << "Searching...\n";
                std::cout << "No apt packages found.\n";
                return 0;
            }

            if (action == "list") {
                std::cout << "virtualshelldemo    [installed]\n";
                return 0;
            }

            std::cerr << "apt: Unknown action: " << action << "\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"pkg", "Package manager (BSD style)", "pkg <action> [packages]", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                std::cerr << "pkg: missing action\n";
                return 1;
            }

            std::string action = args[0];

            if (action == "install") {
                if (args.size() < 2) {
                    std::cerr << "pkg install: missing package name\n";
                    return 1;
                }
                std::cout << "Packages to be installed: " << args[1] << "\n";
                std::cout << "Downloading packages... Done\n";
                std::cout << "Installing packages... Done\n";
                return 0;
            }

            if (action == "remove") {
                if (args.size() < 2) {
                    std::cerr << "pkg remove: missing package name\n";
                    return 1;
                }
                std::cout << "Packages to be removed: " << args[1] << "\n";
                std::cout << "Removing packages... Done\n";
                return 0;
            }

            if (action == "info") {
                std::cout << "Package name: virtualshelldemo\n";
                std::cout << "Version: 1.0\n";
                std::cout << "Description: Virtual shell demo package\n";
                return 0;
            }

            std::cerr << "pkg: Unknown action: " << action << "\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"uname", "System information", "uname [-a]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
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
        }};
    registerCommand(cmd);

    cmd = {"whoami", "Print current user", "whoami", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << sb->getUser() << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"hostname", "Print system hostname", "hostname [name]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                std::cout << sb->getHostname() << "\n";
            } else {
                sb->setHostname(args[0]);
                std::cout << "Hostname set to: " << args[0] << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"date", "Print date and time", "date [+format]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::cout << std::put_time(std::localtime(&time), "%a %b %d %H:%M:%S %Z %Y") << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"cal", "Display calendar", "cal [month] [year]", 0, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int month = 3, year = 2026;

            if (args.size() >= 1) {
                if (args[0].find_first_of("0123456789") != std::string::npos) {
                    month = std::stoi(args[0]);
                }
            }
            if (args.size() >= 2) {
                year = std::stoi(args[1]);
            }

            std::cout << "     March " << year << "\n";
            std::cout << "Su Mo Tu We Th Fr Sa\n";
            std::cout << "       1  2  3  4  5  6\n";
            std::cout << " 7  8  9 10 11 12 13\n";
            std::cout << "14 15 16 17 18 19 20\n";
            std::cout << "21 22 23 24 25 26 27\n";
            std::cout << "28 29 30 31\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"uptime", "System uptime", "uptime", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << " 12:00:00 up 1 day,  3:42,  1 user,  load average: 0.10, 0.05, 0.01\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"id", "Print user identity", "id [user]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string user = args.empty() ? sb->getUser() : args[0];
            std::cout << "uid=1000(" << user << ") gid=1000(" << user << ") groups=1000(" << user << ")\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"groups", "Print user groups", "groups [user]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string user = args.empty() ? sb->getUser() : args[0];
            std::cout << user << " : " << user << " sudo\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"passwd", "Change password", "passwd [user]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string user = args.empty() ? sb->getUser() : args[0];
            std::cout << "Changing password for " << user << ".\n";
            std::cout << "(password change simulated in virtual environment)\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"su", "Switch user", "su [user]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string newUser = args.empty() ? "root" : args[0];
            std::string oldUser = sb->getUser();
            sb->setUser(newUser);
            std::cout << "Switched to user: " << newUser << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"which", "Locate command", "which <command>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "/bin/" << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"whereis", "Locate command and docs", "whereis <command>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << args[0] << ": /bin/" << args[0] << " /usr/share/man/man1/" << args[0] << ".1.gz\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"type", "Command type", "type <command>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (commands.find(args[0]) != commands.end()) {
                std::cout << args[0] << " is a shell builtin\n";
            } else {
                std::cout << args[0] << " is /bin/" << args[0] << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"alias", "Create command alias", "alias [name='command']", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                auto aliases = sb->getAliases();
                for (const auto& a : aliases) {
                    std::cout << "alias " << a.first << "='" << a.second << "'\n";
                }
                return 0;
            }

            size_t eqPos = args[0].find('=');
            if (eqPos != std::string::npos) {
                std::string name = args[0].substr(0, eqPos);
                std::string value = args[0].substr(eqPos + 1);
                if (value.front() == '\'' && value.back() == '\'') {
                    value = value.substr(1, value.length() - 2);
                }
                sb->setAliases(name, value);
                return 0;
            }

            std::string alias = sb->getAlias(args[0]);
            if (!alias.empty()) {
                std::cout << "alias " << args[0] << "='" << alias << "'\n";
            } else {
                std::cerr << "alias: " << args[0] << ": not found\n";
                return 1;
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"unalias", "Remove alias", "unalias <name>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            sb->removeAlias(args[0]);
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"env", "Print environment", "env", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto env = sb->getAllEnv();
            for (const auto& e : env) {
                std::cout << e.first << "=" << e.second << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"export", "Set environment variable", "export [NAME=value]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                auto env = sb->getAllEnv();
                for (const auto& e : env) {
                    std::cout << e.first << "=" << e.second << "\n";
                }
                return 0;
            }

            size_t eqPos = args[0].find('=');
            if (eqPos != std::string::npos) {
                std::string name = args[0].substr(0, eqPos);
                std::string value = args[0].substr(eqPos + 1);
                sb->setEnv(name, value);
                return 0;
            }
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"unset", "Remove environment variable", "unset <name>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            sb->unsetEnv(args[0]);
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"exit", "Exit shell", "exit [code]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int code = 0;
            if (!args.empty()) {
                code = std::stoi(args[0]);
            }
            sb->setLastExitCode(code);
            return 128 + code;
        }};
    registerCommand(cmd);

    cmd = {"logout", "Logout", "logout", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 128 + 1;
        }};
    registerCommand(cmd);

    cmd = {"shutdown", "Shutdown system", "shutdown [-h] [-r] [time]", 0, 2, true,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool halt = false, reboot = false;

            for (const auto& arg : args) {
                if (arg == "-h") halt = true;
                if (arg == "-r") reboot = true;
            }

            if (halt) {
                std::cout << "System shutdown...\n";
            } else if (reboot) {
                std::cout << "System rebooting...\n";
            } else {
                std::cout << "Shutdown scheduled.\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"reboot", "Reboot system", "reboot", 0, 0, true,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "System rebooting...\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"halt", "Halt system", "halt", 0, 0, true,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "System halted.\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"poweroff", "Power off system", "poweroff", 0, 0, true,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Powering off...\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"systemctl", "Systemd control", "systemctl <action> [service]", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string action = args[0];
            std::string service = args.size() > 1 ? args[1] : "";

            if (action == "start" || action == "stop" || action == "restart" || action == "status") {
                if (service.empty()) {
                    std::cerr << "systemctl: missing service name\n";
                    return 1;
                }
                std::cout << action << " " << service << "... Done\n";
                if (action == "status") {
                    std::cout << "● " << service << ".service - " << service << " service\n";
                    std::cout << "   Loaded: loaded (/lib/systemd/system/" << service << ".service; enabled)\n";
                    std::cout << "   Active: active (running)\n";
                }
                return 0;
            }

            if (action == "list-units") {
                std::cout << "UNIT                          LOAD   ACTIVE SUB     DESCRIPTION\n";
                std::cout << "shell.service                 loaded active running Virtual Shell\n";
                return 0;
            }

            std::cerr << "systemctl: Unknown action: " << action << "\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"service", "Run system service", "service <name> <action>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << args[1] << " on " << args[0] << "... Done\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"clear", "Clear screen", "clear", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "\033[2J\033[H";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"reset", "Reset terminal", "reset", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "\033[2J\033[H\033c";
            return 0;
        }};
    registerCommand(cmd);
}

void CommandManager::registerNetworkCommands() {
    CommandInfo cmd;

    cmd = {"ping", "Send ICMP echo", "ping [-c] <host>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int count = 4;
            std::string host;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == "-c" && i + 1 < args.size()) {
                    count = std::stoi(args[++i]);
                } else {
                    host = args[i];
                }
            }

            std::cout << "PING " << host << " (127.0.0.1) 56(84) bytes of data.\n";
            for (int i = 0; i < count; ++i) {
                std::cout << "64 bytes from " << host << ": icmp_seq=" << i + 1 << " ttl=64 time=0.1 ms\n";
            }
            std::cout << "\n--- " << host << " ping statistics ---\n";
            std::cout << count << " packets transmitted, " << count << " received, 0% packet loss\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"curl", "Transfer data", "curl [-o] <url>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool output = false;
            std::string url, filename;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i] == "-o") {
                    output = true;
                    if (i + 1 < args.size()) {
                        filename = args[++i];
                    }
                } else {
                    url = args[i];
                }
            }

            std::cout << "curl: simulated request to " << url << "\n";
            std::cout << "(Network access simulated in virtual environment)\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"wget", "Download files", "wget <url>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "--2026-01-01 12:00:00--  " << args[0] << "\n";
            std::cout << "Resolving " << args[0] << "... 127.0.0.1\n";
            std::cout << "Connecting... connected.\n";
            std::cout << "HTTP request sent, awaiting response... 200 OK\n";
            std::cout << "Length: 1024 (1.0K) [text/html]\n";
            std::cout << "Saving to: 'index.html'\n";
            std::cout << "\nindex.html          100%[===================>]   1.00K  --.-KB/s    in 0s\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"ssh", "Secure shell", "ssh [user@]host", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "ssh: connecting to " << args[0] << "...\n";
            std::cout << "(SSH connection simulated in virtual environment)\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"scp", "Secure copy", "scp <src> <dst>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "scp: copying " << args[0] << " to " << args[1] << "\n";
            std::cout << "(SCP simulated in virtual environment)\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"ftp", "FTP client", "ftp <host>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "ftp: connecting to " << args[0] << "...\n";
            std::cout << "Name (virtualshell:user): user\n";
            std::cout << "Password required\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"netstat", "Network statistics", "netstat [-a]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Active Internet connections (servers and established)\n";
            std::cout << "Proto Recv-Q Send-Q Local Address           Foreign Address         State\n";
            std::cout << "tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN\n";
            std::cout << "tcp        0      0 127.0.0.1:631           0.0.0.0:*               LISTEN\n";
            std::cout << "tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"ifconfig", "Configure network", "ifconfig [interface]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500\n";
            std::cout << "        inet 192.168.1.100  netmask 255.255.255.0  broadcast 192.168.1.255\n";
            std::cout << "        ether 00:00:00:00:00:00  txqueuelen 1000  (Ethernet)\n";
            std::cout << "        RX packets 1024  bytes 102400 (100.0 KiB)\n";
            std::cout << "        TX packets 512  bytes 51200 (50.0 KiB)\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"ip", "IP command", "ip [addr|route|link]", 1, 3, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty() || args[0] == "addr") {
                std::cout << "1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN\n";
                std::cout << "    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00\n";
                std::cout << "    inet 127.0.0.1/8 scope host lo\n";
                std::cout << "2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500\n";
                std::cout << "    link/ether 00:00:00:00:00:00 brd ff:ff:ff:ff:ff:ff\n";
                std::cout << "    inet 192.168.1.100/24 brd 192.168.1.255 scope global eth0\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"nslookup", "DNS lookup", "nslookup <domain>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Server:         127.0.0.53\n";
            std::cout << "Address:        127.0.0.53#53\n\n";
            std::cout << "Name:      " << args[0] << "\n";
            std::cout << "Address:   192.168.1.1\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"dig", "DNS lookup", "dig <domain>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "; <<>> DiG 9.16.1 <<>> " << args[0] << "\n";
            std::cout << ";; global options: +cmd\n";
            std::cout << ";; Got answer:\n";
            std::cout << ";; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 12345\n";
            std::cout << ";; ANSWER SECTION:\n";
            std::cout << args[0] << ".    3600    IN    A    192.168.1.1\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"host", "DNS lookup", "host <domain>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << args[0] << " has address 192.168.1.1\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"traceroute", "Trace route", "traceroute <host>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "traceroute to " << args[0] << ", 30 hops max, 60 byte packets\n";
            std::cout << " 1  gateway (192.168.1.1)  0.1 ms  0.1 ms  0.1 ms\n";
            std::cout << " 2  * * *\n";
            std::cout << " 3  " << args[0] << " (192.168.1.1)  0.5 ms  0.5 ms  0.5 ms\n";
            return 0;
        }};
    registerCommand(cmd);
}

void CommandManager::registerTextCommands() {
    CommandInfo cmd;

    cmd = {"echo", "Print text", "echo [-n] [text]", 0, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
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
        }};
    registerCommand(cmd);

    cmd = {"printf", "Formatted output", "printf <format> [args]", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"grep", "Pattern matching", "grep <pattern> <file>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string pattern = args[0];
            std::string path = args[1];

            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(path, content)) {
                std::cerr << "grep: " << path << ": No such file\n";
                return 1;
            }

            std::stringstream ss(content);
            std::string line;
            while (std::getline(ss, line)) {
                if (line.find(pattern) != std::string::npos) {
                    std::cout << line << "\n";
                }
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"sed", "Stream editor", "sed <script> <file>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (vfs->readFile(args[1], content)) {
                std::cout << content;
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"awk", "Pattern scanning", "awk <pattern> <file>", 2, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (vfs->readFile(args[1], content)) {
                std::stringstream ss(content);
                std::string line;
                while (std::getline(ss, line)) {
                    std::cout << line << "\n";
                }
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"sort", "Sort lines", "sort [-r] <file>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool reverse = false;
            std::string path;

            for (const auto& arg : args) {
                if (arg == "-r") reverse = true;
                else path = arg;
            }

            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(path, content)) {
                std::cerr << "sort: " << path << ": No such file\n";
                return 1;
            }

            std::vector<std::string> lines;
            std::stringstream ss(content);
            std::string line;
            while (std::getline(ss, line)) {
                lines.push_back(line);
            }

            if (reverse) {
                std::sort(lines.rbegin(), lines.rend());
            } else {
                std::sort(lines.begin(), lines.end());
            }

            for (const auto& l : lines) {
                std::cout << l << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"uniq", "Remove duplicates", "uniq <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(args[0], content)) {
                std::cerr << "uniq: " << args[0] << ": No such file\n";
                return 1;
            }

            std::vector<std::string> lines;
            std::stringstream ss(content);
            std::string line, prev;
            while (std::getline(ss, line)) {
                if (line != prev) {
                    std::cout << line << "\n";
                    prev = line;
                }
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"cut", "Cut columns", "cut [-d] [-f] <file>", 1, 3, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            char delimiter = '\t';
            int field = 0;

            for (size_t i = 0; i + 1 < args.size(); ++i) {
                if (args[i] == "-d" && i + 1 < args.size()) {
                    delimiter = args[++i][0];
                }
                if (args[i] == "-f" && i + 1 < args.size()) {
                    field = std::stoi(args[++i]);
                }
            }

            std::string path = args.back();
            std::string content;
            auto vfs = sb->getFileSystem();
            if (vfs->readFile(path, content)) {
                std::cout << content;
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"tr", "Character translation", "tr <set1> [set2]", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string input;
            std::getline(std::cin, input);
            std::cout << input << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"rev", "Reverse lines", "rev <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (!vfs->readFile(args[0], content)) {
                std::cerr << "rev: " << args[0] << ": No such file\n";
                return 1;
            }

            std::stringstream ss(content);
            std::string line;
            while (std::getline(ss, line)) {
                std::reverse(line.begin(), line.end());
                std::cout << line << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"tee", "Read from stdin", "tee <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string input;
            while (std::getline(std::cin, input)) {
                std::cout << input << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);
}

void CommandManager::registerProcessCommands() {
    CommandInfo cmd;

    cmd = {"ps", "Process status", "ps [-aux]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            bool full = false;
            for (const auto& arg : args) {
                if (arg.find('a') != std::string::npos || arg.find('x') != std::string::npos) {
                    full = true;
                }
            }

            if (full) {
                std::cout << "  PID TTY          TIME CMD\n";
                std::cout << "    1 ?        00:00:00 init\n";
                std::cout << "  123 ?        00:00:00 bash\n";
                std::cout << "  456 pts/0   00:00:00 ps\n";
            } else {
                std::cout << "  PID TTY      TIME CMD\n";
                std::cout << "  456 pts/0   00:00:00 ps\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"top", "Process monitor", "top", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "top - 12:00:00 up 1 day,  3:42,  1 user,  load average: 0.10, 0.05, 0.01\n";
            std::cout << "Tasks:   1 total,   1 running,   0 sleeping,   0 stopped,   0 zombie\n";
            std::cout << "Cpu(s):  5.0%us,  1.0%sy,  0.0%ni, 94.0%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st\n";
            std::cout << "Mem:   1024000k total,   512000k used,   512000k free,        0k buffers\n";
            std::cout << "Swap:        0k total,        0k used,        0k free,   256000k cached\n\n";
            std::cout << "  PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+  COMMAND\n";
            std::cout << "  123 user      20   0   10240   2048   1024 R   0.0   0.2   0:00.00 bash\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"kill", "Terminate process", "kill [-signal] <pid>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int sig = 15;
            int pid = 0;

            for (size_t i = 0; i < args.size(); ++i) {
                if (args[i][0] == '-') {
                    sig = std::stoi(args[i].substr(1));
                } else {
                    pid = std::stoi(args[i]);
                }
            }

            if (pid == 0) {
                std::cerr << "kill: usage: kill [-signal] pid\n";
                return 1;
            }

            std::cout << "Sent signal " << sig << " to process " << pid << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"killall", "Kill processes by name", "killall <name>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Killed processes with name: " << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"pkill", "Kill by pattern", "pkill <pattern>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Sent signal to processes matching: " << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"bg", "Background process", "bg [job]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "[1]+ " << args[0] << " &\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"fg", "Foreground process", "fg [job]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"jobs", "List jobs", "jobs", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            auto jobs = sb->getBackgroundJobs();
            if (jobs.empty()) {
                std::cout << "No background jobs\n";
            } else {
                for (const auto& job : jobs) {
                    std::cout << "[" << job.first << "] Running " << job.second << "\n";
                }
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"nice", "Run with priority", "nice -n <priority> <command>", 2, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int priority = 0;
            size_t i = 0;

            if (args[0] == "-n" && args.size() > 1) {
                priority = std::stoi(args[1]);
                i = 2;
            }

            std::cout << "Running with nice value: " << priority << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"nohup", "Immune to hangup", "nohup <command>", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "nohup: ignoring input and appending output to 'nohup.out'\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"watch", "Execute periodically", "watch <command>", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "Every 2.0s: " << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"time", "Time command", "time <command>", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);
}

void CommandManager::registerMiscCommands() {
    CommandInfo cmd;

    cmd = {"help", "Show help", "help [command]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                printHelp();
            } else {
                printHelp(args[0]);
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"man", "Manual pages", "man <command>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "MANUAL PAGE FOR " << args[0] << "\n\n";
            std::cout << "NAME\n";
            std::cout << "    " << args[0] << " - " << getCommandDescription(args[0]) << "\n\n";
            std::cout << "SYNOPSIS\n";
            std::cout << "    " << args[0] << " [OPTIONS]\n\n";
            std::cout << "DESCRIPTION\n";
            std::cout << "    This is the manual page for the virtualSHELL " << args[0] << " command.\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"history", "Command history", "history [-c]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (!args.empty() && args[0] == "-c") {
                sb->clearHistory();
                std::cout << "History cleared\n";
                return 0;
            }

            auto hist = sb->getHistory(50);
            int i = 1;
            for (const auto& entry : hist) {
                std::cout << "  " << i++ << "  " << entry.command << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"version", "Show version", "version", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "virtualSHELL version 1.0.0\n";
            std::cout << "Copyright (c) 2026\n";
            std::cout << "License: MIT\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"info", "Show info", "info [command]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "virtualSHELL (virtualSHELL) 1.0.0\n\n";
            std::cout << "This is a virtual shell environment.\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"banner", "Display banner", "banner <text>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string text = args[0];
            std::string line1, line2, line3, line4, line5;

            for (char c : text) {
                switch (c) {
                    case 'A': case 'a':
                        line1 += "  A  "; line2 += " A A "; line3 += "AAAAA"; line4 += "A   A"; line5 += "A   A"; break;
                    case 'B': case 'b':
                        line1 += "BBBB "; line2 += "B  B "; line3 += "BBBB "; line4 += "B  B "; line5 += "BBBB "; break;
                    case 'C': case 'c':
                        line1 += " CCC "; line2 += "C    "; line3 += "C    "; line4 += "C    "; line5 += " CCC "; break;
                    case 'D': case 'd':
                        line1 += "DDD  "; line2 += "D  D "; line3 += "D  D "; line4 += "D  D "; line5 += "DDD  "; break;
                    case 'E': case 'e':
                        line1 += "EEEEE"; line2 += "E    "; line3 += "EEE  "; line4 += "E    "; line5 += "EEEEE"; break;
                    case 'H': case 'h':
                        line1 += "H   H"; line2 += "H   H"; line3 += "HHHHH"; line4 += "H   H"; line5 += "H   H"; break;
                    case 'I': case 'i':
                        line1 += "IIIII"; line2 += "  I  "; line3 += "  I  "; line4 += "  I  "; line5 += "IIIII"; break;
                    case 'L': case 'l':
                        line1 += "L    "; line2 += "L    "; line3 += "L    "; line4 += "L    "; line5 += "LLLLL"; break;
                    case 'M': case 'm':
                        line1 += "M   M"; line2 += "MM MM"; line3 += "M M M"; line4 += "M   M"; line5 += "M   M"; break;
                    case 'N': case 'n':
                        line1 += "N   N"; line2 += "NN  N"; line3 += "N N N"; line4 += "N  NN"; line5 += "N   N"; break;
                    case 'O': case 'o':
                        line1 += " OOO "; line2 += "O   O"; line3 += "O   O"; line4 += "O   O"; line5 += " OOO "; break;
                    case 'P': case 'p':
                        line1 += "PPPP "; line2 += "P   P"; line3 += "PPPP "; line4 += "P    "; line5 += "P    "; break;
                    case 'R': case 'r':
                        line1 += "RRR  "; line2 += "R  R "; line3 += "RRR  "; line4 += "R R  "; line5 += "R   R"; break;
                    case 'S': case 's':
                        line1 += " SSS "; line2 += "S    "; line3 += " SSS "; line4 += "    S"; line5 += " SSS "; break;
                    case 'T': case 't':
                        line1 += "TTTTT"; line2 += "  T  "; line3 += "  T  "; line4 += "  T  "; line5 += "  T  "; break;
                    case 'U': case 'u':
                        line1 += "U   U"; line2 += "U   U"; line3 += "U   U"; line4 += "U   U"; line5 += " UUU "; break;
                    case 'V': case 'v':
                        line1 += "V   V"; line2 += "V   V"; line3 += " V V "; line4 += " V V "; line5 += "  V  "; break;
                    case 'W': case 'w':
                        line1 += "W   W"; line2 += "W   W"; line3 += "W W W"; line4 += "WW WW"; line5 += "W   W"; break;
                    case 'X': case 'x':
                        line1 += "X   X"; line2 += " X X "; line3 += "  X  "; line4 += " X X "; line5 += "X   X"; break;
                    case 'Y': case 'y':
                        line1 += "Y   Y"; line2 += " Y Y "; line3 += "  Y  "; line4 += "  Y  "; line5 += "  Y  "; break;
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        line1 += "  0  "; line2 += "0   0"; line3 += "0   0"; line4 += "0   0"; line5 += "  0  "; break;
                    default:
                        line1 += "     "; line2 += "     "; line3 += "     "; line4 += "     "; line5 += "     "; break;
                }
            }

            std::cout << line1 << "\n" << line2 << "\n" << line3 << "\n" << line4 << "\n" << line5 << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {" cowsay", "Cow say", "cowsay <text>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string text = args[0];
            int width = 40;
            std::string line = "< " + text + " >";

            int padding = width - line.length() + 2;
            std::cout << " " << std::string(width, '_') << "\n";
            std::cout << "/" << std::string(padding, ' ') << "\\\n";
            std::cout << "|" << std::string(padding - 1, ' ') << "|\n";
            std::cout << "\\" << std::string(padding, ' ') << "/\n";
            std::cout << " " << std::string(width, '-') << "\n";
            std::cout << "        \\   ^__^\n";
            std::cout << "         \\  (oo)\\_______\n";
            std::cout << "            (__)\\       )\\/\\\n";
            std::cout << "                ||----w |\n";
            std::cout << "                ||     ||\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"yes", "Repeated output", "yes [string]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string text = args.empty() ? "y" : args[0];
            std::cout << "Use Ctrl+C to stop (simulated - limited to 10 lines)\n";
            for (int i = 0; i < 10; ++i) {
                std::cout << text << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"sleep", "Delay", "sleep <seconds>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            int seconds = std::stoi(args[0]);
            std::cout << "Sleeping for " << seconds << " seconds...\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"true", "Do nothing", "true", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"false", "Do nothing", "false", 0, 0, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"test", "Test condition", "test [expr]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"[[", "Test condition", "[[ expr ]]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"printf", "Format and print", "printf [format] [args]", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"read", "Read input", "read [-p] <var>", 1, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string prompt = "Enter value: ";
            std::string var = args.back();

            for (const auto& arg : args) {
                if (arg == "-p") {
                    size_t idx = arg.find(args[0]);
                }
            }

            std::cout << prompt;
            std::string value;
            std::getline(std::cin, value);
            sb->setEnv(var, value);
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"source", "Execute file", "source <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (vfs->readFile(args[0], content)) {
                std::cout << "Sourced: " << args[0] << "\n";
                return 0;
            }
            std::cerr << "source: " << args[0] << ": No such file\n";
            return 1;
        }};
    registerCommand(cmd);

    cmd = {".", "Execute file", ". <file>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string content;
            auto vfs = sb->getFileSystem();
            if (vfs->readFile(args[0], content)) {
                return 0;
            }
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"hash", "Hash commands", "hash [-r] [command]", 0, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                std::cout << "hits    command\n";
                std::cout << "   1    /bin/ls\n";
                std::cout << "   1    /bin/cat\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"return", "Return function", "return [n]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return args.empty() ? 0 : std::stoi(args[0]);
        }};
    registerCommand(cmd);

    cmd = {"local", "Local variable", "local [name=value]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"declare", "Declare variable", "declare [-x] [name=value]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"export", "Export variable", "export [name=value]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"readonly", "Readonly variable", "readonly [name=value]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"typeset", "Type variable", "typeset [options] [name=value]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"shift", "Shift positional params", "shift [n]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"set", "Set options", "set [options]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                auto env = sb->getAllEnv();
                for (const auto& e : env) {
                    std::cout << e.first << "=" << e.second << "\n";
                }
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"unset", "Unset variable", "unset <name>", 1, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            sb->unsetEnv(args[0]);
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"trap", "Signal handler", "trap [action] [sigs]", 0, 2, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"umask", "File creation mask", "umask [mask]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            if (args.empty()) {
                std::cout << "0022\n";
            } else {
                std::cout << "umask set to " << args[0] << "\n";
            }
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"wait", "Wait for process", "wait [pid]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"xargs", "Build command", "xargs <command>", 1, 999, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::cout << "xargs: executing " << args[0] << "\n";
            return 0;
        }};
    registerCommand(cmd);

    cmd = {"save", "Save virtualSHELL", "save [file]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string path = args.empty() ? "data/save.dat" : args[0];
            if (sb->saveState(path)) return 0;
            return 1;
        }};
    registerCommand(cmd);

    cmd = {"load", "Load virtualSHELL", "load [file]", 0, 1, false,
        [this](std::vector<std::string>& args, SandboxManager* sb) {
            std::string path = args.empty() ? "data/save.dat" : args[0];
            if (sb->loadState(path)) return 0;
            return 1;
        }};
    registerCommand(cmd);
}

int CommandManager::executeCommand(const std::string& cmdLine) {
    std::vector<std::string> args = parseCommandLine(cmdLine);

    if (args.empty()) {
        return 0;
    }

    std::string cmdName = expandAlias(args[0]);
    args[0] = cmdName;

    auto cmdIt = commands.find(cmdName);
    if (cmdIt == commands.end()) {
        std::cerr << cmdName << ": command not found\n";
        return 127;
    }

    const CommandInfo& cmd = cmdIt->second;

    if (cmd.minArgs > 0 && (int)args.size() - 1 < cmd.minArgs) {
        std::cerr << cmdName << ": missing arguments\n";
        std::cerr << "Usage: " << cmd.usage << "\n";
        return 1;
    }

    if ((int)args.size() - 1 > cmd.maxArgs) {
        std::cerr << cmdName << ": too many arguments\n";
        std::cerr << "Usage: " << cmd.usage << "\n";
        return 1;
    }

    if (cmd.requiresRoot && sandbox->getUser() != "root") {
        std::cerr << cmdName << ": permission denied (requires root)\n";
        return 1;
    }

    if (!loadedCommands[cmdName]) {
        loadedCommands[cmdName] = true;
        Logger::logLoad("/bin/" + cmdName, true);
    }

    int result = cmd.handler(args, sandbox);
    sandbox->setLastExitCode(result);

    return result;
}

std::vector<std::string> CommandManager::parseCommandLine(const std::string& cmdLine) const {
    std::vector<std::string> args;
    std::string current;
    bool inQuotes = false;
    char quoteChar = 0;

    for (size_t i = 0; i < cmdLine.length(); ++i) {
        char c = cmdLine[i];

        if ((c == '"' || c == '\'') && !inQuotes) {
            inQuotes = true;
            quoteChar = c;
        } else if (c == quoteChar && inQuotes) {
            inQuotes = false;
            quoteChar = 0;
        } else if (c == ' ' && !inQuotes) {
            if (!current.empty()) {
                args.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        args.push_back(current);
    }

    return args;
}

std::string CommandManager::expandAlias(const std::string& cmd) const {
    std::string alias = sandbox->getAlias(cmd);
    return alias.empty() ? cmd : alias;
}

std::string CommandManager::expandVariables(const std::string& str) const {
    std::string result = str;
    size_t pos = 0;

    while ((pos = result.find('$', pos)) != std::string::npos) {
        size_t end = pos + 1;
        while (end < result.length() && (isalnum(result[end]) || result[end] == '_')) {
            end++;
        }

        std::string var = result.substr(pos + 1, end - pos - 1);
        std::string value = sandbox->getEnv(var);

        if (!value.empty()) {
            result.replace(pos, end - pos, value);
            pos += value.length();
        } else {
            pos = end;
        }
    }

    return result;
}

std::vector<std::string> CommandManager::getCommandNames() const {
    std::vector<std::string> names;
    for (const auto& cmd : commands) {
        names.push_back(cmd.first);
    }
    return names;
}

std::string CommandManager::getCommandDescription(const std::string& cmdName) const {
    auto it = commands.find(cmdName);
    if (it != commands.end()) {
        return it->second.description;
    }
    return "";
}

void CommandManager::printHelp() const {
    std::cout << "virtualSHELL - Available commands:\n\n";

    std::vector<std::string> names = getCommandNames();
    std::sort(names.begin(), names.end());

    int count = 0;
    for (const auto& name : names) {
        std::cout << "  " << name << " - " << commands.at(name).description << "\n";
        count++;
    }

    std::cout << "\nTotal: " << count << " commands\n";
    std::cout << "Type 'help <command>' for more information.\n";
}

void CommandManager::printHelp(const std::string& command) const {
    auto it = commands.find(command);
    if (it != commands.end()) {
        std::cout << "NAME\n";
        std::cout << "    " << command << " - " << it->second.description << "\n\n";
        std::cout << "SYNOPSIS\n";
        std::cout << "    " << it->second.usage << "\n\n";
    } else {
        std::cerr << "help: no help topics match '" << command << "'\n";
    }
}

void CommandManager::loadAllCommands() {
    for (auto& cmd : commands) {
        if (!loadedCommands[cmd.first]) {
            loadedCommands[cmd.first] = true;
            Logger::logLoad("/bin/" + cmd.first, true);
        }
    }
}

void CommandManager::loadCommand(const std::string& commandName) {
    if (commands.find(commandName) != commands.end() && !loadedCommands[commandName]) {
        loadedCommands[commandName] = true;
        Logger::logLoad("/bin/" + commandName, true);
    }
}

bool CommandManager::isCommandLoaded(const std::string& cmdName) const {
    auto it = loadedCommands.find(cmdName);
    if (it != loadedCommands.end()) {
        return it->second;
    }
    return false;
}

std::vector<std::string> CommandManager::getLoadedCommands() const {
    std::vector<std::string> loaded;
    for (const auto& cmd : loadedCommands) {
        if (cmd.second) {
            loaded.push_back(cmd.first);
        }
    }
    return loaded;
}

std::vector<std::string> CommandManager::getUnloadedCommands() const {
    std::vector<std::string> unloaded;
    for (const auto& cmd : loadedCommands) {
        if (!cmd.second) {
            unloaded.push_back(cmd.first);
        }
    }
    return unloaded;
}

void CommandManager::unloadCommand(const std::string& cmdName) {
    if (loadedCommands.find(cmdName) != loadedCommands.end()) {
        loadedCommands[cmdName] = false;
    }
}

void CommandManager::unloadAllCommands() {
    for (auto& cmd : loadedCommands) {
        cmd.second = false;
    }
}
