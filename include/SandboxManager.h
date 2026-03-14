#ifndef SANDBOXMANAGER_H
#define SANDBOXMANAGER_H

#include <string>
#include <vector>
#include <map>
#include "VirtualFileSystem.h"

class SandboxManager {
public:
    struct HistoryEntry {
        std::string command;
        std::string timestamp;
        int exitCode;
    };

    struct EnvironmentVar {
        std::string name;
        std::string value;
    };

    SandboxManager();
    ~SandboxManager();

    VirtualFileSystem* getFileSystem();

    void addToHistory(const std::string& command, int exitCode = 0);
    std::vector<HistoryEntry> getHistory(int limit = 100) const;
    void clearHistory();

    std::string getEnv(const std::string& name) const;
    void setEnv(const std::string& name, const std::string& value);
    void unsetEnv(const std::string& name);
    std::map<std::string, std::string> getAllEnv() const;

    void setWorkingDirectory(const std::string& path);
    std::string getWorkingDirectory() const;

    int getLastExitCode() const;
    void setLastExitCode(int code);

    bool saveState(const std::string& filepath);
    bool loadState(const std::string& filepath);

    void setUser(const std::string& user);
    std::string getUser() const;
    void setHostname(const std::string& host);
    std::string getHostname() const;

    void setAliases(const std::string& name, const std::string& value);
    std::string getAlias(const std::string& name) const;
    void removeAlias(const std::string& name);
    std::map<std::string, std::string> getAliases() const;

    void addBackgroundJob(int pid, const std::string& cmd);
    void removeBackgroundJob(int pid);
    std::vector<std::pair<int, std::string>> getBackgroundJobs() const;

private:
    VirtualFileSystem* vfs;
    std::vector<HistoryEntry> history;
    std::map<std::string, std::string> envVars;
    std::map<std::string, std::string> aliases;
    std::vector<std::pair<int, std::string>> backgroundJobs;
    std::string workingDirectory;
    std::string currentUser;
    std::string hostname;
    int lastExitCode;

    void initializeDefaultEnv();
    std::string getCurrentTimestamp();
};

#endif
