#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>

class SourceEntity
{
public:
    virtual ~SourceEntity() {}

    virtual std::string_view             getNextBlock(std::size_t blockSize) = 0;
    virtual void                         setPos(std::size_t pos)             = 0;
    virtual std::uintmax_t               size() const noexcept               = 0;
    virtual const std::filesystem::path& path() const noexcept               = 0;
};

class FileSource : public SourceEntity
{
public:
    FileSource(const std::filesystem::path path)
        : stream_ { std::filesystem::is_regular_file(path)
                        ? path
                        : throw std::invalid_argument { "Path doesn't lead to regular file" } }
        , path_ { path }
        , size_ { std::filesystem::file_size(path) }
    {
        if (!stream_.is_open()) {
            throw std::runtime_error("Failed to open file at " + path.string());
        }
    }

    std::string_view getNextBlock(std::size_t blockSize)
    {
        currentBlock_.resize(blockSize);
        stream_.read(currentBlock_.data(), blockSize);
        return currentBlock_;
    }
    void                         setPos(std::size_t pos) { stream_.seekg(pos); }
    std::uintmax_t               size() const noexcept { return size_; };
    const std::filesystem::path& path() const noexcept { return path_; }

private:
    std::ifstream         stream_;
    std::filesystem::path path_;
    std::uintmax_t        size_;
    std::string           currentBlock_;
};