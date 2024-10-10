#include "bulker/bulker_logger.h"
#include "bulker/bulker_command_parser.h"
#include "bulker/bulker_command_executor.h"

#include <iostream>
#include <functional>
#include <charconv>
#include <sstream>

int main(int argc, char* argv[])
{
    std::size_t staticBlockSize = 3;
    //if(argc == 2){
    //    const auto& result = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), staticBlockSize);
    //}
    auto logger = std::make_shared<BulkerLogger>(std::cout);
    BulkerCommandExecutor executor {
        std::cerr, logger
    };
    BulkerCommandParser parser {};
    parser.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor, std::placeholders::_1));

    std::stringstream stream {"cmd1\ncmd2\ncmd3\n{\ncmd4"};
    parser.readCommands(stream);
    //parser.readCommands(std::cin);

    return 0;
}
