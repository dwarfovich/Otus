#include "common_aux/port_number.h"
#include "common_aux/default_port.h"
#include "common_aux/debug_message.h"
#include "common_aux/boost_aliases.h"

#if defined(_WIN32) || defined(_WIN64)
    #define IS_WINDOWS_OS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif // DEBUG

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <iostream>

static inline bool exitRequested = false;

#ifdef IS_WINDOWS_OS
BOOL WINAPI CtrlHandler(DWORD ctrlType)
{
    switch (ctrlType) {
        case CTRL_C_EVENT:     // Handle the CTRL-C signal.
        case CTRL_CLOSE_EVENT: // CTRL-CLOSE: confirm that the user wants to exit.
        case CTRL_SHUTDOWN_EVENT: exitRequested = true; return true;
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

        const auto         port = (argc == 2 ? boost::lexical_cast<PortNumber>(argv[1]) : g_defaultPort);
        basio::io_context  ioContext;
        BoostTcp::endpoint endPoint { basio::ip::address::from_string("127.0.0.1"), port };
        BoostTcp::socket   socket { ioContext };

        socket.connect(endPoint);

        g_debugOut << "Client connected to 127.0.0.1 at port " << port << '\n';
        std::string input;
        while (std::getline(std::cin, input) && !exitRequested) {
            input += '\n';
            g_debugOut << "[Sending message]: " << input;
            basio::write(socket, basio::buffer(input, input.size()));
            basio::streambuf buffer;
            basio::read_until(socket, buffer, boost::regex("\n"));
            //basio::read(socket, buffer);
            std::istream iStream { &buffer };
            std::string  line;
            std::getline(iStream, line);
            std::cout << "[Response]: " << line << '\n';
            if(line.find("OK") != std::string::npos){
                // do nothing
            } else if(line.size() >= 3 && line.starts_with("ERR")){
                // do nothing
            } else{
                while(line != "OK"){
                    basio::read_until(socket, buffer, boost::regex("\n"));
                    std::getline(iStream, line);
                    std::cout << line << '\n';
                }
            }
        }
    } catch (const boost::system::system_error& ex) {
        std::cout << "boost exception! " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "std::exception! " << ex.what() << std::endl;
    }

    return 0;
}
