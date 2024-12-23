#pragma once

#include "path_vector.h"
#include "duplicates.h"
#include "finder_task.h"
#include "file_finder.h"
#include "signal.h"
#include "digest_blocks.h"

#include <gtest/gtest_prod.h>

#include <boost/thread.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_future.hpp>

#include <fstream>

class DuplicateFinder
{
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_1);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_2);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_3);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_4);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_5);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_6);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_7);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_8);
    FRIEND_TEST(DuplicateFinder, CompareDifferentStringSources_9);
    FRIEND_TEST(DuplicateFinder, CompareSameStringSources_1);
    FRIEND_TEST(DuplicateFinder, CompareSameStringSources_2);
    FRIEND_TEST(DuplicateFinder, CompareSameStringSources_3);
    FRIEND_TEST(DuplicateFinder, CompareSameStringSources_4);
    FRIEND_TEST(DuplicateFinder, CompareSameStringSources_5);
    FRIEND_TEST(DuplicateFinder, CompareSameStringSources_6);
    FRIEND_TEST(DuplicateFinder, FindZeroDuplicatesForSources_1);
    FRIEND_TEST(DuplicateFinder, FindZeroDuplicatesForSources_2);
    FRIEND_TEST(DuplicateFinder, FindZeroDuplicatesForSources_3);
    FRIEND_TEST(DuplicateFinder, FindZeroDuplicatesForSources_4);
    FRIEND_TEST(DuplicateFinder, FindDuplicatesForSources_1);
    FRIEND_TEST(DuplicateFinder, FindDuplicatesForSources_2);
    FRIEND_TEST(DuplicateFinder, FindDuplicatesForSources_3);
    FRIEND_TEST(DuplicateFinder, FindDuplicatesForSources_4);
    FRIEND_TEST(DuplicateFinder, FindDuplicatesForSources_5);

private: // types
    using ThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;
    struct SourceData
    {
        SourceEntity& source;
        DigestBlocks& digest;
    };

public: // methods
    const Duplicates& duplicates()  const{
        return duplicates_;
    }    

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
                findDuplicatesForSources(jobFiles);
            });
        }
    }

    void findDuplicatesForSources(const SourceVector& files)
    {
        using DigestVector = std::vector<DigestBlocks>;

        DigestVector digestVector(files.size());
        for (std::size_t i = 0; i < files.size(); ++i) {
            for (std::size_t j = i + 1; j < files.size(); ++j) {
                bool areSame = compareSources({ *files[i], digestVector[i] }, { *files[j], digestVector[j] });
                if (areSame) {
                    duplicates_.addDuplicate(digestVector[i], files[i]);
                    duplicates_.addDuplicate(digestVector[j], files[j]);
                }
            }
        }
    }

    bool compareSources(SourceData source1Data, SourceData source2Data)
    {
        if (source1Data.source.size() != source2Data.source.size()){
            return false;
        }

        const auto minEndDigestIndex = std::min(source1Data.digest.size(), source2Data.digest.size());
        for (std::size_t i = 0; i < minEndDigestIndex; ++i) {
            if (source1Data.digest[i] != source2Data.digest[i]) {
                return false;
            }
        }

        auto              digest1Index = minEndDigestIndex;
        auto              digest2Index = minEndDigestIndex;
        const auto        fileSize     = source1Data.source.size();
        const std::size_t endIndex     = (fileSize / currentTask_.blockSize) + (fileSize % currentTask_.blockSize > 0);
        while (digest1Index < endIndex) {
            if (digest1Index >= source1Data.digest.size()) {
                const auto& nextBlock = source1Data.source.getNextBlock(currentTask_.blockSize);
                source1Data.digest.push_back(currentTask_.digester->calculate(nextBlock));
            }
            if (digest2Index >= source2Data.digest.size()) {
                const auto& nextBlock = source2Data.source.getNextBlock(currentTask_.blockSize);
                source2Data.digest.push_back(currentTask_.digester->calculate(nextBlock));
            }
            if (source1Data.digest.back() != source2Data.digest.back()) {
                return false;
            }
            ++digest1Index;
            ++digest2Index;
        }

        return true;
    }

    void readNextFileBlock(std::ifstream& file, std::size_t startPos, DigestBlocks& digest)
    {
        file.seekg(startPos);
        std::string block;
        block.resize(currentTask_.blockSize);
        file.read(block.data(), currentTask_.blockSize);
        digest.push_back(currentTask_.digester->calculate(block));
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