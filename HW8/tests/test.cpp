#include "duplicate_finder/file_properties.h"
#include "duplicate_finder/duplicate_finder.h"

#include <gtest/gtest.h>

#include <vector>

std::vector<FileProperties> generateFileProperties(std::size_t count)
{
    std::vector<FileProperties> files;
    for (std::size_t i = 0; i < count; ++i) {
        files.push_back({ "file_" + std::to_string(i), 1 });
    }

    return files;
}

/*TEST(DuplicateFinder, CalculateComparisons_ZeroResult){
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


TEST(DuplicateFinder, JobGeneration_ZeroJobs)
{
    DuplicateFinder finder;
    finder.currentTask_.minimumComparisonsPerThread = 1;

    auto files = generateFileProperties(0);
    auto jobs = finder.generateThreadJobs(files);
    ASSERT_TRUE(jobs.empty());

    files = generateFileProperties(1);
    jobs  = finder.generateThreadJobs(files);
    ASSERT_TRUE(jobs.empty());

    finder.currentTask_.minimumComparisonsPerThread = 2;
    files = generateFileProperties(0);
    jobs  = finder.generateThreadJobs(files);
    ASSERT_TRUE(jobs.empty());

    files = generateFileProperties(1);
    jobs  = finder.generateThreadJobs(files);
    ASSERT_TRUE(jobs.empty());
}

TEST(DuplicateFinder, JobGeneration_SingleJob)
{
    using ThreadJobs = std::vector<DuplicateFinder::ThreadJob>;

    DuplicateFinder finder;
    finder.currentTask_.minimumComparisonsPerThread = 1;



    auto files    = generateFileProperties(2);
    auto jobs     = finder.generateThreadJobs(files);
    auto expected = ThreadJobs { { files.cbegin(), files.cbegin() + 1, files.cend(), 1 } };
    ASSERT_EQ(jobs, expected);

    finder.currentTask_.minimumComparisonsPerThread = 2;

    files    = generateFileProperties(2);
    jobs     = finder.generateThreadJobs(files);
    expected = ThreadJobs { { files.cbegin(), files.cbegin() + 1, files.cend(), 1 } };
    ASSERT_EQ(jobs, expected);

    finder.currentTask_.minimumComparisonsPerThread = 3;

    files                                           = generateFileProperties(2);
    jobs                                            = finder.generateThreadJobs(files);
    expected = ThreadJobs { { files.cbegin(), files.cbegin() + 1, files.cend(), 1 } };
    ASSERT_EQ(jobs, expected);
}*/

/*
    struct ThreadJob
    {
        using ConstIter = FileFinder::FilePropertiesVector::const_iterator;
        ConstIter   firstFile;
        ConstIter   secondFile;
        ConstIter   end;
        std::size_t comparisons = 0;
    };
    */

#define ASSERT_JOBS_EQ(minimumComparisonsPerThreadInput, threadsCount, files, expectedJobs) \
    DuplicateFinder finder;                                                                 \
    finder.currentTask_.minimumComparisonsPerThread = (minimumComparisonsPerThreadInput);   \
    finder.resizeThreadPool((threadsCount));                                                \
    auto jobs = finder.generateThreadJobs(files);                                           \
    ASSERT_EQ(jobs, (expectedJobs));

TEST(DuplicateFinder, JobGeneration_2Jobs_3Comparisons)
{
    using ThreadJobs = std::vector<DuplicateFinder::ThreadJob>;

    auto files       = generateFileProperties(3); // 3 comparisons
    auto expected    = ThreadJobs { { files.cbegin(), files.cbegin() + 1, files.cend(), 1 },
                                    { files.cbegin(), files.cbegin() + 2, files.cend(), 2 } };
    ASSERT_JOBS_EQ(1, 2, files, expected);
}

TEST(DuplicateFinder, JobGeneration_2Jobs_6Comparisons)
{
    using ThreadJobs = std::vector<DuplicateFinder::ThreadJob>;
    auto files       = generateFileProperties(4); // 6 comparisons
    auto expected    = ThreadJobs { { files.cbegin(), files.cbegin() + 1, files.cend(), 3 },
                                    { files.cbegin() + 1, files.cbegin() + 2, files.cend(), 3 } };
    ASSERT_JOBS_EQ(2, 2, files, expected);
}

TEST(DuplicateFinder, JobGeneration_6Jobs_6Comparisons)
{
    using ThreadJobs = std::vector<DuplicateFinder::ThreadJob>;
    auto files       = generateFileProperties(4); // 6 comparisons
    auto expected    = ThreadJobs { { files.cbegin(), files.cbegin() + 1, files.cend(), 1 },
                                    { files.cbegin(), files.cbegin() + 2, files.cend(), 1 },
                                    { files.cbegin(), files.cbegin() + 3, files.cend(), 1 },
                                    { files.cbegin() + 1, files.cbegin() + 2, files.cend(), 1 },
                                    { files.cbegin() + 1, files.cbegin() + 3, files.cend(), 1 },
                                    { files.cbegin() + 2, files.cbegin() + 3, files.cend(), 1 } };
    ASSERT_JOBS_EQ(2, 6, files, expected);
}