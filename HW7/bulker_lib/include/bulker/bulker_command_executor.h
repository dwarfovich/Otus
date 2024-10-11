#pragma once

#include "command_block.h"
#include "bulker_logger.h"
#include "bulker_dummy_logger.h"
#include "bulker_command_factory.h"

#include <memory>
#include <filesystem>

#ifdef _WIN32
    #include <io.h>
    #define access _access_s
#else
    #include <unistd.h>
#endif

class BulkerCommandExecutor
{
public:
    BulkerCommandExecutor(std::ostream&                        stream,
                          const std::shared_ptr<BulkerLogger>& logger = std::make_shared<BulkerDummyLogger>())
        : factory_ { stream, logger }, stream_ { stream }, logger_ { logger }
    {
    }

    void onGotCommandBlock(const CommandBlock& block)
    {
        auto path = formLogFilePath(block.timestamp());
        logger_->setLogFile(path);
        logger_->log("Bulk: ");
        for (std::size_t i = 0; i < block.identifiers().size(); ++i) {
            auto command = factory_.create(block.identifiers()[i]);
            command->execute();
            if (i != block.identifiers().size() - 1) {
                logger_->log(", ");
            }
        }
        logger_->log("\n");
    }

    bool FileExists(const std::string& Filename) const { return access(Filename.c_str(), 0) == 0; }

private: // methods
    std::string formLogFilePath(std::time_t timestamp) const
    {
        std::string                  path { "bulk" };
        std::tm*                     tm         = std::localtime(&timestamp);
        static constexpr std::size_t bufferSize = 32;
        std::string                  buffer;
        buffer.resize(bufferSize);
        auto actualLength = std::strftime(buffer.data(), bufferSize, "%Y-%m-%d_%H-%M-%S", tm);
        buffer.resize(actualLength);
        path += buffer;

        static constexpr std::size_t maxIndex = 100;

        std::size_t     index    = 1;
        std::string     tempPath = path;
        while (std::filesystem::exists((tempPath)) && index <= maxIndex) {
            tempPath = path + '-' + std::to_string(index++);
        }

        path = std::move(tempPath);
        return path;
    }

private: // data
    BulkerCommandFactory          factory_;
    std::shared_ptr<BulkerLogger> logger_ = nullptr;
    std::ostream&                 stream_;
};