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

// #include <thread>
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
        auto mainThread = boost::thread { [this]() {
            auto files      = fileFinder_.findFiles(currentTask_);
            findDuplicates(std::move(files));
            threadPool_->join();
            taskFinished();
        } };
        mainThread.detach();
    }

    void findDuplicates(const FileFinder::FilePropertiesVector& files)
    {
        const auto  filesPerThread = calclulateFilesPerThread(files.size());
        std::size_t distanceToEnd  = files.size();
        for (auto iter = files.cbegin(), nextIter = iter; iter != files.cend(); iter = nextIter) {
            distanceToEnd = static_cast<std::size_t>(std::distance(files.cend(), iter));
            nextIter      = iter + std::min(filesPerThread, distanceToEnd);
            boost::asio::post(*threadPool_, [this, iter, nextIter]() {

            });
        }
    }

    std::size_t calclulateFilesPerThread(std::size_t filesCount) const
    {
        const auto filesPerThread = filesCount / threadPoolSize_;
        if (filesPerThread >= minimumFilesPerThread_) {
            return filesPerThread;
        } else {
            const auto threadsNeeded = filesCount / minimumFilesPerThread_;

            return filesCount / threadsNeeded;
        }
    }

private: // data
    static inline constexpr std::size_t minimumFilesPerThread_ = 20;

    boost::thread mainThread_;
    FinderTask    currentTask_;
    Duplicates    duplicates_;
    FileFinder    fileFinder_;
    unsigned      threadPoolSize_ = 0;
    ThreadPoolPtr threadPool_;
};