#include "session.h"
#include "server.h"

Session::Session(TcpSocket socket) : socket_ { std::move(socket) }
{
    g_debugOut << "Creating session\n";
}

Session ::~Session()
{
    g_debugOut << "Destroying session\n";
}

void Session::startAsyncReadData()
{
    basio::async_read_until(
        socket_,
        buffer_,
        '\n',
        boost::bind(&Session::readData, this, basio::placeholders::error, basio::placeholders::bytes_transferred));
}

void Session::readData(const BoostErrorCode& error, size_t bytes_transferred)
{
    if (!error) {
        std::istream stream(&buffer_);
        std::string  result;
        std::getline(stream, result);
        g_debugOut << result << '\n';
        startAsyncReadData();
    } else {
        if ((error == basio::error::eof) || (error == basio::error::connection_reset)) {
        } else {
            g_debugOut << "Error reading data: " << error.message() << '\n';
        }
    }
}

void Session::writeData(std::size_t length)
{
    // auto self { shared_from_this() };
    // boost::asio::async_write(socket_,
    //                          boost::asio::buffer(data_, length),
    //                          [this, self](boost::system::error_code ec, std::size_t /*length*/) {
    //                              if (!ec) {
    //                                  doRead();
    //                              }
    //                          });
}
