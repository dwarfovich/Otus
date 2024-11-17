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
    std::uintmax_t        size_ = 0;
    std::string           currentBlock_;
};

class StringSource : public SourceEntity
{
public:
    StringSource(const std::filesystem::path& path, const std::string& data) : path_ { path }, data_ { data } {}

    std::string_view getNextBlock(std::size_t blockSize)
    {
        const auto begin = data_.cbegin() + currentPos_;
        const auto size =
            std::min(blockSize, static_cast<std::size_t>(std::distance(begin, data_.cend())));
        const auto end =
            begin + size;
        std::string_view result = { begin, end };
        currentPos_ += size;
        return result;
    }
    void                         setPos(std::size_t pos) { currentPos_ = pos; }
    std::uintmax_t               size() const noexcept { return data_.size(); };
    const std::filesystem::path& path() const noexcept { return path_; }

private:
    std::filesystem::path path_;
    std::string           data_;
    std::size_t           currentPos_ = 0;
};