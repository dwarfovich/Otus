#include  "server.h"
#include "request_processor.h"

#include "db/database.h"

#include "common_aux/port_number.h"
#include "common_aux/debug_message.h"
#include "common_aux/default_port.h"
#include "common_aux/boost_aliases.h"

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind/bind.hpp>

#include <iostream>
#include <memory>
#include <unordered_set>


int main(int argc, char* argv[])
{
    try {
        if (argc > 2) {
            std::cerr << "I can handle maximum one argument: connection port\n";
            return -1;
        }

        auto database = std::make_shared<Database>();
        const auto port = (argc == 2 ? boost::lexical_cast<PortNumber>(argv[1]) : g_defaultPort);

        basio::io_context ioContext;
        Server            server { ioContext, port };
        server.setRequestProcessor(std::make_unique<RequestProcessor>(database));
        g_debugOut << "Start listening at port " << port << '\n';
        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}