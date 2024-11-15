#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/digester.h"

#include <gtest/gtest.h>

#include <vector>

class TestDigester : public Digester
{
public:
    std::unique_ptr<Digester> clone() const override { return std::make_unique<TestDigester>(); }
    std::string               calculate(const std::string_view& message) const override
    {
        return { message.cbegin(), message.cend() };
    }
};

#define CREATE_DATA_FOR_COMPARISONS(data1, data2)           \
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

// TEST(DuplicateFinder, CompareDifferentStringSources_1)
//{
//     CREATE_DATA_FOR_COMPARISONS("", "1");
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_2)
//{
//     CREATE_DATA_FOR_COMPARISONS("0", "");
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_3)
//{
//     CREATE_DATA_FOR_COMPARISONS("0", "1");
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_4)
//{
//     CREATE_DATA_FOR_COMPARISONS("0", "00");
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_5)
//{
//     CREATE_DATA_FOR_COMPARISONS("111", "11");
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_6)
//{
//     CREATE_DATA_FOR_COMPARISONS("111", "11");
//     task.blockSize = 2;
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_7)
//{
//     CREATE_DATA_FOR_COMPARISONS("111", "11");
//     task.blockSize = 3;
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareDifferentStringSources_8)
//{
//     CREATE_DATA_FOR_COMPARISONS("1111", "1110");
//     task.blockSize = 4;
//     ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareSameStringSources_1)
//{
//     CREATE_DATA_FOR_COMPARISONS("", "");
//     ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareSameStringSources_2)
//{
//     CREATE_DATA_FOR_COMPARISONS("0", "0");
//     ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareSameStringSources_3)
//{
//     CREATE_DATA_FOR_COMPARISONS("1", "1");
//     task.blockSize = 2;
//     ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareSameStringSources_4)
//{
//     CREATE_DATA_FOR_COMPARISONS("12", "12");
//     task.blockSize = 2;
//     ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareSameStringSources_5)
//{
//     CREATE_DATA_FOR_COMPARISONS("123", "123");
//     task.blockSize = 1;
//     ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }
//
// TEST(DuplicateFinder, CompareSameStringSources_6)
//{
//     CREATE_DATA_FOR_COMPARISONS("123", "123");
//     task.blockSize = 2;
//     ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
// }

template<typename... Args>
void addSourcesToVector(std::vector<std::shared_ptr<SourceEntity>>& vector, Args... args)
{
    (vector.push_back(std::make_shared<StringSource>(args, args)), ...);
}

#define CREATE_DATA_FOR_FINDING_DUPLICATES(...)             \
    DuplicateFinder finder;                                 \
    FinderTask      task;                                   \
    task.digester       = std::make_shared<TestDigester>(); \
    task.maxThreadCount = 1;                                \
    task.blockSize      = 1;                                \
    finder.currentTask_ = task;                             \
    std::vector<std::shared_ptr<SourceEntity>> sources;     \
    addSourcesToVector(sources, __VA_ARGS__);               \
    finder.findDuplicatesForSources(sources);

TEST(DuplicateFinder, FindZeroDuplicatesForSources_1)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES();
    ASSERT_TRUE(finder.duplicates_.duplicates().empty());
}

TEST(DuplicateFinder, FindZeroDuplicatesForSources_2)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES("");
    ASSERT_TRUE(finder.duplicates_.duplicates().empty());
}

TEST(DuplicateFinder, FindZeroDuplicatesForSources_3)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES("1");
    ASSERT_TRUE(finder.duplicates_.duplicates().empty());
}

TEST(DuplicateFinder, FindZeroDuplicatesForSources_4)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES("1", "2", "", "3");
    ASSERT_TRUE(finder.duplicates_.duplicates().empty());
}

template<typename... Args>
std::vector<std::shared_ptr<SourceEntity>> createSourceVector(Args... args)
{
    return { std::make_shared<StringSource>(args, args)... };
}

template<typename... Args>
void addSourceVectorToVector(std::vector<std::shared_ptr<SourceEntity>>& vector, Args... args)
{
    (vector.insert(vector.cend(), args.cbegin(), args.cend()), ...);
}

#define CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(...) \
    DuplicateFinder finder;                                  \
    FinderTask      task;                                    \
    task.digester       = std::make_shared<TestDigester>();  \
    task.maxThreadCount = 1;                                 \
    task.blockSize      = 1;                                 \
    finder.currentTask_ = task;                              \
    std::vector<std::shared_ptr<SourceEntity>> sources;      \
    addSourceVectorToVector(sources, __VA_ARGS__);           \
    finder.findDuplicatesForSources(sources);

bool hasDuplicatesSet(const Duplicates& duplicates, const std::vector<std::shared_ptr<SourceEntity>>& sources)
{
    assert(sources.size() > 0);
    for (const auto& [digest, entities] : duplicates.duplicates()) {
        bool entryFound = (entities.size() == sources.size());
        if (entryFound) {
            for (const auto& source : sources) {
                if (entities.count(source) == 0) {
                    entryFound = false;
                    break;
                }
            }
        }
        if (entryFound) {
            return true;
        }
    }

    return false;
}

TEST(DuplicateFinder, FindDuplicatesForSources_1)
{
    auto v1 = createSourceVector("1", "1");
    CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(sources, v1);
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v1));
}

TEST(DuplicateFinder, FindDuplicatesForSources_2)
{
    auto v1 = createSourceVector("1", "1");
    auto v2 = createSourceVector("2");
    CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(sources, v1, v2);
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v1));
    ASSERT_FALSE(hasDuplicatesSet(finder.duplicates_, v2));
}

TEST(DuplicateFinder, FindDuplicatesForSources_3)
{
    auto v1 = createSourceVector("1", "1");
    auto v2 = createSourceVector("2", "2");
    CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(sources, v1, v2);
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v1));
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v2));
}

TEST(DuplicateFinder, FindDuplicatesForSources_4)
{
    auto v1 = createSourceVector("1", "1");
    auto v2 = createSourceVector("2", "2", "2");
    CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(sources, v1, v2);
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v1));
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v2));
}

TEST(DuplicateFinder, FindDuplicatesForSources_5)
{
    auto v1 = createSourceVector("1", "1");
    auto v2 = createSourceVector("2", "2", "2");
    auto v3 = createSourceVector("3");
    auto v4 = createSourceVector("4", "5", "6");
    CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(sources, v1, v2,v3, v4);
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v1));
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v2));
    ASSERT_FALSE(hasDuplicatesSet(finder.duplicates_, v3));
    ASSERT_FALSE(hasDuplicatesSet(finder.duplicates_, v4));
}