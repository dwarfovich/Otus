#include "duplicate_finder/duplicate_finder.h"
#include "duplicate_finder/digester.h"
#include "duplicate_finder/file_finder.h"
#include "duplicate_finder/path_utils.h"
#include "duplicate_finder/path_uset.h"

#include <gtest/gtest.h>

#include <vector>

#define CREATE_DATA_FOR_COMPARISONS(data1, data2)            \
    DuplicateFinder finder;                                  \
    FinderTask      task;                                    \
    task.digester       = std::make_shared<DummyDigester>(); \
    task.maxThreadCount = 1;                                 \
    task.blockSize      = 1;                                 \
    finder.currentTask_ = task;                              \
    DigestBlocks digest1;                                    \
    DigestBlocks digest2;                                    \
    auto         s1 = StringSource { "s1", (data1) };        \
    auto         s2 = StringSource { "s2", (data2) };

TEST(DuplicateFinder, CompareDifferentStringSources_1)
{
    CREATE_DATA_FOR_COMPARISONS("", "1");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_2)
{
    CREATE_DATA_FOR_COMPARISONS("0", "");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_3)
{
    CREATE_DATA_FOR_COMPARISONS("0", "1");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_4)
{
    CREATE_DATA_FOR_COMPARISONS("0", "00");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_5)
{
    CREATE_DATA_FOR_COMPARISONS("111", "11");
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_6)
{
    CREATE_DATA_FOR_COMPARISONS("111", "11");
    finder.currentTask_.blockSize = 2;
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_7)
{
    CREATE_DATA_FOR_COMPARISONS("111", "11");
    task.blockSize = 3;
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareDifferentStringSources_8)
{
    CREATE_DATA_FOR_COMPARISONS("1111", "1110");
    finder.currentTask_.blockSize = 4;
    ASSERT_FALSE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareSameStringSources_1)
{
    CREATE_DATA_FOR_COMPARISONS("", "");
    ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareSameStringSources_2)
{
    CREATE_DATA_FOR_COMPARISONS("0", "0");
    ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareSameStringSources_3)
{
    CREATE_DATA_FOR_COMPARISONS("1", "1");
    finder.currentTask_.blockSize = 2;
    ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareSameStringSources_4)
{
    CREATE_DATA_FOR_COMPARISONS("12", "12");
    finder.currentTask_.blockSize = 2;
    ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareSameStringSources_5)
{
    CREATE_DATA_FOR_COMPARISONS("123", "123");
    finder.currentTask_.blockSize = 1;
    ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

TEST(DuplicateFinder, CompareSameStringSources_6)
{
    CREATE_DATA_FOR_COMPARISONS("123", "123");
    finder.currentTask_.blockSize = 2;
    ASSERT_TRUE(finder.compareSources({ s1, digest1 }, { s2, digest2 }));
}

template<typename... Args>
void addSourcesToVector(std::vector<std::shared_ptr<SourceEntity>>& vector, Args... args)
{
    (vector.push_back(std::make_shared<StringSource>(args, args)), ...);
}

#define CREATE_DATA_FOR_FINDING_DUPLICATES(...)              \
    DuplicateFinder finder;                                  \
    FinderTask      task;                                    \
    task.digester       = std::make_shared<DummyDigester>(); \
    task.maxThreadCount = 1;                                 \
    task.blockSize      = 1;                                 \
    finder.currentTask_ = task;                              \
    std::vector<std::shared_ptr<SourceEntity>> sources;      \
    addSourcesToVector(sources, __VA_ARGS__);                \
    finder.findDuplicatesForSources(sources);

TEST(DuplicateFinder, FindZeroDuplicatesForSources_1)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES();
    ASSERT_TRUE(finder.duplicates_.duplicatesMap().empty());
}

TEST(DuplicateFinder, FindZeroDuplicatesForSources_2)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES("");
    ASSERT_TRUE(finder.duplicates_.duplicatesMap().empty());
}

TEST(DuplicateFinder, FindZeroDuplicatesForSources_3)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES("1");
    ASSERT_TRUE(finder.duplicates_.duplicatesMap().empty());
}

TEST(DuplicateFinder, FindZeroDuplicatesForSources_4)
{
    CREATE_DATA_FOR_FINDING_DUPLICATES("1", "2", "", "3");
    ASSERT_TRUE(finder.duplicates_.duplicatesMap().empty());
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
    task.digester       = std::make_shared<DummyDigester>(); \
    task.maxThreadCount = 1;                                 \
    task.blockSize      = 1;                                 \
    finder.currentTask_ = task;                              \
    std::vector<std::shared_ptr<SourceEntity>> sources;      \
    addSourceVectorToVector(sources, __VA_ARGS__);           \
    finder.findDuplicatesForSources(sources);

bool hasDuplicatesSet(const Duplicates& duplicates, const std::vector<std::shared_ptr<SourceEntity>>& sources)
{
    assert(sources.size() > 0);
    for (const auto& [digest, entities] : duplicates.duplicatesMap()) {
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
    CREATE_DATA_FOR_FINDING_DUPLICATES_FROM_VECTORS(sources, v1, v2, v3, v4);
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v1));
    ASSERT_TRUE(hasDuplicatesSet(finder.duplicates_, v2));
    ASSERT_FALSE(hasDuplicatesSet(finder.duplicates_, v3));
    ASSERT_FALSE(hasDuplicatesSet(finder.duplicates_, v4));
}

TEST(PathUtils, PathIsNotUnder_1)
{
    const PathVector            blacks = {};
    const std::filesystem::path path   = "folder";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsNotUnder_2)
{
    const PathVector            blacks = { "/folder123" };
    const std::filesystem::path path   = "/folder";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsNotUnder_3)
{
    const PathVector            blacks = { "/folder" };
    const std::filesystem::path path   = "/folder123";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsNotUnder_4)
{
    const PathVector            blacks = { "/folder1/folder2" };
    const std::filesystem::path path   = "/folder1/folder3";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsNotUnder_5)
{
    const PathVector            blacks = { "/folder1/folder2/folder3" };
    const std::filesystem::path path   = "/folder1/folder3";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsNotUnder_6)
{
    const PathVector            blacks = { "/folder1/folder2" };
    const std::filesystem::path path   = "/folder1/folder3/folder2";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsNotUnder_7)
{
    const PathVector            blacks = { "/folder1/folder2/" };
    const std::filesystem::path path   = "/folder1/folder3/folder2";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}
TEST(PathUtils, PathIsNotUnder_8)
{
    const PathVector            blacks = { "/folder1/folder2/" };
    const std::filesystem::path path   = "/folder1/folder3/folder2/";
    ASSERT_FALSE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsUnder_1)
{
    const PathVector            blacks = { "/folder1" };
    const std::filesystem::path path   = "/folder1/folder2";
    ASSERT_TRUE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsUnder_2)
{
    const PathVector            blacks = { "/folder1" };
    const std::filesystem::path path   = "/folder1/file1.txt";
    ASSERT_TRUE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsUnder_3)
{
    const PathVector            blacks = { "/folder1/folder2" };
    const std::filesystem::path path   = "/folder1/folder2/file1.txt";
    ASSERT_TRUE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsUnder_4)
{
    const PathVector            blacks = { "/folder1" };
    const std::filesystem::path path   = "/folder1/folder2/folder3/file1.txt";
    ASSERT_TRUE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsUnder_5)
{
    const PathVector            blacks = { "/folder1/" };
    const std::filesystem::path path   = "/folder1/folder2/folder3/file1.txt";
    ASSERT_TRUE(pathIsUnder(path, blacks));
}

TEST(PathUtils, PathIsUnder_6)
{
    const PathVector            blacks = { "/folder1/folder2/folder3/file1.txt" };
    const std::filesystem::path path   = "/folder1/folder2/folder3/file1.txt";
    ASSERT_TRUE(pathIsUnder(path, blacks));
}

std::filesystem::path generatePathForCase(const std::string& caseName)
{
    return { TEST_DATA_PATH + caseName };
}

#define CREATE_FILE_FINDER_TASK_DATA(path, aNonRecursiveSearch)  \
    FinderTask task;                                          \
    task.digester        = std::make_shared<DummyDigester>(); \
    task.nonRecursiveSearch = (aNonRecursiveSearch);                \
    task.targets.push_back(path);                             \
    FileFinder finder;

TEST(FileFinder, FindFiles_Empty)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case1"), true);
    const auto files = finder.findFiles(task);
    ASSERT_TRUE(files.empty());
}

TEST(FileFinder, FindFiles_RecursiveEmpty)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case2"), false);
    const auto files = finder.findFiles(task);
    ASSERT_TRUE(files.empty());
}

std::size_t totalFileCount(const FileFinder::FileMap& files)
{
    std::size_t count = 0;
    for (const auto& [size, set] : files) {
        count += set.size();
    }

    return count;
}

inline constexpr std::size_t case3TotalFileCount          = 23;
inline constexpr std::size_t case3FileCountWithoutFolder3 = 20;
inline constexpr std::size_t case3TopFiles                = 4;
inline constexpr std::size_t case3FileasHeavierThan6Bytes = 7;

TEST(FileFinder, FindFiles_Physical_FullSearch)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    const auto files = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3TotalFileCount);
}

TEST(FileFinder, FindFiles_Physical_Blacklist_1)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    task.blackList.push_back(TEST_DATA_PATH + std::string { "case3/TopFolder1" });
    const auto files = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3TopFiles);
}

TEST(FileFinder, FindFiles_Physical_Blacklist_2)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), true);
    task.blackList.push_back(TEST_DATA_PATH);
    const auto files = finder.findFiles(task);
    ASSERT_TRUE(files.empty());
}

TEST(FileFinder, FindFiles_Physical_MinimalFileSize_1)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    task.minimalFileSize = 6;
    const auto files     = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3FileasHeavierThan6Bytes);
}

TEST(FileFinder, FindFiles_Physical_MinimalFileSize_2)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), true);
    task.minimalFileSize = 100;
    const auto files     = finder.findFiles(task);
    ASSERT_TRUE(files.empty());
}

inline constexpr std::size_t case3TxtFiles                         = 10;
inline constexpr std::size_t case3JpegFiles                        = 1;
inline constexpr std::size_t case3FilesWithoutExtension            = 1;
inline constexpr std::size_t case3FilesWithExtensionContainingDots = 1;

TEST(FileFinder, FindFiles_Physical_FileExtensions_1)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    task.extensionsMasks.push_back(".txt");
    const auto files = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3TxtFiles);
}

TEST(FileFinder, FindFiles_Physical_FileExtensions_2)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    task.extensionsMasks.push_back("");
    const auto files = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3FilesWithoutExtension);
}

TEST(FileFinder, FindFiles_Physical_FileExtensions_3)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    task.extensionsMasks.push_back(".txt");
    task.extensionsMasks.push_back(".jpeg");
    const auto files = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3TxtFiles + case3JpegFiles);
}

TEST(FileFinder, FindFiles_Physical_FileExtensions_4)
{
    CREATE_FILE_FINDER_TASK_DATA(generatePathForCase("case3"), false);
    task.extensionsMasks.push_back(".txt.ini.jpeg");
    const auto files = finder.findFiles(task);
    ASSERT_EQ(totalFileCount(files), case3FilesWithExtensionContainingDots);
}

inline std::atomic_bool taskFinished { false };

void waitForTaskToFinish()
{
    while (!taskFinished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    taskFinished.store(false);
}

void onTaskFinished()
{
    taskFinished.store(true);
}

std::size_t totalFileCount(const Duplicates::DuplicatesUmap& files)
{
    std::size_t count = 0;
    for (const auto& [size, set] : files) {
        count += set.size();
    }

    return count;
}

std::mutex testMutex;

class SingleThreadDuplicateFinderTest : public testing::Test
{
protected:
    void SetUp() override { testMutex.lock(); }
    void TearDown() override { testMutex.unlock(); }
};

TEST_F(SingleThreadDuplicateFinderTest, Physical_SanityCheck_1)
{
    FinderTask task;
    task.digester        = std::make_shared<DummyDigester>();
    task.nonRecursiveSearch = true;
    task.targets.push_back(generatePathForCase("case2"));

    DuplicateFinder duplicatesFinder;
    duplicatesFinder.taskFinished.connect(onTaskFinished);
    duplicatesFinder.findDuplicates(task);

    waitForTaskToFinish();

    duplicatesFinder.taskFinished.disconnect_all_slots();
    const auto& duplicates = duplicatesFinder.duplicates().duplicatesMap();
    ASSERT_EQ(totalFileCount(duplicates), 0);
}

bool duplicatesContainGroup(const Duplicates::DuplicatesUmap& duplicates, std::size_t fileSize, const PathUset& files)
{
    bool groupFound = false;
    for (const auto& [digest, entities] : duplicates) {
        if (entities.size() != files.size()) {
            continue;
        }

        groupFound = true;
        for (const auto& entity : entities) {
            const auto& normalized = entity->path().lexically_normal();
            if (entity->size() != fileSize || files.count(normalized) == 0) {
                groupFound = false;
                break;
            }
        }
        if (groupFound) {
            return true;
        }
    }

    return false;
}

std::filesystem::path formNormalizedPath(const std::string& str)
{
    std::filesystem::path path { str };
    return path.lexically_normal();
}

TEST_F(SingleThreadDuplicateFinderTest, Physical_SanityCheck_2)
{
    FinderTask task;
    task.digester        = std::make_shared<DummyDigester>();
    task.nonRecursiveSearch = false;
    task.targets.push_back(generatePathForCase("case3"));

    DuplicateFinder duplicatesFinder;
    duplicatesFinder.taskFinished.connect(onTaskFinished);
    duplicatesFinder.findDuplicates(task);

    waitForTaskToFinish();

    const auto& duplicates = duplicatesFinder.duplicates().duplicatesMap();

    PathUset group1; // empty
    group1.insert(formNormalizedPath(TEST_DATA_PATH + std::string { "case3/top1.ini" }));
    group1.insert(formNormalizedPath(TEST_DATA_PATH + std::string { "case3/top2.ini" }));
    group1.insert(formNormalizedPath(TEST_DATA_PATH + std::string { "case3/TopFolder1/TopFolderFile1.txt" }));
    group1.insert(formNormalizedPath(TEST_DATA_PATH + std::string { "case3/TopFolder1/TopFolderFile2.txt" }));

    PathUset group2; // "1"
    group2.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/TopFolderFile1.ini" });
    group2.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/TopFolderFile2.ini" });

    PathUset group3; // "hello"
    group3.insert(TEST_DATA_PATH + std::string { "case3/top1.txt" });
    group3.insert(TEST_DATA_PATH + std::string { "case3/top2.txt" });
    group3.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder4/Folder4File1.txt" });
    group3.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder4/Folder4File2.txt" });

    PathUset group4; // "Hello, world!"
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder2File1.txt" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder2File2.txt" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder3/1" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder3/1.jpeg" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder3/1.txt.ini.jpeg" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder5/Folder5File1.txt" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder5/Folder5File2.txt" });

    PathUset group5; // "6"
    group5.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder2File2.ini" });
    group5.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder5/Folder5File1.ini" });
    group5.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder5/Folder5File2.ini" });

    ASSERT_EQ(duplicates.size(), 5);

    ASSERT_TRUE(duplicatesContainGroup(duplicates, 0, group1));
    ASSERT_TRUE(duplicatesContainGroup(duplicates, 1, group2));
    ASSERT_TRUE(duplicatesContainGroup(duplicates, 5, group3));
    ASSERT_TRUE(duplicatesContainGroup(duplicates, 13, group4));
    ASSERT_TRUE(duplicatesContainGroup(duplicates, 1, group5));
}

TEST_F(SingleThreadDuplicateFinderTest, Physical_SanityCheck_3)
{
    FinderTask task;
    task.digester        = std::make_shared<DummyDigester>();
    task.nonRecursiveSearch = false;
    task.minimalFileSize = 5;
    task.extensionsMasks.push_back(".txt");
    task.targets.push_back(generatePathForCase("case3"));

    DuplicateFinder duplicatesFinder;
    duplicatesFinder.taskFinished.connect(onTaskFinished);
    duplicatesFinder.findDuplicates(task);

    waitForTaskToFinish();

    const auto& duplicates = duplicatesFinder.duplicates().duplicatesMap();

    PathUset group3; // "hello"
    group3.insert(TEST_DATA_PATH + std::string { "case3/top1.txt" });
    group3.insert(TEST_DATA_PATH + std::string { "case3/top2.txt" });
    group3.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder4/Folder4File1.txt" });
    group3.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder4/Folder4File2.txt" });

    PathUset group4; // "Hello, world!"
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder2File1.txt" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder2/Folder2File2.txt" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder5/Folder5File1.txt" });
    group4.insert(TEST_DATA_PATH + std::string { "case3/TopFolder1/Folder5/Folder5File2.txt" });

    ASSERT_EQ(duplicates.size(), 2);

    ASSERT_TRUE(duplicatesContainGroup(duplicates, 5, group3));
    ASSERT_TRUE(duplicatesContainGroup(duplicates, 13, group4));
}