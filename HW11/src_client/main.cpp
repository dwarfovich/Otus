#include "common_aux/port_number.h"
#include "common_aux/default_port.h"
#include "common_aux/debug_message.h"

#if defined(_WIN32) || defined(_WIN64)
    #define IS_WINDOWS_OS
    #define WIN32_LEAN_AND_MEAN 
    #include <Windows.h>
#endif // DEBUG

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

namespace ba = boost::asio;

static inline bool exitRequested = false;

#ifdef IS_WINDOWS_OS
BOOL WINAPI CtrlHandler(DWORD ctrlType)
{
    switch (ctrlType) {
        case CTRL_C_EVENT: // Handle the CTRL-C signal.
        case CTRL_CLOSE_EVENT:// CTRL-CLOSE: confirm that the user wants to exit.
        case CTRL_SHUTDOWN_EVENT:
            exitRequested = true;
            return true;
        default: return true;
    }
}
#endif // IS_WINDOWS_OS

int main(int argc, char* argv[])
{
    std::locale::global(std::locale(""));

    try {
        if (argc > 2) {
            std::cerr << "I can handle maximum one argument: connection port\n";
            return -1;
        }

        const auto            port = (argc == 2 ? boost::lexical_cast<PortNumber>(argv[1]) : g_defaultPort);
        ba::io_context        ioContext;
        ba::ip::tcp::endpoint endPoint { ba::ip::address::from_string("127.0.0.1"), port };
        ba::ip::tcp::socket   socket { ioContext };

        socket.connect(endPoint);

        g_debugOut << "Client connected to 127.0.0.1 at port " << port << '\n';
        std::string input;
        while (std::getline(std::cin, input) && !exitRequested) {
            g_debugOut << "[Sending message] "  << input << '\n';
            ba::write(socket, ba::buffer(input, input.size()));
        }

        /*ba::write(socket, ba::buffer("ping", 4));

        char   data[4];
        size_t len = socket.read_some(ba::buffer(data));
        std::cout << "receive " << len << "=" << std::string { data, len } << std::endl;*/
    } catch (const boost::system::system_error& ex) {
        std::cout << "boost exception! " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "std::exception! " << ex.what() << std::endl;
    }

    return 0;
}
