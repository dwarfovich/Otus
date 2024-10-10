#include "bulker/bulker_command_parser.h"
#include "bulker/bulker_command_executor.h"

#include <gtest/gtest.h>

struct TestExecutor
{
    TestExecutor(std::size_t blockSize) : executor { outStream }, parser { blockSize }
    {
        parser.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor, std::placeholders::_1));
    }

    void readCommands(std::istream& inStream) { parser.readCommands(inStream);
}

    std::string result() const { return outStream.str(); }

    std::ostringstream    outStream;
    BulkerCommandExecutor executor;
    BulkerCommandParser   parser;
};

#define ASSERT_OUTPUT(blockSize, input, expected)          \
    TestExecutor       executor { (blockSize) }; \
    std::istringstream stream { (input) };       \
    executor.readCommands(stream);               \
    ASSERT_EQ(executor.result(), std::string{(expected)});

#define ASSERT_THROWS(blockSize, input) \
{ TestExecutor       executor { (blockSize) }; \
std::istringstream stream { (input) }; \
ASSERT_ANY_THROW(executor.readCommands(stream));}

TEST(BulkerParser, StaticBlockSize1)
{
    ASSERT_OUTPUT(1, "cmd1", "Bulk: cmd1\n");
}

TEST(BulkerParser, MultipleStaticBlockSize1)
{
    ASSERT_OUTPUT(1, "cmd1\ncmd1\ncmd2", "Bulk: cmd1\nBulk: cmd1\nBulk: cmd2\n");
}

TEST(BulkerParser, MultipleStaticBlockSize2)
{
    ASSERT_OUTPUT(2, "cmd1\ncmd1", "Bulk: cmd1, cmd1\n");
}

TEST(BulkerParser, MultipleStaticBlockSize2_2)
{
    ASSERT_OUTPUT(2, "cmd1\ncmd1\ncmd2\ncmd3" , "Bulk: cmd1, cmd1\nBulk: cmd2, cmd3\n");
}

TEST(BulkerParser, MultipleStaticBlockSize2_3)
{
    ASSERT_OUTPUT(2, "cmd1\ncmd1\ncmd2", "Bulk: cmd1, cmd1\nBulk: cmd2\n");
}

TEST(BulkerParser, StartDynamicBlock)
{
    ASSERT_OUTPUT(1, "{\ncmd1\n}", "Bulk: cmd1\n");
}

TEST(BulkerParser, StartDynamicBlock2)
{
    ASSERT_OUTPUT(1, "{\ncmd1\ncmd1\n}", "Bulk: cmd1, cmd1\n");
}

TEST(BulkerParser, StartDynamicBlock3)
{
    ASSERT_OUTPUT(1, "{\ncmd1\ncmd2\ncmd3\n}", "Bulk: cmd1, cmd2, cmd3\n");
}

TEST(BulkerParser, StartDynamicBlockNested)
{
    ASSERT_OUTPUT(1, "{\n{\ncmd1\n}\n}", "Bulk: cmd1\n");
}

TEST(BulkerParser, StartDynamicBlockNested_2)
{
    ASSERT_OUTPUT(1, "{\n{\ncmd1\n}\n{\ncmd2\n}\n}", "Bulk: cmd1\nBulk: cmd2\n");
}

TEST(BulkerParser, StartDynamicBlockNested_3)
{
    ASSERT_OUTPUT(1, "{\n{\ncmd1\n{\ncmd3\n}\n}\n{\ncmd2\n}\n}", "Bulk: cmd1, cmd3\nBulk: cmd2\n");
}

TEST(BulkerParser, StartDynamicBlockNested_4)
{
    ASSERT_OUTPUT(1, "{\n{\ncmd1\n{\ncmd3\n}\n}\n{\ncmd2\n{\ncmd4\n{\ncmd5\n}\n}\n}\n}", "Bulk: cmd1, cmd3\nBulk: cmd2, cmd4, cmd5\n");
}

TEST(BulkerParser, MixedBlocks)
{
    ASSERT_OUTPUT(1, "cmd1\n{\ncmd2\n}", "Bulk: cmd1\nBulk: cmd2\n");
}

TEST(BulkerParser, MixedBlocks_2)
{
    ASSERT_OUTPUT(1, "cmd1\ncmd2\n{\ncmd2\n{\ncmd3\n}\n}", "Bulk: cmd1\nBulk: cmd2\nBulk: cmd2, cmd3\n");
}

TEST(BulkerParser, MixedBlocks_3)
{
    ASSERT_OUTPUT(2, "cmd1\ncmd2\n{\ncmd2\n{\ncmd3\n}\n}", "Bulk: cmd1, cmd2\nBulk: cmd2, cmd3\n");
}

TEST(BulkerParser, MixedBlocks_StaticBlockInterrupted)
{
    ASSERT_OUTPUT(3, "cmd1\ncmd2\n{\ncmd2\n{\ncmd3\n}\n}", "Bulk: cmd1, cmd2\nBulk: cmd2, cmd3\n");
}

TEST(BulkerParser, MixedBlocks_StaticBlockInterrupted_2)
{
    ASSERT_OUTPUT(3, "cmd1\ncmd2\n{\ncmd2\n{\ncmd3\n}\n}\ncmd4", "Bulk: cmd1, cmd2\nBulk: cmd2, cmd3\nBulk: cmd4\n");
}

TEST(BulkerParser, DynamicBlockInterrupted)
{
    ASSERT_OUTPUT(3, "\n{\ncmd1\ncmd2", "");
}

TEST(BulkerParser, DynamicBlockInterrupted_2)
{
    ASSERT_OUTPUT(3, "{\ncmd1\n{\ncmd2", "");
}

TEST(BulkerParser, ThrowsOnWrongBlockClosing)
{
    ASSERT_THROWS(1, "}");
    ASSERT_THROWS(2, "}");
    ASSERT_THROWS(1, "\n}");
    ASSERT_THROWS(2, "\n}");
    ASSERT_THROWS(2, "cmd1\n}");
    ASSERT_THROWS(2, "{\ncmd1\n}\n}");
    ASSERT_THROWS(2, "cmd1\n{\ncmd1\n}\n}");
}