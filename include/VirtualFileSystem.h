#ifndef VIRTUALFILESYSTEM_H
#define VIRTUALFILESYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class VirtualFileSystem {
public:
    struct FileNode {
        std::string name;
        bool isDirectory;
        std::string content;
        std::vector<FileNode*> children;
        FileNode* parent;
        std::string permissions;
        size_t size;
        std::string modifiedTime;

        FileNode(const std::string& n, bool isDir, FileNode* p = nullptr);
        ~FileNode();
    };

    VirtualFileSystem();
    ~VirtualFileSystem();

    bool createDirectory(const std::string& path);
    bool createFile(const std::string& path, const std::string& content = "");
    bool writeFile(const std::string& path, const std::string& content);
    bool readFile(const std::string& path, std::string& content);
    bool deletePath(const std::string& path);
    bool exists(const std::string& path) const;
    bool isDirectory(const std::string& path) const;
    std::string getCurrentPath() const;
    bool changeDirectory(const std::string& path);
    std::vector<std::string> listDirectory(const std::string& path = "");
    std::string getAbsolutePath(const std::string& path) const;
    bool copyFile(const std::string& src, const std::string& dst);
    bool moveFile(const std::string& src, const std::string& dst);
    size_t getFileSize(const std::string& path) const;
    bool isEmpty(const std::string& path) const;

    bool saveToFile(const std::string& filepath);
    bool loadFromFile(const std::string& filepath);

    void printTree(const std::string& path = "", int depth = 0) const;

private:
    FileNode* root;
    FileNode* currentDir;

    FileNode* resolvePath(const std::string& path) const;
    std::string getNodePath(FileNode* node) const;
    void serializeNode(FileNode* node, std::ofstream& out, int depth);
    FileNode* deserializeNode(std::ifstream& in, FileNode* parent);
    static std::string getCurrentTimestamp();
};

#endif
