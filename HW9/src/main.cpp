#include "bulker/bulker_logger.h"
#include "bulker/bulker_command_parser.h"
#include "bulker/bulker_command_executor.h"

#include <iostream>
#include <functional>
#include <charconv>
#include <sstream>
#include <cstring>

int main(int argc, char* argv[])
{
    std::size_t staticBlockSize = 3;
    if (argc == 2) {
        auto [ptr, errorCode] = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), staticBlockSize);
        if (errorCode != std::errc {}) {
            std::cout << "Wrong arguments\n";
            return -1;
        }
    }

    DummyStream           dummyStream;
    auto                  logger = std::make_shared<BulkerLogger>(std::cout, std::ref(dummyStream));
    logger->enableLogToFile();
    BulkerCommandExecutor executor { dummyStream, logger };
    BulkerCommandParser   parser { staticBlockSize };
    parser.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor, std::placeholders::_1));

    parser.readCommands(std::cin);

    return 0;
}
