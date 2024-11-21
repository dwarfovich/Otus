#pragma once

#include "threaded_bulker_logger.h"
#include "bulker_command_executor.h"
#include "bulker_command_parser.h"
/*DummyStream           dummyStream;
auto                  logger = std::make_shared<BulkerLogger>(std::cout, std::ref(dummyStream));
logger->enableLogToFile();
BulkerCommandExecutor executor { dummyStream, logger };
BulkerCommandParser   parser { staticBlockSize };
parser.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor, std::placeholders::_1));

parser.readCommands(std::cin);*/

class CommandExecutorContext
{
public:
    CommandExecutorContext(std::size_t staticBlockSize)
        : logger_ { std::make_shared<ThreadedBulkerLogger>() }
        , executor_ { dummyStream_, logger_ }
        , parser_ { staticBlockSize }
    {
        parser_.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor_, std::placeholders::_1));
    }

    void readCommands(const char* text, std::size_t size) {
        std::cout << "Context is reading commands in " << std::this_thread::get_id() << '\n';
        sstream_.clear();
        sstream_ << text;

        parser_.readCommands(sstream_);
    }

private:
    std::shared_ptr<ThreadedBulkerLogger> logger_ = nullptr;
    BulkerCommandExecutor                 executor_;
    DummyStream                           dummyStream_;
    BulkerCommandParser                   parser_;
    std::stringstream sstream_;

};