//#include "bulker/bulker_command_factory.h"
#include "bulker/bulker_logger.h"
#include "bulker/bulker_command_parser.h"
#include "bulker/bulker_command_executor.h"

#include <iostream>
#include <functional>

int main()
{
    auto logger = std::make_shared<BulkerLogger>(std::cout);
    BulkerCommandExecutor executor {logger};
    BulkerCommandParser parser {std::cin};
    parser.setReadyNotifier(std::bind(&BulkerCommandExecutor::onGotCommandBlock, &executor, std::placeholders::_1));

    parser.readCommands(std::cin);

    return 0;
}
