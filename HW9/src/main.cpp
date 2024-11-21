// #include "bulker/bulker_logger.h"
// #include "bulker/bulker_command_parser.h"
// #include "bulker/bulker_command_executor.h"

#include "bulker_sl/async.h"

#include <iostream>
#include <functional>
#include <charconv>
#include <sstream>
#include <cstring>

int main(int argc, char* argv[])
{
    std::size_t staticBlockSize = 1;
    if (argc == 2) {
        auto [ptr, errorCode] = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), staticBlockSize);
        if (errorCode != std::errc {}) {
            std::cout << "Wrong arguments\n";
            return -1;
        }
    }

    auto h  = async::connect(staticBlockSize);
    auto h2 = async::connect(staticBlockSize);
    async::receive(h, "cmd1\ncmd2", 1);
    //async::receive(h, "1", 1);
    //async::receive(h2, "1\n", 2);
    //async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    //async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);

    /*DummyStream           dummyStream;
    auto                  logger = std::make_shared<BulkerLogger>(std::cout, std::ref(dummyStream));
    logger->enableLogToFile();
    BulkerCommandExecutor executor { dummyStream, logger };
    BulkerCommandParser   parser { staticBlockSize };
    parser.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor, std::placeholders::_1));

    parser.readCommands(std::cin);*/

    return 0;
}
