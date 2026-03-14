#!/usr/bin/env python3
import os

commands = [
    # File commands
    ("ls", "List directory contents", "ls [-la] [path]", 0, 2, False),
    ("cd", "Change current directory", "cd [path]", 0, 1, False),
    ("pwd", "Print working directory", "pwd", 0, 0, False),
    ("mkdir", "Create directories", "mkdir [-p] <dir>", 1, 2, False),
    ("rmdir", "Remove directories", "rmdir <dir>", 1, 1, False),
    ("rm", "Remove files", "rm [-rf] <file>", 1, 2, False),
    ("touch", "Create empty files", "touch <file>", 1, 1, False),
    ("cat", "Display file contents", "cat <file>", 1, 1, False),
    ("cp", "Copy files", "cp <src> <dst>", 2, 2, False),
    ("mv", "Move/rename files", "mv <src> <dst>", 2, 2, False),
    ("ln", "Create links", "ln -s <target> <link>", 2, 3, False),
    ("chmod", "Change permissions", "chmod <perm> <file>", 2, 2, False),
    ("chown", "Change owner", "chown <owner> <file>", 2, 2, True),
    ("tree", "Display directory tree", "tree [path]", 0, 1, False),
    ("find", "Search for files", "find <path> <name>", 2, 2, False),
    ("du", "Disk usage", "du [path]", 0, 1, False),
    ("df", "Disk filesystem", "df", 0, 0, False),
    ("stat", "Display file status", "stat <file>", 1, 1, False),
    ("wc", "Word count", "wc [-l] <file>", 1, 2, False),
    ("head", "Display first lines", "head [-n] <file>", 1, 2, False),
    ("tail", "Display last lines", "tail [-n] <file>", 1, 2, False),
    # System commands
    ("sudo", "Execute as superuser", "sudo <command>", 1, 999, False),
    ("apt", "Package manager", "apt <action> [packages]", 1, 999, False),
    ("pkg", "Package manager (BSD style)", "pkg <action> [packages]", 1, 999, False),
    ("uname", "System information", "uname [-a]", 0, 1, False),
    ("whoami", "Print current user", "whoami", 0, 0, False),
    ("hostname", "Print system hostname", "hostname [name]", 0, 1, False),
    ("date", "Print date and time", "date [+format]", 0, 1, False),
    ("cal", "Display calendar", "cal [month] [year]", 0, 2, False),
    ("uptime", "System uptime", "uptime", 0, 0, False),
    ("id", "Print user identity", "id [user]", 0, 1, False),
    ("groups", "Print user groups", "groups [user]", 0, 1, False),
    ("passwd", "Change password", "passwd [user]", 0, 1, False),
    ("su", "Switch user", "su [user]", 0, 1, False),
    ("which", "Locate command", "which <command>", 1, 1, False),
    ("whereis", "Locate command and docs", "whereis <command>", 1, 1, False),
    ("type", "Command type", "type <command>", 1, 1, False),
    ("alias", "Create command alias", "alias [name='command']", 0, 1, False),
    ("unalias", "Remove alias", "unalias <name>", 1, 1, False),
    ("env", "Print environment", "env", 0, 0, False),
    ("export", "Set environment variable", "export [NAME=value]", 0, 1, False),
    ("unset", "Remove environment variable", "unset <name>", 1, 1, False),
    ("exit", "Exit shell", "exit [code]", 0, 1, False),
    ("logout", "Logout", "logout", 0, 0, False),
    ("shutdown", "Shutdown system", "shutdown [-h] [-r] [time]", 0, 2, True),
    ("reboot", "Reboot system", "reboot", 0, 0, True),
    ("halt", "Halt system", "halt", 0, 0, True),
    ("poweroff", "Power off system", "poweroff", 0, 0, True),
    ("systemctl", "Systemd control", "systemctl <action> [service]", 1, 2, False),
    ("service", "Run system service", "service <name> <action>", 2, 2, False),
    ("clear", "Clear screen", "clear", 0, 0, False),
    ("reset", "Reset terminal", "reset", 0, 0, False),
    # Network commands
    ("ping", "Send ICMP echo", "ping [-c] <host>", 1, 2, False),
    ("curl", "Transfer data", "curl [-o] <url>", 1, 2, False),
    ("wget", "Download files", "wget <url>", 1, 1, False),
    ("ssh", "Secure shell", "ssh [user@]host", 1, 1, False),
    ("scp", "Secure copy", "scp <src> <dst>", 2, 2, False),
    ("ftp", "FTP client", "ftp <host>", 1, 1, False),
    ("netstat", "Network statistics", "netstat [-a]", 0, 1, False),
    ("ifconfig", "Configure network", "ifconfig [interface]", 0, 1, False),
    ("ip", "IP command", "ip [addr|route|link]", 1, 3, False),
    ("nslookup", "DNS lookup", "nslookup <domain>", 1, 1, False),
    ("dig", "DNS lookup", "dig <domain>", 1, 1, False),
    ("host", "DNS lookup", "host <domain>", 1, 1, False),
    ("traceroute", "Trace route", "traceroute <host>", 1, 1, False),
    # Text commands
    ("echo", "Print text", "echo [-n] [text]", 0, 999, False),
    ("printf", "Formatted output", "printf <format> [args]", 1, 999, False),
    ("grep", "Pattern matching", "grep <pattern> <file>", 2, 2, False),
    ("sed", "Stream editor", "sed <script> <file>", 2, 2, False),
    ("awk", "Pattern scanning", "awk <pattern> <file>", 2, 2, False),
    ("sort", "Sort lines", "sort [-r] <file>", 1, 2, False),
    ("uniq", "Remove duplicates", "uniq <file>", 1, 1, False),
    ("cut", "Cut columns", "cut [-d] [-f] <file>", 1, 3, False),
    ("tr", "Character translation", "tr <set1> [set2]", 1, 2, False),
    ("rev", "Reverse lines", "rev <file>", 1, 1, False),
    ("tee", "Read from stdin", "tee <file>", 1, 1, False),
    # Process commands
    ("ps", "Process status", "ps [-aux]", 0, 1, False),
    ("top", "Process monitor", "top", 0, 0, False),
    ("kill", "Terminate process", "kill [-signal] <pid>", 1, 2, False),
    ("killall", "Kill processes by name", "killall <name>", 1, 1, False),
    ("pkill", "Kill by pattern", "pkill <pattern>", 1, 1, False),
    ("bg", "Background process", "bg [job]", 0, 1, False),
    ("fg", "Foreground process", "fg [job]", 0, 1, False),
    ("jobs", "List jobs", "jobs", 0, 0, False),
    ("nice", "Run with priority", "nice -n <priority> <command>", 2, 999, False),
    ("nohup", "Immune to hangup", "nohup <command>", 1, 999, False),
    ("watch", "Execute periodically", "watch <command>", 1, 999, False),
    ("time", "Time command", "time <command>", 1, 999, False),
    # Misc commands
    ("help", "Show help", "help [command]", 0, 1, False),
    ("man", "Manual pages", "man <command>", 1, 1, False),
    ("history", "Command history", "history [-c]", 0, 1, False),
    ("version", "Show version", "version", 0, 0, False),
    ("info", "Show info", "info [command]", 0, 1, False),
    ("banner", "Display banner", "banner <text>", 1, 1, False),
    ("cowsay", "Cow say", "cowsay <text>", 1, 1, False),
    ("yes", "Repeated output", "yes [string]", 0, 1, False),
    ("sleep", "Delay", "sleep <seconds>", 1, 1, False),
    ("true", "Do nothing", "true", 0, 0, False),
    ("false", "Do nothing", "false", 0, 0, False),
    ("test", "Test condition", "test [expr]", 0, 1, False),
    ("read", "Read input", "read [-p] <var>", 1, 2, False),
    ("source", "Execute file", "source <file>", 1, 1, False),
    ("hash", "Hash commands", "hash [-r] [command]", 0, 2, False),
    ("return", "Return function", "return [n]", 0, 1, False),
    ("local", "Local variable", "local [name=value]", 0, 1, False),
    ("declare", "Declare variable", "declare [-x] [name=value]", 0, 1, False),
    ("readonly", "Readonly variable", "readonly [name=value]", 0, 1, False),
    ("typeset", "Type variable", "typeset [options] [name=value]", 0, 1, False),
    ("shift", "Shift positional params", "shift [n]", 0, 1, False),
    ("set", "Set options", "set [options]", 0, 1, False),
    ("trap", "Signal handler", "trap [action] [sigs]", 0, 2, False),
    ("umask", "File creation mask", "umask [mask]", 0, 1, False),
    ("wait", "Wait for process", "wait [pid]", 0, 1, False),
    ("xargs", "Build command", "xargs <command>", 1, 999, False),
    ("save", "Save virtualSHELL", "save [file]", 0, 1, False),
    ("load", "Load virtualSHELL", "load [file]", 0, 1, False),
]

base_dir = "/home/ngquanghuy/miniShell/bin/commands"
os.makedirs(base_dir, exist_ok=True)

template_header = """#ifndef {classname_upper}_H
#define {classname_upper}_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cctype>
#include "Command.h"

class {classname} : public Command {{
public:
    std::string getName() const override {{ return "{name}"; }}
    std::string getDescription() const override {{ return "{description}"; }}
    std::string getUsage() const override {{ return "{usage}"; }}
    int getMinArgs() const override {{ return {min_args}; }}
    int getMaxArgs() const override {{ return {max_args}; }}
    bool requiresRoot() const override {{ return {requires_root}; }}
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
}};

#endif
"""

template_impl = """#include "{classname}.h"

int {classname}::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {{
    // Implementation for {name}
{implementation}
}}
"""

# Generate command implementations (simplified placeholder)
def generate_implementation(name, usage):
    impl = '    return 0;'
    
    if name == "ls":
        impl = '''    bool showAll = false;
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
            std::cout << f << (vfs->isDirectory(fullPath) ? "/" : "") << "\\n";
        }
    } else {
        for (const auto& f : files) {
            if (!showAll && f[0] == '.') continue;
            std::cout << f << (vfs->isDirectory(vfs->getAbsolutePath(path) + "/" + f) ? "/" : "") << " ";
        }
        std::cout << "\\n";
    }
    return 0;'''
    
    elif name == "cd":
        impl = '''    std::string path = args.empty() ? "/home/user" : args[0];
    auto vfs = sandbox->getFileSystem();
    if (vfs->changeDirectory(path)) {
        sandbox->setWorkingDirectory(vfs->getCurrentPath());
        return 0;
    }
    std::cerr << "cd: " << path << ": No such directory\\n";
    return 1;'''
    
    elif name == "pwd":
        impl = '''    std::cout << sandbox->getWorkingDirectory() << "\\n";
    return 0;'''
    
    elif name == "echo":
        impl = '''    bool noNewline = false;
    for (const auto& arg : args) {
        if (arg == "-n") {
            noNewline = true;
            continue;
        }
        std::cout << arg << " ";
    }
    if (!noNewline) std::cout << "\\n";
    return 0;'''
    
    elif name == "whoami":
        impl = '''    std::cout << sandbox->getUser() << "\\n";
    return 0;'''
    
    elif name == "date":
        impl = '''    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::put_time(std::localtime(&time), "%a %b %d %H:%M:%S %Z %Y") << "\\n";
    return 0;'''
    
    elif name == "uname":
        impl = '''    bool all = false;
    for (const auto& arg : args) {
        if (arg == "-a") all = true;
    }
    if (all) {
        std::cout << "Linux virtualshell 5.0.0-virtual #1 SMP Mon Jan 1 00:00:00 UTC 2026 x86_64 GNU/Linux\\n";
    } else {
        std::cout << "Linux\\n";
    }
    return 0;'''
        
    return impl

# Create Command.h
command_h = '''#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class SandboxManager;

class Command {
public:
    virtual ~Command() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getUsage() const = 0;
    virtual int getMinArgs() const = 0;
    virtual int getMaxArgs() const = 0;
    virtual bool requiresRoot() const = 0;
    virtual int execute(const std::vector<std::string>& args, SandboxManager* sandbox) = 0;
};

#endif
'''

with open(f"{base_dir}/Command.h", "w") as f:
    f.write(command_h)

count = 0
for name, description, usage, min_args, max_args, requires_root in commands:
    classname = f"Cmd_{name.replace('-', '_')}"
    classname_upper = classname.upper()
    
    header_content = template_header.format(
        classname=classname,
        classname_upper=classname_upper,
        name=name,
        description=description,
        usage=usage,
        min_args=min_args,
        max_args=max_args,
        requires_root="true" if requires_root else "false"
    )
    
    impl_content = template_impl.format(
        classname=classname,
        name=name,
        implementation=generate_implementation(name, usage)
    )
    
    header_file = f"{base_dir}/{classname}.h"
    impl_file = f"{base_dir}/{classname}.cpp"
    
    with open(header_file, "w") as f:
        f.write(header_content)
    
    with open(impl_file, "w") as f:
        f.write(impl_content)
    
    count += 1

print(f"Generated {count} command files in {base_dir}")
