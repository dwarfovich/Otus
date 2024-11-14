#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/digester.h"

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

class TestDigester : public Digester
{
public:
    std::unique_ptr<Digester> clone() const override { return std::make_unique<DummyDigester>(); }
    std::string               calculate(const std::string_view& message) const override
    {
        return { message.cbegin(), message.cend() };
    }
};

#define CREATE_DATA_FOR_DIFFERENT_SOURCES(data1, data2)     \
    DuplicateFinder finder;                                 \
    FinderTask      task;                                   \
    task.digester       = std::make_shared<TestDigester>(); \
    task.maxThreadCount = 1;                                \
    task.blockSize      = 1;                                \
    finder.currentTask_ = task;                             \
    DigestBlocks digest1;                                   \
    DigestBlocks digest2;                                   \
    auto         s1 = StringSource { "s1", (data1) };       \
    auto         s2 = StringSource { "s2", (data2) };

TEST(DuplicateFinder, CompareDifferentStringSources_1)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("", "1");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_2)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("0", "");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_3)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("0", "1");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_4)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("0", "00");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_5)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("111", "11");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_6)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("111", "11");
    task.blockSize = 2;
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_7)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("111", "11");
    task.blockSize = 3;
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_8)
{
    CREATE_DATA_FOR_DIFFERENT_SOURCES("1111", "1110");
    task.blockSize = 4;
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}