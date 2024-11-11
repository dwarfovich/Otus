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
    FRIEND_TEST(DuplicateFinder, JobGeneration_ZeroJobs);
    FRIEND_TEST(DuplicateFinder, JobGeneration_SingleJob);
    FRIEND_TEST(DuplicateFinder, JobGeneration_1Job_3Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_2Jobs_3Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_1Job_6Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_2Jobs_6Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_3Jobs_6Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_4Jobs_6Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_4Jobs_6Comparisons_1MinComparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_5Jobs_6Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_6Jobs_6Comparisons);
    FRIEND_TEST(DuplicateFinder, JobGeneration_2Jobs_6Comparisons_ByMinComparisons);

private: // types
    using ThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;

    struct ThreadJob
    {
        using ConstIter = FileFinder::FilePropertiesVector::const_iterator;

        ThreadJob(const ConstIter& aFirstFile,
                  const ConstIter& aSecondFile,
                  const ConstIter& aEnd,
                  std::size_t      aComparisons)
            : firstFile { aFirstFile }, secondFile { aSecondFile }, end { aEnd }, comparisons { aComparisons }
        {
        }

        bool operator==(const ThreadJob& rhs) const
        {
            return std::tie(firstFile, secondFile, end, comparisons)
                   == std::tie(rhs.firstFile, rhs.secondFile, rhs.end, rhs.comparisons);
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

    std::size_t comparisonsForFirstThread(std::size_t comparisonsCount, std::size_t preferredComparions)
    {
        return threadPoolSize_ == 1 ? comparisonsCount
                                    : std::max(std::min(currentTask_.minimumComparisonsPerThread, comparisonsCount),
                                               preferredComparions);
    }

    std::vector<ThreadJob> generateThreadJobs(const FileFinder::FilePropertiesVector& files)
    {
        std::vector<ThreadJob> jobs;

        const auto comparisons = calculateComparisonsCount(files.size());
        if (comparisons == 0) {
            return jobs;
        }

        if (comparisons <= currentTask_.minimumComparisonsPerThread) {
            jobs.push_back({ files.cbegin(), files.cbegin() + 1, files.cend(), comparisons });
            return jobs;
        }

        auto comparisonsRemain = comparisons;
        auto nextFirstFile     = files.cbegin();
        auto nextSecondFile    = nextFirstFile + 1;
        auto comparisonsForNextThread =
            comparisonsForFirstThread(comparisonsRemain, comparisonsRemain / threadPoolSize_);
        for (std::size_t i = 0; i < threadPoolSize_ && comparisonsRemain > 0; ++i) {
            jobs.push_back({ nextFirstFile, nextSecondFile, files.cend(), comparisonsForNextThread });

            assert(comparisonsRemain >= comparisonsForNextThread);
            comparisonsRemain -= comparisonsForNextThread;
            while (comparisonsRemain > 0 && comparisonsForNextThread > 0) {
                const auto distanceToEnd = static_cast<std::size_t>(std::distance(nextSecondFile, files.cend()));
                if (distanceToEnd == 1) {
                    comparisonsForNextThread -= 1;
                    ++nextFirstFile;
                    nextSecondFile = nextFirstFile + 1;
                } else {
                    auto restComparisons = std::min(distanceToEnd, comparisonsForNextThread);
                    nextSecondFile += restComparisons;
                    if (nextSecondFile == files.cend()) {
                        ++nextFirstFile;
                        nextSecondFile = nextFirstFile + 1;
                    }
                    comparisonsForNextThread -= restComparisons;
                }
            }
            comparisonsForNextThread = threadPoolSize_ == i + 2
                                           ? comparisonsRemain
                                           : std::min(currentTask_.minimumComparisonsPerThread, comparisonsRemain);
        }

        assert(comparisonsRemain == 0);

        return jobs;
    }

   /* struct ThreadJob
    {
        using ConstIter = FileFinder::FilePropertiesVector::const_iterator;
        ConstIter   firstFile;
        ConstIter   secondFile;
        ConstIter   end;
        std::size_t comparisons = 0;
    };*/

    void performThreadJob(const ThreadJob& job)
    {
        
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