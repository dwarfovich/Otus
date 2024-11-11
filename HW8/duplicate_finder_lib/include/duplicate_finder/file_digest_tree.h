#pragma once

#include <map>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace details {
using FilesConstIterator = std::vector<FileProperties>::const_iterator;
};

struct FilePropertiesIterHasher
{
    using is_transparent = void;

    std::size_t operator()(const details::FilesConstIterator& iter) const { return hashFromVector(iter->hash()); }
    std::size_t operator()(const std::vector<std::string>& inputHash) const { return hashFromVector(inputHash); }

private:
    std::size_t hashFromVector(const std::vector<std::string>& inputHash) const
    {
        std::size_t hash = 0;
        for (const auto& hashPart : inputHash) {
            boost::hash_combine(hash, hashPart);
        }

        return hash;
    }
};

struct FilePropertiesIterComparator
{
    bool operator()(const details::FilesConstIterator& lhs, const details::FilesConstIterator& rhs) const
    {
        return lhs->hash() == rhs->hash();
    }
};

class FilesMap
{
public:
    void setFiles(std::vector<FileProperties>&& files)
    {
        files_ = std::move(files);
        populateFileMap();
    }

private: // methods
    void populateFileMap()
    {
        fileMap_.clear();
        for (auto iter = files_.cbegin(); iter != files_.cend(); ++iter) {
            fileMap_[iter->size()].insert(iter);
        }
    }

public: // data
    std::vector<FileProperties> files_;

    using FileSet =
        std::unordered_multiset<details::FilesConstIterator, FilePropertiesIterHasher, FilePropertiesIterComparator>;
    std::unordered_map<std::uintmax_t, FileSet> fileMap_;
};