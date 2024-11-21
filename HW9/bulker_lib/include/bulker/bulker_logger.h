#pragma once

#include "logger.h"
#include "dummy_stream.h"

#include <ostream>
#include <fstream>
#include <functional>
#include <sstream>

class BulkerLogger : public Logger
{
public:
    BulkerLogger(std::ostream& consoleStream, const std::reference_wrapper<std::ostream>& fileStream)
        : stream_ { consoleStream }, fileStream_ { fileStream }
    {
    }

    ~BulkerLogger()
    {
        underlyingFileStream_.flush();
        underlyingFileStream_.close();
    }

    void log(const std::string& message) override
    {
        buffer_ << message;
        /*stream_ << message;
        fileStream_.get() << message;
        fileStream_.get().flush();*/
    }

    virtual void flush() {
        stream_ << buffer_.str();
        fileStream_.get() << buffer_.str();
        fileStream_.get().flush();
        buffer_.str(std::string());
    }

    virtual void enableLogToFile() { logToFile_ = true; }

    virtual void setLogFile(const std::string& filePath)
    {
        if (logToFile_) {
            underlyingFileStream_.close();
            underlyingFileStream_.open(filePath, std::ios::out);
            fileStream_ = std::ref(underlyingFileStream_);
        }
    }

protected:
    std::ostream&                        stream_;
    std::reference_wrapper<std::ostream> fileStream_;
    std::ofstream                        underlyingFileStream_;
    bool                                 logToFile_ = false;
    std::stringstream buffer_;
};