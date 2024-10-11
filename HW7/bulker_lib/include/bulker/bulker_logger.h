#pragma once

#include "logger.h"
#include "dummy_stream.h"

#include <ostream>
#include <fstream>

class BulkerLogger : public Logger
{
public:
    BulkerLogger(std::ostream& consoleStream, const std::reference_wrapper<std::ostream>& fileStream)
        : stream_ { consoleStream }, fileStream_ { fileStream }
    {
    }

    void log(const std::string& message) override {
        stream_ << message;
        fileStream_.get() << message;
    }

    virtual void setLogFile(const std::string& filePath){
        underlyingFileStream_.open(filePath);
        fileStream_ = std::ref(underlyingFileStream_);
    }

private:
    std::ostream& stream_;
    std::reference_wrapper<std::ostream> fileStream_;
    std::ofstream underlyingFileStream_;
};