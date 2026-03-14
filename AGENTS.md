# AGENTS.md - virtualSHELL Development Guide

## Overview
virtualSHELL is a C++17 virtual shell implementation with sandbox filesystem, command registry, and persistence.

## Project Structure
```
/home/ngquanghuy/d/
├── include/              # Header files (.h)
├── src/                  # Implementation files (.cpp)
├── bin/commands/         # Individual command implementations
│   └── Cmd_*.h/cpp      # One class per command file
├── data/                 # Runtime data (sandbox persistence)
├── Makefile             # Build system
└── virtualSHELL         # Compiled binary
```

## Build Commands

### Full Build
```bash
make clean && make
```

### Run the Shell
```bash
./virtualSHELL              # Interactive mode
./virtualSHELL -c "ls"     # Single command mode
./virtualSHELL -c "help"    # Get help
./virtualSHELL -v           # Version info
```

### Individual Compilation
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/Logger.cpp -o obj/Logger.o
g++ -std=c++17 -Wall -Wextra -Iinclude -o virtualSHELL obj/*.o
```

### Clean Build Artifacts
```bash
make clean
```

## Code Style Guidelines

### General Principles
- Use C++17 standard (`-std=c++17`)
- Keep files under 2000 lines
- Prefer clear, human-readable code over clever optimizations
- Add comments only for complex/important sections

### Header Files (.h)
- Use include guards: `#ifndef FILENAME_H` / `#define FILENAME_H` / `#endif`
- Order includes: standard library → project headers → forward declarations
- Use forward declarations for `class SandboxManager;` to avoid circular dependencies

```cpp
#ifndef MYCLASS_H
#define MYCLASS_H

#include <string>
#include <vector>
#include "OtherProjectHeader.h"

class SandboxManager;

class MyClass {
public:
    void doSomething();
    
private:
    int privateMethod();
};

#endif
```

### Implementation Files (.cpp)
- Include corresponding header first
- Use meaningful variable names
- Avoid magic numbers; use constants

```cpp
#include "MyClass.h"

void MyClass::doSomething() {
    // Implementation
}
```

### Naming Conventions
| Element | Convention | Example |
|---------|------------|---------|
| Classes | PascalCase | `VirtualFileSystem` |
| Methods/Functions | PascalCase | `getWorkingDirectory()` |
| Variables | snake_case | `current_directory` |
| Constants | UPPER_SNAKE | `MAX_BUFFER_SIZE` |
| Files | PascalCase or snake_case | `Logger.h`, `cmd_ls.cpp` |
| Commands | Prefix with Cmd_ | `Cmd_ls`, `Cmd_cd` |

### Types and Templates
- Use `std::string` instead of C-style `char*`
- Use `std::vector<T>` for dynamic arrays
- Use `std::unique_ptr<T>` for exclusive ownership
- Use `std::shared_ptr<T>` for shared ownership

### Error Handling
- Return error codes (0 = success, non-zero = error)
- Use `std::cerr` for error messages
- Log errors with `Logger::log(Logger::Level::ERROR, message)`

```cpp
int myFunction() {
    if (somethingFailed) {
        std::cerr << "Error: operation failed\n";
        return 1;
    }
    return 0;
}
```

### Command Implementation
Each command is a class inheriting from `Command`:

```cpp
// In bin/commands/Cmd_example.h
class Cmd_example : public Command {
public:
    std::string getName() const override { return "example"; }
    std::string getDescription() const override { return "Does something"; }
    std::string getUsage() const override { return "example <arg>"; }
    int getMinArgs() const override { return 1; }
    int getMaxArgs() const override { return 2; }
    bool requiresRoot() const override { return false; }
    
    int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override;
};
```

```cpp
// In bin/commands/Cmd_example.cpp
#include "Cmd_example.h"

int Cmd_example::execute(const std::vector<std::string>& args, SandboxManager* sandbox) {
    // Access filesystem via: sandbox->getFileSystem()
    // Access env vars via: sandbox->getEnv(), sandbox->setEnv()
    return 0;
}
```

## Testing

### Manual Testing
```bash
# Test single command
./virtualSHELL -c "ls"

# Test interactive mode
echo -e "ls\npwd\nexit" | ./virtualSHELL

# Test error cases
./virtualSHELL -c "cat nonexistent_file"
```

### Build Verification
```bash
# Should compile without errors
make clean && make

# Test binary runs
./virtualSHELL -c "version"
```

## Common Patterns

### Accessing Sandbox Services
```cpp
VirtualFileSystem* vfs = sandbox->getFileSystem();
std::string user = sandbox->getUser();
sandbox->setEnv("VAR", "value");
sandbox->addToHistory("command", exitCode);
```

### Filesystem Operations
```cpp
auto vfs = sandbox->getFileSystem();
vfs->createFile("/path/file.txt", "content");
vfs->readFile("/path/file.txt", content);
vfs->createDirectory("/path/dir");
vfs->exists("/path");
```

### Logging
```cpp
#include "Logger.h"
Logger::log(Logger::Level::INFO, "Starting operation");
Logger::log(Logger::Level::SUCCESS, "Operation completed");
Logger::log(Logger::Level::ERROR, "Operation failed");
Logger::logLoad("/bin/command", true);  // Colored output
```

## Important Notes

1. **Include Paths**: Headers are in `include/`, use `#include "Header.h"`
2. **No External Dependencies**: This is a standalone C++ project - no CMake, no vcpkg
3. **Single Binary**: All object files link into single `virtualSHELL` executable
4. **Persistence**: Sandbox state saves to `data/save.dat` on exit
5. **ANSI Colors**: Use `\033[32m` for green, `\033[31m` for red, etc.
