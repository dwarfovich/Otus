#pragma once

#include "command_parser.h"
#include "command_block.h"
#include "command_identifier.h"

#include <iostream>
#include <stdexcept>

class BulkerCommandParser : public CommandParser
{
public:
    BulkerCommandParser(std::size_t staticBlockSize = 3)
        : /*stream_ { stream },*/ staticBlockSize_ { staticBlockSize }
    {
        if(staticBlockSize_ == 0){
            throw std::invalid_argument("staticBlockSize can not be zero.");
        }
    }

    void readCommands(std::istream& stream) override {
        std::string command;
        while (std::getline(stream, command)) {
            auto needSendBlock = processCommand(std::move(command));
            if (needSendBlock){
                sendBlock();
            }
            /*if(stream.eof()){
                sendBlock();
            }*/
            /*readCommands_.addCommandIdentifier( std::move(command) );
            notifier_(readCommands_);
            command.clear();
            readCommands_.clear();*/
        }
    }
    void setReadyNotifier(const ReadyNotifier& notifier) override { notifier_ = notifier; }

    private: // methods
        bool isOpenDynamicBlockCommand(const std::string& str){
            return str.size() == 1 && str.front() == openBlockChar;
        }
        bool isCloseDynamicBlockCommand(const std::string& str)
        {
            return str.size() == 1 && str.front() == closeBlockChar;
        }
        bool processCommand(std::string&& str){
            if (blockDepth_ == 0) {
                if (isOpenDynamicBlockCommand(str)) {
                    return true;
                    ++blockDepth_;
                } else if (isCloseDynamicBlockCommand(str)) {
                    if (blockDepth_ > 0) {
                        --blockDepth_;
                        if (blockDepth_ == 0){
                            return true;
                        }
                    }
                } else{
                    readCommands_.addCommandIdentifier(std::move(str));
                    if(readCommands_.identifiers().size() == staticBlockSize_){
                        return true;
                    }
                }
            } else {
                if (isOpenDynamicBlockCommand(str)) {
                    ++blockDepth_;
                } else if (isCloseDynamicBlockCommand(str)){
                    if (blockDepth_ > 0) {
                        --blockDepth_;
                        if (blockDepth_ == 0) {
                            return true;
                        }
                    }
                } else{
                    readCommands_.addCommandIdentifier(std::move(str));
                }
            }

            return false;
        }
        void sendBlock(){
            notifier_(readCommands_);
            readCommands_.clear();
        }

    private: // data
    static constexpr char openBlockChar = '{';
    static constexpr char closeBlockChar = '}';

    //std::istream& stream_;
    CommandBlock  readCommands_;
    ReadyNotifier notifier_;
    std::size_t staticBlockSize_ = 3;
    std::size_t blockDepth_ = 0;
};