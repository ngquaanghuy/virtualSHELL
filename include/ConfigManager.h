#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class ConfigManager {
public:
    struct ShellConfig {
        std::string name;
        std::string version;
    };

    struct UserConfig {
        std::string username;
        std::string hostname;
        std::string home;
        std::string shell;
    };

    struct PromptConfig {
        std::string format;
        std::string color;
        bool show_hostname;
        bool show_path;
    };

    struct SandboxConfig {
        bool auto_save;
        bool save_on_exit;
        std::string data_file;
        int max_history;
        bool create_default_dirs;
    };

    struct CommandsConfig {
        bool load_on_demand;
        bool show_load_message;
        std::map<std::string, std::string> default_aliases;
    };

    struct LoggingConfig {
        bool verbose;
        std::string log_level;
        bool show_timestamps;
        bool colored_output;
    };

    struct FilesystemConfig {
        std::vector<std::string> default_dirs;
        std::map<std::string, std::string> default_files;
    };

    ConfigManager();
    ~ConfigManager();

    bool loadFromFile(const std::string& filepath);
    bool saveToFile(const std::string& filepath);

    ShellConfig getShellConfig() const;
    UserConfig getUserConfig() const;
    PromptConfig getPromptConfig() const;
    SandboxConfig getSandboxConfig() const;
    CommandsConfig getCommandsConfig() const;
    LoggingConfig getLoggingConfig() const;
    FilesystemConfig getFilesystemConfig() const;

    void setUserConfig(const UserConfig& config);
    void setPromptConfig(const PromptConfig& config);

    std::string getValue(const std::string& section, const std::string& key) const;
    void setValue(const std::string& section, const std::string& key, const std::string& value);

private:
    ShellConfig shellConfig;
    UserConfig userConfig;
    PromptConfig promptConfig;
    SandboxConfig sandboxConfig;
    CommandsConfig commandsConfig;
    LoggingConfig loggingConfig;
    FilesystemConfig filesystemConfig;

    std::map<std::string, std::map<std::string, std::string>> rawConfig;

    void parseLine(const std::string& line);
    void applyDefaultConfig();
    std::string trim(const std::string& str) const;
};

#endif
