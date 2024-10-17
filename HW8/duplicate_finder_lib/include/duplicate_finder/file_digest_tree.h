#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include <filesystem>

class FileDigestNode
{
    friend class DigestTree;

public:
private:
    using NodeUptr = std::unique_ptr<FileDigestNode>;
    std::map<char, NodeUptr> map;
};

class FileDigestTree
{
public:
    using NodeUptr = std::unique_ptr<FileDigestNode>;

private:
    NodeUptr                                            root_;
    std::unordered_map<NodeUptr, std::filesystem::path> files_;
};