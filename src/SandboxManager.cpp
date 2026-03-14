#include "SandboxManager.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

SandboxManager::SandboxManager(const std::string& configPath) 
    : vfs(nullptr), configManager(nullptr), lastExitCode(0) {
    
    configManager = new ConfigManager();
    configManager->loadFromFile(configPath);

    ConfigManager::UserConfig userCfg = configManager->getUserConfig();
    ConfigManager::LoggingConfig logCfg = configManager->getLoggingConfig();

    currentUser = userCfg.username;
    hostname = userCfg.hostname;
    workingDirectory = userCfg.home;

    Logger::setVerbose(logCfg.verbose);

    vfs = new VirtualFileSystem();

    ConfigManager::SandboxConfig sandCfg = configManager->getSandboxConfig();
    if (sandCfg.auto_save) {
        vfs->loadFromFile(sandCfg.data_file + ".vfs");
    }

    auto fsCfg = configManager->getFilesystemConfig();
    auto cmdCfg = configManager->getCommandsConfig();

    for (const auto& alias : cmdCfg.default_aliases) {
        aliases[alias.first] = alias.second;
    }

    initializeDefaultEnv();
}

SandboxManager::~SandboxManager() {
    delete vfs;
    delete configManager;
}

void SandboxManager::initializeDefaultEnv() {
    envVars["HOME"] = "/home/user";
    envVars["USER"] = "user";
    envVars["SHELL"] = "/bin/virtualsh";
    envVars["PATH"] = "/bin:/usr/bin:/usr/local/bin";
    envVars["PWD"] = "/home/user";
    envVars["HOSTNAME"] = "virtualshell";
    envVars["LANG"] = "en_US.UTF-8";
    envVars["TERM"] = "xterm-256color";
    envVars["HISTFILE"] = "/home/user/.history";
    envVars["HISTFILESIZE"] = "1000";
    envVars["HISTSIZE"] = "1000";
    envVars["EDITOR"] = "vim";
    envVars["VISUAL"] = "vim";
    envVars["PAGER"] = "less";
    envVars["LS_OPTIONS"] = "-F";
    envVars["GREP_OPTIONS"] = "--color=auto";
    envVars["CLICOLOR"] = "1";
}

std::string SandboxManager::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

VirtualFileSystem* SandboxManager::getFileSystem() {
    return vfs;
}

ConfigManager* SandboxManager::getConfigManager() {
    return configManager;
}

void SandboxManager::addToHistory(const std::string& command, int exitCode) {
    HistoryEntry entry;
    entry.command = command;
    entry.timestamp = getCurrentTimestamp();
    entry.exitCode = exitCode;
    history.push_back(entry);

    if (history.size() > 1000) {
        history.erase(history.begin());
    }
}

std::vector<SandboxManager::HistoryEntry> SandboxManager::getHistory(int limit) const {
    if (limit <= 0 || (size_t)limit >= history.size()) {
        return history;
    }
    std::vector<HistoryEntry> result(history.end() - limit, history.end());
    return result;
}

void SandboxManager::clearHistory() {
    history.clear();
}

std::string SandboxManager::getEnv(const std::string& name) const {
    auto it = envVars.find(name);
    if (it != envVars.end()) {
        return it->second;
    }
    return "";
}

void SandboxManager::setEnv(const std::string& name, const std::string& value) {
    envVars[name] = value;
    if (name == "PWD") {
        workingDirectory = value;
    }
}

void SandboxManager::unsetEnv(const std::string& name) {
    envVars.erase(name);
}

std::map<std::string, std::string> SandboxManager::getAllEnv() const {
    return envVars;
}

void SandboxManager::setWorkingDirectory(const std::string& path) {
    workingDirectory = path;
    envVars["PWD"] = path;
}

std::string SandboxManager::getWorkingDirectory() const {
    return workingDirectory;
}

int SandboxManager::getLastExitCode() const {
    return lastExitCode;
}

void SandboxManager::setLastExitCode(int code) {
    lastExitCode = code;
}

bool SandboxManager::saveState(const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        return false;
    }

    out << "# virtualSHELL State File\n";
    out << "# Do not edit manually\n\n";

    out << "[ENVIRONMENT]\n";
    for (const auto& env : envVars) {
        out << env.first << "=" << env.second << "\n";
    }

    out << "\n[ALIASES]\n";
    for (const auto& alias : aliases) {
        out << alias.first << "=" << alias.second << "\n";
    }

    out << "\n[HISTORY]\n";
    for (const auto& entry : history) {
        out << entry.timestamp << "|" << entry.exitCode << "|" << entry.command << "\n";
    }

    out << "\n[USER]\n";
    out << "user=" << currentUser << "\n";
    out << "hostname=" << hostname << "\n";

    out.close();

    if (vfs) {
        std::string vfsFile = filepath + ".vfs";
        vfs->saveToFile(vfsFile);
    }

    Logger::log(Logger::Level::SUCCESS, "Sandbox state saved to " + filepath);
    return true;
}

bool SandboxManager::loadState(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) {
        return false;
    }

    std::string line;
    std::string currentSection;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line == "[ENVIRONMENT]") {
            currentSection = "ENV";
            continue;
        } else if (line == "[ALIASES]") {
            currentSection = "ALIAS";
            continue;
        } else if (line == "[HISTORY]") {
            currentSection = "HIST";
            continue;
        } else if (line == "[USER]") {
            currentSection = "USER";
            continue;
        }

        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (currentSection == "ENV") {
            envVars[key] = value;
        } else if (currentSection == "ALIAS") {
            aliases[key] = value;
        } else if (currentSection == "HIST") {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                HistoryEntry entry;
                entry.timestamp = line.substr(0, pos1);
                entry.exitCode = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                entry.command = line.substr(pos2 + 1);
                history.push_back(entry);
            }
        } else if (currentSection == "USER") {
            if (key == "user") currentUser = value;
            else if (key == "hostname") hostname = value;
        }
    }

    in.close();

    std::string vfsFile = filepath + ".vfs";
    if (vfs) {
        vfs->loadFromFile(vfsFile);
        vfs->changeDirectory(workingDirectory);
    }

    Logger::log(Logger::Level::SUCCESS, "Sandbox state loaded from " + filepath);
    return true;
}

void SandboxManager::setUser(const std::string& user) {
    currentUser = user;
    envVars["USER"] = user;
}

std::string SandboxManager::getUser() const {
    return currentUser;
}

void SandboxManager::setHostname(const std::string& host) {
    hostname = host;
    envVars["HOSTNAME"] = host;
}

std::string SandboxManager::getHostname() const {
    return hostname;
}

void SandboxManager::setAliases(const std::string& name, const std::string& value) {
    aliases[name] = value;
}

std::string SandboxManager::getAlias(const std::string& name) const {
    auto it = aliases.find(name);
    if (it != aliases.end()) {
        return it->second;
    }
    return "";
}

void SandboxManager::removeAlias(const std::string& name) {
    aliases.erase(name);
}

std::map<std::string, std::string> SandboxManager::getAliases() const {
    return aliases;
}

void SandboxManager::addBackgroundJob(int pid, const std::string& cmd) {
    backgroundJobs.push_back({pid, cmd});
}

void SandboxManager::removeBackgroundJob(int pid) {
    backgroundJobs.erase(
        std::remove_if(backgroundJobs.begin(), backgroundJobs.end(),
            [pid](const std::pair<int, std::string>& job) {
                return job.first == pid;
            }),
        backgroundJobs.end()
    );
}

std::vector<std::pair<int, std::string>> SandboxManager::getBackgroundJobs() const {
    return backgroundJobs;
}
