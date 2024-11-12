#pragma once

#include "path_vector.h"
#include "duplicates.h"
#include "finder_task.h"
#include "file_finder.h"
#include "signal.h"

#include <gtest/gtest_prod.h>

#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_future.hpp>

class DuplicateFinder
{
    FRIEND_TEST(DuplicateFinder, CalculateComparisons_ZeroResult);
    FRIEND_TEST(DuplicateFinder, CalculateComparisons);

private: // types
    using ThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;

public: // methods
    void findDuplicates(const FinderTask& task)
    {
        if (threadPool_) {
            threadPool_->stop();
        }
        currentTask_       = task;
        const auto newSize = task.maxThreadCount == 0 ? boost::thread::hardware_concurrency() : task.maxThreadCount;
        resizeThreadPool(newSize);
        std::cout << "Start job" << '\n';
        startWork();
    }

public: // signals
    Signal<void()> taskFinished;

private: // methods
    void startWork()
    {
        auto mainThread = boost::thread { [this]() {
            auto files      = fileFinder_.findFiles(currentTask_);
            filterFiles(files);
            distributeJobs(std::move(files));
            threadPool_->join();
            taskFinished();
        } };
        mainThread.detach();
    }

    void filterFiles(FileFinder::FileMap& files)
    {
        std::erase_if(files, [](const auto& entry) {
            return entry.second.size() < 2;
        });
    }

    void distributeJobs(const FileFinder::FileMap& files)
    {
        for (const auto& [size, jobFiles] : files) {
            boost::asio::post(*threadPool_, [this, &jobFiles]() {
                performThreadJob(jobFiles);
            });
        }
    }

    void performThreadJob(const PathVector& files)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Hello\n";
    }

    void resizeThreadPool(unsigned size)
    {
        if (threadPoolSize_ == 0 || threadPoolSize_ != size) {
            threadPool_     = std::make_shared<boost::asio::thread_pool>(size);
            threadPoolSize_ = size;
        }
    }

private: // data
    boost::thread mainThread_;
    FinderTask    currentTask_;
    Duplicates    duplicates_;
    FileFinder    fileFinder_;
    unsigned      threadPoolSize_ = 0;
    ThreadPoolPtr threadPool_;
};