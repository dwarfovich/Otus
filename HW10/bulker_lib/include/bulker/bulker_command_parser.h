#pragma once

#include "command_parser.h"
#include "command_block.h"
#include "command_identifier.h"

#include <iostream>
#include <stdexcept>
#include <optional>

class BulkerCommandParser : public CommandParser
{
public:
    BulkerCommandParser(std::size_t staticBlockSize = 3) : staticBlockSize_ { staticBlockSize }
    {
        if (staticBlockSize_ == 0) {
            throw std::invalid_argument("staticBlockSize can not be zero.");
        }
    }

    void readCommands(std::istream& stream) override
    {
        std::string command;
        while (std::getline(stream, command)) {
            if (!command.empty()) {
                auto blockToSend = processCommand(std::move(command), stream.eof());
                if (blockToSend.has_value()) {
                    sendBlock(std::move(blockToSend.value()));
                }
            }
        }
    }
    void setReadyNotifier(const ReadyNotifier& notifier) override { notifier_ = notifier; }

private: // methods
    bool isOpenDynamicBlockCommand(const std::string& str) { return str.size() == 1 && str.front() == openBlockChar; }
    bool isCloseDynamicBlockCommand(const std::string& str) { return str.size() == 1 && str.front() == closeBlockChar; }
    std::optional<CommandBlock> processCommand(std::string&& str, bool endOfFile)
    {
        if (endOfFile && blockDepth_ > 0 && !isCloseDynamicBlockCommand(str)) {
            block_.clear();
        } else {
            if (isOpenDynamicBlockCommand(str)) {
                ++blockDepth_;
                if (blockDepth_ == 1 && !block_.identifiers().empty()) {
                    return takeBlock();
                }
            } else if (isCloseDynamicBlockCommand(str)) {
                if (blockDepth_ == 0) {
                    throw std::logic_error("Invalid block ending");
                }
                --blockDepth_;
                if (blockDepth_ == 0) {
                    return takeBlock();
                }
            } else {
                block_.addCommandIdentifier(std::move(str));
                block_.setTimestampIfAbcent(std::time(nullptr));
                if ((blockDepth_ == 0 && block_.identifiers().size() == staticBlockSize_) || endOfFile) {
                    return takeBlock();
                }
            }
        }

        return {};
    }
    CommandBlock takeBlock()
    {
        CommandBlock temp { std::move(block_) };
        block_.clear();

        return temp;
    }
    void sendBlock(CommandBlock&& block) { 
        notifier_(std::move(block));
    }

private: // data
    static constexpr char openBlockChar  = '{';
    static constexpr char closeBlockChar = '}';

    CommandBlock  block_;
    ReadyNotifier notifier_;
    std::size_t   staticBlockSize_ = 3;
    std::size_t   blockDepth_      = 0;
};