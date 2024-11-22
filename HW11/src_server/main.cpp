//
// Based on Boost' async_tcp_echo_server.cpp by Christopher M. Kohlhoff (chris at kohlhoff dot com)
// ~~~~~~~~~~~~~~~~~~~~~~~~~

#include "common_aux/port_number.h"
#include "common_aux/debug_message.h"
#include "common_aux/default_port.h"

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket) : socket_ { std::move(socket) } {
        g_debugOut << "Creating session\n";
    }
    ~Session() { g_debugOut << "Destroying session\n"; }

    void start() { doRead(); }

private: // methods
    void doRead()
    {
        auto self { shared_from_this() };
        socket_.async_read_some(
            boost::asio::buffer(data_, maxDataLength), [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    g_debugOut << "[Received " << length << " bytes]: " << std::string { data_, length } << '\n';
                    //doWrite(length);
                }
            });
    }

    void doWrite(std::size_t length)
    {
        auto self { shared_from_this() };
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(data_, length),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec) {
                                         doRead();
                                     }
                                 });
    }

private: // data
    static inline constexpr std::size_t maxDataLength = 1024;

    tcp::socket socket_;
    char        data_[maxDataLength] = {};
};

class Server
{
public:
    Server(boost::asio::io_context& ioContext, PortNumber port) : acceptor_(ioContext, tcp::endpoint(tcp::v4(), port))
    {
        if (port == 0) {
            throw std::runtime_error("port cann't be zero");
        }
        doAccept();
    }

private: // methods
    void doAccept()
    {
        g_debugOut << "doAccept()\n";
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }

            doAccept();
        });
    }

private: // data
    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
    try {
        if (argc > 2) {
            std::cerr << "I can handle maximum one argument: connection port\n";
            return -1;
        }

        const auto port = (argc == 2 ? boost::lexical_cast<PortNumber>(argv[1]) : g_defaultPort);

        boost::asio::io_context ioContext;
        Server                  server(ioContext, port);
        g_debugOut << "Start listening at port " << port << '\n';
        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}