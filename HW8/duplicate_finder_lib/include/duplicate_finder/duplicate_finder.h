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

    struct FileData
    {
        const std::filesystem::path& path;
        DigestBlocks&                digest;
    };

    void performThreadJob(const PathVector& files)
    {
        using DigestVector = std::vector<DigestBlocks>;

        DigestVector digestVector(files.size());
        for (std::size_t i = 0; i < files.size(); ++i) {
            for (std::size_t j = i + 1; j < files.size(); ++j) {
                bool areSame = compareFiles({ files[i], digestVector[i] }, { files[j], digestVector[j] });
            }
        }
    }

    bool compareFiles(FileData file1Data, FileData file2Data)
    {
        std::size_t minDigestIndex = std::min(file1Data.digest.size(), file2Data.digest.size());

        for (std::size_t i = 0; i < minDigestIndex; ++i) {
            if (file1Data.digest[i] != file2Data.digest[i]) {
                return false;
            }
        }

        std::size_t       digest1Index = minDigestIndex;
        std::size_t       digest2Index = minDigestIndex;
        std::ifstream     file1 { file1Data.path };
        std::ifstream     file2 { file2Data.path };
        const auto        fileSize = std::filesystem::file_size(file1Data.path);
        const std::size_t endIndex = fileSize / currentTask_.blockSize + fileSize % currentTask_.blockSize > 0;
        while (digest1Index < endIndex) {
            if (digest1Index < file1Data.digest.size()) {
                readNextFileBlock(file1, digest1Index * currentTask_.blockSize, file1Data.digest);
            }
            if (digest2Index < file2Data.digest.size()) {
                readNextFileBlock(file2, digest2Index * currentTask_.blockSize, file2Data.digest);
            }
            if (file1Data.digest.back() != file2Data.digest.back()) {
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