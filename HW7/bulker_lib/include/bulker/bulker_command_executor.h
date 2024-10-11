#pragma once

#include "command_block.h"
#include "bulker_logger.h"
#include "bulker_dummy_logger.h"
#include "bulker_command_factory.h"

#include <memory>

class BulkerCommandExecutor
{
public:
    BulkerCommandExecutor(std::ostream& stream, const std::shared_ptr<BulkerLogger>& logger = std::make_shared<BulkerDummyLogger>()) : factory_ {stream, logger }, stream_{stream}, logger_{logger} {
    }
    
    void onGotCommandBlock(const CommandBlock& block) {
        auto path = formLogFilePath(block.timestamp());
        logger_->setLogFile(path);
        logger_->log("Bulk: ");
        for (std::size_t i = 0; i < block.identifiers().size(); ++i){
            auto command = factory_.create(block.identifiers()[i]);     
            command->execute();
            if (i != block.identifiers().size() - 1) {
                logger_->log(", ");
            }
        }
        logger_->log("\n");
    }

    private: // methods
        std::string formLogFilePath(std::time_t timestamp) const {
            std::string path {"bulk"};
            std::tm*    tm = std::localtime(&timestamp);
            static constexpr std::size_t bufferSize = 32;
            std::string buffer (bufferSize, '\0');
            // Format: Mo, 15.06.2009 20:20:00
            std::strftime(buffer.data(), bufferSize, "%Y-%m-%d_%H-%M-%S", tm);  

            return path+buffer;
        }

    private: // data
        BulkerCommandFactory factory_;
        std::shared_ptr<BulkerLogger> logger_ = nullptr;
        std::ostream& stream_;
};