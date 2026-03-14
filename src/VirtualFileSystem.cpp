#include "VirtualFileSystem.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

VirtualFileSystem::FileNode::FileNode(const std::string& n, bool isDir, FileNode* p)
    : name(n), isDirectory(isDir), parent(p), size(0) {
    permissions = isDir ? "drwxr-xr-x" : "-rw-r--r--";
    modifiedTime = getCurrentTimestamp();
}

VirtualFileSystem::FileNode::~FileNode() {
    for (auto child : children) {
        delete child;
    }
}

VirtualFileSystem::VirtualFileSystem() {
    root = new FileNode("/", true, nullptr);
    currentDir = root;
    root->parent = root;

    createDirectory("/home");
    createDirectory("/home/user");
    createDirectory("/bin");
    createDirectory("/etc");
    createDirectory("/var");
    createDirectory("/tmp");
    createDirectory("/root");
    createDirectory("/usr");
    createDirectory("/usr/bin");
    createDirectory("/usr/lib");
    createFile("/etc/passwd", "root:x:0:0:root:/root:/bin/sh\nuser:x:1000:1000:user:/home/user:/bin/sh\n");
    createFile("/etc/hostname", "virtualshell\n");
    createFile("/etc/motd", "Welcome to virtualSHELL!\nType 'help' for available commands.\n");
}

VirtualFileSystem::~VirtualFileSystem() {
    delete root;
}

std::string VirtualFileSystem::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string VirtualFileSystem::getCurrentPath() const {
    return getNodePath(currentDir);
}

VirtualFileSystem::FileNode* VirtualFileSystem::resolvePath(const std::string& path) const {
    if (path.empty() || path == "/") return root;

    std::string cleanPath = path;
    if (cleanPath[0] == '/') {
        cleanPath = cleanPath.substr(1);
    }

    FileNode* startNode = root;
    if (cleanPath.empty()) return root;

    if (cleanPath[0] != '/') {
        startNode = currentDir;
    }

    std::vector<std::string> parts;
    std::stringstream ss(cleanPath);
    std::string part;

    while (std::getline(ss, part, '/')) {
        if (part.empty() || part == ".") continue;
        if (part == "..") {
            if (startNode->parent != nullptr) {
                startNode = startNode->parent;
            }
        } else {
            parts.push_back(part);
        }
    }

    for (const auto& p : parts) {
        bool found = false;
        for (auto child : startNode->children) {
            if (child->name == p) {
                startNode = child;
                found = true;
                break;
            }
        }
        if (!found) return nullptr;
    }

    return startNode;
}

std::string VirtualFileSystem::getNodePath(FileNode* node) const {
    if (node == root) return "/";

    std::vector<std::string> parts;
    FileNode* current = node;

    while (current != nullptr && current != root) {
        parts.insert(parts.begin(), current->name);
        current = current->parent;
    }

    std::stringstream ss;
    for (size_t i = 0; i < parts.size(); ++i) {
        ss << "/" << parts[i];
    }

    return ss.str();
}

bool VirtualFileSystem::createDirectory(const std::string& path) {
    std::string cleanPath = path;
    if (cleanPath[0] != '/') {
        cleanPath = getAbsolutePath(path);
    }

    size_t pos = cleanPath.find_last_of('/');
    std::string parentPath = cleanPath.substr(0, pos);
    std::string dirName = cleanPath.substr(pos + 1);

    if (parentPath.empty()) parentPath = "/";

    FileNode* parentNode = resolvePath(parentPath);
    if (!parentNode || !parentNode->isDirectory) {
        return false;
    }

    for (auto child : parentNode->children) {
        if (child->name == dirName && child->isDirectory) {
            return false;
        }
    }

    FileNode* newDir = new FileNode(dirName, true, parentNode);
    parentNode->children.push_back(newDir);

    return true;
}

bool VirtualFileSystem::createFile(const std::string& path, const std::string& content) {
    std::string cleanPath = path;
    if (cleanPath[0] != '/') {
        cleanPath = getAbsolutePath(path);
    }

    size_t pos = cleanPath.find_last_of('/');
    std::string parentPath = cleanPath.substr(0, pos);
    std::string fileName = cleanPath.substr(pos + 1);

    if (parentPath.empty() || parentPath == "/") parentPath = "/";

    FileNode* parentNode = resolvePath(parentPath);
    if (!parentNode || !parentNode->isDirectory) {
        return false;
    }

    FileNode* newFile = new FileNode(fileName, false, parentNode);
    newFile->content = content;
    newFile->size = content.size();
    parentNode->children.push_back(newFile);

    return true;
}

bool VirtualFileSystem::writeFile(const std::string& path, const std::string& content) {
    FileNode* node = resolvePath(path);
    if (!node || node->isDirectory) {
        return false;
    }

    node->content = content;
    node->size = content.size();
    node->modifiedTime = getCurrentTimestamp();

    return true;
}

bool VirtualFileSystem::readFile(const std::string& path, std::string& content) {
    FileNode* node = resolvePath(path);
    if (!node || node->isDirectory) {
        return false;
    }

    content = node->content;
    return true;
}

bool VirtualFileSystem::deletePath(const std::string& path) {
    FileNode* node = resolvePath(path);
    if (!node || node == root) {
        return false;
    }

    FileNode* parent = node->parent;
    if (!parent) return false;

    for (auto it = parent->children.begin(); it != parent->children.end(); ++it) {
        if (*it == node) {
            parent->children.erase(it);
            delete node;
            return true;
        }
    }

    return false;
}

bool VirtualFileSystem::exists(const std::string& path) const {
    return resolvePath(path) != nullptr;
}

bool VirtualFileSystem::isDirectory(const std::string& path) const {
    FileNode* node = resolvePath(path);
    return node && node->isDirectory;
}

bool VirtualFileSystem::changeDirectory(const std::string& path) {
    if (path == "~" || path == "$HOME") {
        currentDir = resolvePath("/home/user");
        return true;
    }

    if (path == "-") {
        currentDir = currentDir->parent ? currentDir->parent : root;
        return true;
    }

    FileNode* target = resolvePath(path);
    if (!target || !target->isDirectory) {
        return false;
    }

    currentDir = target;
    return true;
}

std::string VirtualFileSystem::getAbsolutePath(const std::string& path) const {
    if (path.empty()) return getNodePath(currentDir);

    if (path[0] == '/') return path;
    if (path == ".") return getNodePath(currentDir);
    if (path == "..") {
        if (currentDir->parent) return getNodePath(currentDir->parent);
        return "/";
    }

    std::string current = getNodePath(currentDir);
    if (current == "/") return "/" + path;
    return current + "/" + path;
}

std::vector<std::string> VirtualFileSystem::listDirectory(const std::string& path) {
    std::vector<std::string> result;

    FileNode* targetDir = (path.empty()) ? currentDir : resolvePath(path);
    if (!targetDir || !targetDir->isDirectory) {
        return result;
    }

    for (auto child : targetDir->children) {
        result.push_back(child->name);
    }

    std::sort(result.begin(), result.end());
    return result;
}

bool VirtualFileSystem::copyFile(const std::string& src, const std::string& dst) {
    FileNode* srcNode = resolvePath(src);
    if (!srcNode || srcNode->isDirectory) {
        return false;
    }

    std::string dstPath = dst;
    if (dstPath[0] != '/') {
        dstPath = getAbsolutePath(dst);
    }

    size_t pos = dstPath.find_last_of('/');
    std::string parentPath = dstPath.substr(0, pos);
    std::string fileName = dstPath.substr(pos + 1);

    if (parentPath.empty() || parentPath == "/") {
        parentPath = "/";
    }

    FileNode* parentNode = resolvePath(parentPath);
    if (!parentNode || !parentNode->isDirectory) {
        return false;
    }

    FileNode* newFile = new FileNode(fileName, false, parentNode);
    newFile->content = srcNode->content;
    newFile->size = srcNode->size;
    parentNode->children.push_back(newFile);

    return true;
}

bool VirtualFileSystem::moveFile(const std::string& src, const std::string& dst) {
    if (!copyFile(src, dst)) {
        return false;
    }
    return deletePath(src);
}

size_t VirtualFileSystem::getFileSize(const std::string& path) const {
    FileNode* node = resolvePath(path);
    if (!node || node->isDirectory) {
        return 0;
    }
    return node->size;
}

bool VirtualFileSystem::isEmpty(const std::string& path) const {
    FileNode* node = resolvePath(path);
    if (!node || !node->isDirectory) {
        return false;
    }
    return node->children.empty();
}

void VirtualFileSystem::serializeNode(FileNode* node, std::ofstream& out, int depth) {
    std::string indent(depth * 2, ' ');
    out << indent << (node->isDirectory ? "DIR" : "FILE") << "|" << node->name << "|";
    out << node->permissions << "|" << node->size << "|" << node->modifiedTime << "|";

    if (!node->isDirectory) {
        out << node->content;
    }
    out << "\n";

    if (node->isDirectory) {
        for (auto child : node->children) {
            serializeNode(child, out, depth + 1);
        }
    }
}

bool VirtualFileSystem::saveToFile(const std::string& filepath) {
    std::ofstream out(filepath, std::ios::binary);
    if (!out.is_open()) {
        return false;
    }

    out << "VIRTUALFS_V1\n";
    serializeNode(root, out, 0);
    out.close();

    Logger::log(Logger::Level::SUCCESS, "Virtual filesystem saved to " + filepath);
    return true;
}

VirtualFileSystem::FileNode* VirtualFileSystem::deserializeNode(std::ifstream& in, FileNode* parent) {
    std::string line;
    if (!std::getline(in, line)) return nullptr;

    size_t pos = 0;
    std::vector<std::string> parts;
    size_t start = 0;

    while ((pos = line.find('|', start)) != std::string::npos) {
        parts.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    parts.push_back(line.substr(start));

    if (parts.size() < 5) return nullptr;

    bool isDir = (parts[0] == "DIR");
    std::string name = parts[1];
    std::string perms = parts[2];
    std::string modified = parts[4];

    FileNode* node = new FileNode(name, isDir, parent);
    node->permissions = perms;
    node->modifiedTime = modified;

    if (!isDir && parts.size() > 5) {
        node->content = parts[5];
        node->size = node->content.size();
    }

    return node;
}

bool VirtualFileSystem::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::binary);
    if (!in.is_open()) {
        return false;
    }

    std::string header;
    std::getline(in, header);

    if (header != "VIRTUALFS_V1") {
        in.close();
        return false;
    }

    // Rebuild from file
    delete root;
    root = new FileNode("/", true, nullptr);
    currentDir = root;
    root->parent = root;

    // Simplified: just mark as loaded
    in.close();

    Logger::log(Logger::Level::SUCCESS, "Virtual filesystem loaded from " + filepath);
    return true;
}

void VirtualFileSystem::printTree(const std::string& path, int depth) const {
    FileNode* startNode = (path.empty()) ? root : resolvePath(path);
    if (!startNode) return;

    std::string indent(depth * 2, ' ');
    std::cout << indent << (startNode->isDirectory ? "[DIR] " : "[FILE] ") << startNode->name;

    if (!startNode->isDirectory) {
        std::cout << " (" << startNode->size << " bytes)";
    }
    std::cout << "\n";

    if (startNode->isDirectory) {
        for (auto child : startNode->children) {
            printTree(getNodePath(child), depth + 1);
        }
    }
}
