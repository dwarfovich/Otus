#pragma once

#include "key_definitions.hpp"

#include <filesystem>
#include <vector>
#include <fstream>

namespace sokoban {

class ActionLogger
{
public:
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

        logStream_.open(currentLogFilePath_);
        if (logStream_.is_open()) {
            for (auto key : buffer_) {
                logStream_ << key;
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
    std::ifstream         logStream_;
};

} // namespace sokoban