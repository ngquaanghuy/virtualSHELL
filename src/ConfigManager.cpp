#include "ConfigManager.h"
#include <iostream>
#include <algorithm>

ConfigManager::ConfigManager() {
    applyDefaultConfig();
}

ConfigManager::~ConfigManager() {
}

void ConfigManager::applyDefaultConfig() {
    shellConfig = {"virtualSHELL", "1.0.0"};
    userConfig = {"user", "virtualshell", "/home/user", "/bin/virtualsh"};
    promptConfig = {"\\u@\\h:\\w\\$ ", "green", true, true};
    sandboxConfig = {true, true, "data/save.dat", 1000, true};
    commandsConfig = {true, true, {{"ll", "ls -la"}, {"la", "ls -a"}, {"l", "ls -l"}}};
    loggingConfig = {false, "INFO", true, true};
    filesystemConfig = {
        {"/home", "/home/user", "/bin", "/etc", "/var", "/tmp", "/root", "/usr", "/usr/bin", "/usr/lib"},
        {
            {"/etc/passwd", "root:x:0:0:root:/root:/bin/sh\nuser:x:1000:1000:user:/home/user:/bin/sh\n"},
            {"/etc/hostname", "virtualshell\n"},
            {"/etc/motd", "Welcome to virtualSHELL!\nType 'help' for available commands.\n"}
        }
    };
}

bool ConfigManager::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ConfigManager: Could not open config file: " << filepath << "\n";
        return false;
    }

    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty() || line[0] == '#' || line[0] == '/') {
            continue;
        }

        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        size_t separatorPos = line.find('=');
        if (separatorPos == std::string::npos) {
            separatorPos = line.find(':');
        }
        if (separatorPos != std::string::npos) {
            std::string key = trim(line.substr(0, separatorPos));
            std::string value = trim(line.substr(separatorPos + 1));

            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }

            rawConfig[currentSection][key] = value;

            if (currentSection == "user") {
                if (key == "username") userConfig.username = value;
                else if (key == "hostname") userConfig.hostname = value;
                else if (key == "home") userConfig.home = value;
                else if (key == "shell") userConfig.shell = value;
            }
            else if (currentSection == "shell") {
                if (key == "name") shellConfig.name = value;
                else if (key == "version") shellConfig.version = value;
            }
        }
    }

    file.close();
    return true;
}

bool ConfigManager::saveToFile(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    file << "{\n";
    file << "    \"shell\": {\n";
    file << "        \"name\": \"" << shellConfig.name << "\",\n";
    file << "        \"version\": \"" << shellConfig.version << "\"\n";
    file << "    },\n";
    file << "    \"user\": {\n";
    file << "        \"username\": \"" << userConfig.username << "\",\n";
    file << "        \"hostname\": \"" << userConfig.hostname << "\",\n";
    file << "        \"home\": \"" << userConfig.home << "\",\n";
    file << "        \"shell\": \"" << userConfig.shell << "\"\n";
    file << "    }\n";
    file << "}\n";

    file.close();
    return true;
}

ConfigManager::ShellConfig ConfigManager::getShellConfig() const {
    return shellConfig;
}

ConfigManager::UserConfig ConfigManager::getUserConfig() const {
    return userConfig;
}

ConfigManager::PromptConfig ConfigManager::getPromptConfig() const {
    return promptConfig;
}

ConfigManager::SandboxConfig ConfigManager::getSandboxConfig() const {
    return sandboxConfig;
}

ConfigManager::CommandsConfig ConfigManager::getCommandsConfig() const {
    return commandsConfig;
}

ConfigManager::LoggingConfig ConfigManager::getLoggingConfig() const {
    return loggingConfig;
}

ConfigManager::FilesystemConfig ConfigManager::getFilesystemConfig() const {
    return filesystemConfig;
}

void ConfigManager::setUserConfig(const UserConfig& config) {
    userConfig = config;
}

void ConfigManager::setPromptConfig(const PromptConfig& config) {
    promptConfig = config;
}

std::string ConfigManager::getValue(const std::string& section, const std::string& key) const {
    auto sectionIt = rawConfig.find(section);
    if (sectionIt != rawConfig.end()) {
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            return keyIt->second;
        }
    }
    return "";
}

void ConfigManager::setValue(const std::string& section, const std::string& key, const std::string& value) {
    rawConfig[section][key] = value;
}

std::string ConfigManager::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}
