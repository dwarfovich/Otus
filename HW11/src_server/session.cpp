#include "session.h"
#include "server.h"

Session::Session(TcpSocket socket, Server* server) : socket_ { std::move(socket) }, server_ { server }
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

void Session::readData(const BoostErrorCode& ec, size_t bytes_transferred)
{
    if (!ec) {
        std::istream stream(&buffer_);
        std::string  result;
        std::getline(stream, result);
        g_debugOut << result << '\n';
        sendAnswer(result.size());
        startAsyncReadData();
    } else {
        if ((ec == basio::error::eof) || (ec == basio::error::connection_reset)) {
            server_->removeSession(this);
        } else {
            g_debugOut << "Error reading data: " << ec.message() << '\n';
        }
    }
}

void Session::sendAnswer(std::size_t length)
{
    answer_ = "Received " + std::to_string(length) + " bytes\n";
    // auto self { shared_from_this() };
    basio::async_write(
        socket_, basio::buffer(answer_, answer_.size()), [this]( BoostErrorCode ec, std::size_t /*length*/) {
            if (!ec) {
            }
        });

    // boost::asio::async_write(socket_,
    //                          boost::asio::buffer(data_, length),
    //                          [this, self](boost::system::error_code ec, std::size_t /*length*/) {
    //                              if (!ec) {
    //                                  doRead();
    //                              }
    //                          });
}
