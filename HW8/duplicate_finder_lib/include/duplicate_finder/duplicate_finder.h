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

// #include <thread>
#include <iostream>

class DuplicateFinder
{
    FRIEND_TEST(DuplicateFinder, JobGeneration_WithZeroJobs);
    FRIEND_TEST(DuplicateFinder, CalculateComparisons_ZeroResult);
    FRIEND_TEST(DuplicateFinder, CalculateComparisons);

private: // types
    using ThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;

    struct ThreadJob
    {
        using ConstIter = FileFinder::FilePropertiesVector::const_iterator;

        ThreadJob(const ConstIter& aFirstFile,
                  const ConstIter& aSecondFile,
                  const ConstIter& aEnd,
                  std::size_t      aComparisons)
            : firstFile { aFirstFile }
        {
        }

        ConstIter   firstFile;
        ConstIter   secondFile;
        ConstIter   end;
        std::size_t comparisons = 0;
    };

public: // methods
    void findDuplicates(const FinderTask& task)
    {
        if (threadPool_) {
            threadPool_->stop();
        }
        currentTask_       = task;
        const auto newSize = task.maxThreadCount == 0 ? boost::thread::hardware_concurrency() : task.maxThreadCount;
        resizeThreadPool(newSize);
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
        auto threadJobs = generateThreadJobs(files);
        for (auto& job : threadJobs) {
            boost::asio::post(*threadPool_, [this, &job]() {
                performThreadJob(std::move(job));
            });
        }
    }

    std::size_t calculateComparisonsCount(std::size_t filesCount)
    {
        if (filesCount < 2) {
            return 0;
        }
        return (filesCount - 1) * filesCount / 2;
    }

    /*
    struct ThreadJob
    {
        using ConstIter = decltype(FinderTask::targets)::const_iterator;
        ConstIter   firstFile;
        ConstIter   secondFile;
        ConstIter   end;
        std::size_t comparisons = 0;
    };*/
    std::vector<ThreadJob> generateThreadJobs(const FileFinder::FilePropertiesVector& files)
    {
        std::vector<ThreadJob> jobs;

        const auto comparisons = calculateComparisonsCount(files.size());
        if (comparisons < 2) {
            return jobs;
        }

        if (comparisons <= currentTask_.minimumComparisonsPerThread) {
            jobs.push_back({ files.cbegin(), files.cbegin() + 1, files.cend(), comparisons });
            return jobs;
        }

        auto comparisonsRemain = comparisons;
        jobs.push_back({ files.cbegin(), files.cbegin() + 1, files.cend(), currentTask_.minimumComparisonsPerThread });
        //auto nextFirstFile = 
        //auto nextSecondFile = 
        comparisonsRemain -= currentTask_.minimumComparisonsPerThread;
        while (comparisonsRemain != 0){
        }

        return jobs;
    }

    void performThreadJob(const ThreadJob& job)
    {
        std::cout << "Performing job on thread " << std::this_thread::get_id() << "\n";
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