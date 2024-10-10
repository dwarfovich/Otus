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
        blocks_.resize(1);
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
        if (blockDepth_ == 0) {
            if (isOpenDynamicBlockCommand(str)) {
                ++blockDepth_;
                if (!blocks_[0].identifiers().empty()) {
                    return std::move(blocks_[0]);
                }
            } else if (isCloseDynamicBlockCommand(str)) {
                throw std::logic_error("Wrong block ending");
            } else {
                if (blocks_.back().identifiers().size() == staticBlockSize_) {
                    blocks_.back().clear();
                }
                blocks_.back().addCommandIdentifier(std::move(str));
                if (blocks_.back().identifiers().size() == staticBlockSize_ || endOfFile) {
                    return std::move(blocks_.back());
                }
            }
        } else {
            if (endOfFile) {
                blocks_.resize(1);
            }
            if (isOpenDynamicBlockCommand(str)) {
                ++blockDepth_;
            } else if (isCloseDynamicBlockCommand(str)) {
                std::optional<CommandBlock> b;
                if (!blocks_.back().identifiers().empty()) {
                    b = std::move(blocks_.back());
                }
                if (blockDepth_ > 0) {
                    --blockDepth_;
                    if (blocks_.size() > 1) {
                        blocks_.resize(blocks_.size() - 1);
                    }
                }
                return b;

            } else {
                blocks_.back().addCommandIdentifier(std::move(str));
            }
        }

        return {};
    }
    void sendBlock(CommandBlock&& block)
    {
        notifier_(std::move(block));
    }

private: // data
    static constexpr char openBlockChar  = '{';
    static constexpr char closeBlockChar = '}';

    std::vector<CommandBlock> blocks_;
    ReadyNotifier notifier_;
    std::size_t   staticBlockSize_ = 3;
    std::size_t   blockDepth_      = 0;
};