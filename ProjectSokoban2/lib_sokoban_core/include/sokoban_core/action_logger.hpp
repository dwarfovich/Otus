#pragma once

#include "key_definitions.hpp"

#include <filesystem>
#include <vector>
#include <fstream>

namespace sokoban {

class ActionLogger
{
public:
    ActionLogger(std::filesystem::path path) : currentLogFilePath_{path}{}
    ~ActionLogger() { flushBuffer(); }

    void setLogFilePath(const std::filesystem::path& path) { newLogFilePath_ = path; }

    void setMaxBufferLength(std::size_t length)
    {
        maxBufferLength_ = length;
        flushBufferIfNeeded();
    }

    void log(Key key)
    {
        buffer_.push_back(key);
        flushBufferIfNeeded();
    }

private: // methods
    void flushBuffer()
    {
        if (logStream_.is_open() && currentLogFilePath_ != newLogFilePath_) {
            logStream_.close();
            currentLogFilePath_ = newLogFilePath_;
        }

        std::filesystem::create_directories(currentLogFilePath_.parent_path());
        logStream_.open(currentLogFilePath_, std::ios::binary);
        if (logStream_.is_open()) {
            for (auto key : buffer_) {
                logStream_ << static_cast<std::underlying_type<Key>::type>(key) << '\n';
            }
            buffer_.clear();
        }
    }

    void flushBufferIfNeeded()
    {
        if (buffer_.size() >= maxBufferLength_) {
            flushBuffer();
        }
    }

private: // data
    std::size_t           maxBufferLength_ = 100;
    std::vector<Key>      buffer_;
    std::filesystem::path currentLogFilePath_;
    std::filesystem::path newLogFilePath_;
    std::ofstream         logStream_;
};

} // namespace sokoban