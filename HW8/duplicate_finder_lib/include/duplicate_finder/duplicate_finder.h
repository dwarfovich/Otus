#pragma once

#include "path_vector.h"
#include "duplicates.h"
#include "finder_task.h"
#include "file_finder.h"
#include "signal.h"

#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_future.hpp>

#include <iostream>

class DuplicateFinder
{
private: // types
    using ThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;

public: // methods
    void findDuplicates(const FinderTask& task)
    {
        if (threadPool_) {
            threadPool_->stop();
        }
        currentTask_ = task;
        auto newSize = task.maxThreadCount == 0 ? boost::thread::hardware_concurrency() : task.maxThreadCount;
        if (threadPoolSize_ == 0 || threadPoolSize_ != newSize) {
            threadPool_     = std::make_shared<boost::asio::thread_pool>(newSize);
            threadPoolSize_ = newSize;
        }
        startWork();
    }

public: // signals
    Signal<void()> taskFinished;

private: // methods
    void startWork()
    {
        auto filesFuture = boost::asio::post(*threadPool_, boost::asio::use_future([this]() {
            auto files = fileFinder_.findFiles(currentTask_);
            findDuplicates(std::move(files));
        }));
    }

    void findDuplicates(const FileFinder::FilePropertiesUSet& files) {
        std::cout << "threadPoolSize_: " << threadPoolSize_ << '\n';
        std::cout << "files: " << files.size() << '\n';
        std::size_t filesPerThread = 0;
        if (files.size() / minimumFilesPerThread_ <= threadPoolSize_){
            filesPerThread = files.size() / minimumFilesPerThread_;
        } else{
            filesPerThread = files.size() / minimumFilesPerThread_;
        }
        
        std::cout << "filesPerThread: " << filesPerThread  << '\n';
    }

private: // data
    static inline constexpr std::size_t minimumFilesPerThread_ = 20;

    FinderTask    currentTask_;
    Duplicates    duplicates_;
    FileFinder    fileFinder_;
    unsigned      threadPoolSize_ = 0;
    ThreadPoolPtr threadPool_;

};