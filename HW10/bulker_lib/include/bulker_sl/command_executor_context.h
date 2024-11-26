#pragma once

#include "threaded_bulker_logger.h"
#include "bulker/bulker_command_executor.h"
#include "bulker/bulker_command_parser.h"

class CommandExecutorContext
{
public:
    CommandExecutorContext(std::size_t staticBlockSize, std::ostream& stream)
        : logger_ { std::make_shared<ThreadedBulkerLogger>(stream) }
        , executor_ { dummyStream_, logger_ }
        , parser_ { staticBlockSize }
    {
        parser_.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor_, std::placeholders::_1));
    }

    void readCommands(const char* text) {
        sstream_.clear();
        sstream_ << text;

        parser_.readCommands(sstream_);
    }

public:
    std::shared_ptr<ThreadedBulkerLogger> logger_ = nullptr;
    BulkerCommandExecutor                 executor_;
  DummyStream                           dummyStream_;
    //std::ostream& stream_;
    BulkerCommandParser                   parser_;
    std::stringstream sstream_;

};