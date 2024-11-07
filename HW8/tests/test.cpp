#include "duplicate_finder/file_properties.h"
#include "duplicate_finder/duplicate_finder.h"

#include <gtest/gtest.h>

#include <vector>

std::vector<FileProperties> generateFileProperties(std::size_t count){
    std::vector<FileProperties> files;
    for (std::size_t i = 0; i < count; ++i){
        files.push_back({"file_" + std::to_string(i), 1});
    }

    return files;
}

TEST(DuplicateFinder, CalculateComparisons_ZeroResult){
    DuplicateFinder finder;
    ASSERT_EQ(finder.calculateComparisonsCount(0), 0);
    ASSERT_EQ(finder.calculateComparisonsCount(1), 0);
}

TEST(DuplicateFinder, CalculateComparisons)
{
    DuplicateFinder finder;
    ASSERT_EQ(finder.calculateComparisonsCount(2), 1);
    ASSERT_EQ(finder.calculateComparisonsCount(3), 3);
    ASSERT_EQ(finder.calculateComparisonsCount(4), 6);
    ASSERT_EQ(finder.calculateComparisonsCount(5), 10);
}

//TEST(DuplicateFinder, JobGeneration_WithZeroJobs)
//{
//    DuplicateFinder finder;
//    finder.currentTask_.minimumComparisonsPerThread = 1;
//
//    auto files = generateFileProperties(0);
//    auto jobs = finder.generateThreadJobs(files);
//    ASSERT_TRUE(jobs.empty());
//
//    files = generateFileProperties(1);
//    jobs  = finder.generateThreadJobs(files);
//    ASSERT_TRUE(jobs.empty());
//
//    finder.currentTask_.minimumComparisonsPerThread = 2;
//    files = generateFileProperties(0);
//    jobs  = finder.generateThreadJobs(files);
//    ASSERT_TRUE(jobs.empty());
//
//    files = generateFileProperties(1);
//    jobs  = finder.generateThreadJobs(files);
//    ASSERT_TRUE(jobs.empty());
//}
