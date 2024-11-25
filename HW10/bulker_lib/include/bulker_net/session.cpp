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
        auto answer = server_->handleInput(std::move(result));
        if(!answer.empty()){
            sendAnswer(std::move(answer));
        }
        startAsyncReadData();
    } else {
        if ((ec == basio::error::eof) || (ec == basio::error::connection_reset)) {
            server_->removeSession(this);
        } else {
            g_debugOut << "Error reading data: " << ec.message() << '\n';
        }
    }
}

void Session::sendAnswer(std::string answer)
{
    answer += '\n';
    basio::write(socket_, basio::buffer(answer, answer.size()));
}
