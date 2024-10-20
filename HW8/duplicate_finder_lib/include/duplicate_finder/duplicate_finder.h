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

class DuplicateFinder
{
private: // types
    using ThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;

public: // methods
    void findDuplicates(const FinderTask& task)
    {
        threadPool_->stop();
        currentTask_ = task;
        auto newSize = task.threadCount == 0 ? boost::thread::hardware_concurrency() : task.threadCount;
        if (threadPoolSize_ == 0 || threadPoolSize_ != newSize) {
            threadPool_     = std::make_shared<boost::asio::thread_pool>(newSize);
            threadPoolSize_ = newSize;
        }
        startWork();
    }

public: // signals
    Signal<void()> taskFinished;

private: // methods
    void startWork() {
        auto filesFuture = boost::asio::post(*threadPool_, boost::asio::use_future([this](){
            return fileFinder_.findFiles(currentTask_);
            }));
        findDuplicates(std::move(filesFuture.get()), currentTask_);
    }

    void findDuplicates(FileFinder::FilePropertiesUSet files, FinderTask    task) {
    }

private: // data
    FinderTask    currentTask_;
    Duplicates    duplicates_;
    FileFinder  fileFinder_;
    unsigned      threadPoolSize_ = 0;
    ThreadPoolPtr threadPool_;

    static inline constexpr std::size_t minimumFilesPerThread_ = 20;
};