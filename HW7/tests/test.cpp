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
TEST(BulkerParser, ValidStaticBlockSize1)
{
    TestExecutor       executor { 1 };
    std::istringstream stream { "cmd1" };
    executor.readCommands(stream);
    ASSERT_EQ(executor.result(), "cmd1");
}
