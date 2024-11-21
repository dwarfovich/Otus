#pragma once

#include "bulker_logger.h"
#include "dummy_stream.h"
#include "thread_safe_cout.h"

#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_future.hpp>

class ThreadedBulkerLogger : public BulkerLogger
{
public:
    ThreadedBulkerLogger()
        : BulkerLogger { std::cout, std::ref(dummyStream_) }
        , fileLogger1_ { dummyStream_, std::ref(fileStream1_) }
        , fileLogger2_ { dummyStream_, std::ref(fileStream2_) }
        , currentLogger_ { &fileLogger1_ }
        , currentMutex_ { &mutex1_ }
        , threadPool_ { 3 }
    {
        fileLogger1_.enableLogToFile();
        fileLogger2_.enableLogToFile();
    }

    ~ThreadedBulkerLogger(){
        flush();
    }
    void log(const std::string& message) override
    {
        std::lock_guard lock { *currentMutex_ };
        BulkerLogger::log(message);
        //currentLogger_->log(message);
    }
    void flush() override
    {
        std::scoped_lock lock { mutex1_, mutex2_ };
        boost::asio::post(threadPool_, [this]() {
        //std::cout << std::this_thread::get_id() << " Flushing message: " << BulkerLogger::buffer_.str() << '\n';
            BulkerLogger::flush();
        });
        boost::asio::post(threadPool_, [this]() {
            currentLogger_->flush();
        });
        //threadPool_.join();
        //switchFileLogger();
    }
    void enableLogToFile() override {}

    void setLogFile(const std::string& filePath) override
    {
        std::scoped_lock lock { mutex1_, mutex2_ };
        // std::cout << "Switching logger" << '\n';
        // std::cout << ss_.str();
        // switchFileLogger();
        currentLogger_->setLogFile(filePath);
        /*if (logToFile_) {
            underlyingFileStream_.close();
            underlyingFileStream_.open(filePath, std::ios::out);
            fileStream_ = std::ref(underlyingFileStream_);
        }*/
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
    // std::string addUnicIdToLogFileName(const std::string& basePath) { return basePath; }

private: // data
    DummyStream              dummyStream_;
    BulkerLogger             fileLogger1_;
    BulkerLogger             fileLogger2_;
    BulkerLogger*            currentLogger_ = nullptr;
    std::mutex               mutex1_;
    std::mutex               mutex2_;
    std::mutex*              currentMutex_;
    std::mutex               switchingMutex_;
    boost::asio::thread_pool threadPool_;
    std::ofstream            fileStream1_;
    std::ofstream            fileStream2_;

    //std::stringstream ss_;
    std::mutex        coutMutex_;
};