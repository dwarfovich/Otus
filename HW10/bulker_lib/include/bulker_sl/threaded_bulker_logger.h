#pragma once

#include "bulker/bulker_logger.h"
#include "bulker/dummy_stream.h"
#include "thread_safe_cout.h"

#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_future.hpp>

#include <iostream>

class ThreadedBulkerLogger : public BulkerLogger
{
public:
    ThreadedBulkerLogger(std::ostream& stream)
        : BulkerLogger { stream, std::ref(dummyStream_) }
        , fileLogger1_ { dummyStream_, std::ref(fileStream1_) }
        , fileLogger2_ { dummyStream_, std::ref(fileStream2_) }
        , currentLogger_ { &fileLogger1_ }
        , currentMutex_ { &mutex1_ }
        , threadPool_ { 3 }
    {
        fileLogger1_.enableLogToFile();
        fileLogger2_.enableLogToFile();
    }

    ~ThreadedBulkerLogger() override
    {
        flush();
        threadPool_.wait();
        threadPool_.join();
    }

    void log(const std::string& message) override
    {
        BulkerLogger::log(message);
        currentLogger_->log(message);
    }

    void flush() override
    {
        boost::asio::post(threadPool_, [this]() {
            std::lock_guard lock { *currentMutex_ };
            BulkerLogger::flush();
            currentLogger_->flush();
        });
        switchFileLogger();
    }
    void enableLogToFile() override {}

    void setLogFile(const std::string& filePath) override
    {
        std::scoped_lock lock { mutex1_, mutex2_ };
        switchFileLogger();
        currentLogger_->setLogFile(filePath);
    }

private: // methods
    void switchFileLogger()
    {
        std::lock_guard lock { switchingMutex_ };
        if (currentLogger_ == &fileLogger1_) {
            currentLogger_ = &fileLogger2_;
            currentMutex_  = &mutex2_;
        } else {
            currentLogger_ = &fileLogger1_;
            currentMutex_  = &mutex1_;
        }
    }

public: // data
    boost::asio::thread_pool threadPool_;
    DummyStream              dummyStream_;
    BulkerLogger             fileLogger1_;
    BulkerLogger             fileLogger2_;
    BulkerLogger*            currentLogger_ = nullptr;
    std::mutex               mutex1_;
    std::mutex               mutex2_;
    std::mutex*              currentMutex_;
    std::mutex               switchingMutex_;
    std::ofstream            fileStream1_;
    std::ofstream            fileStream2_;
    //std::ostream& stream_;
    // std::mutex coutMutex_;
};