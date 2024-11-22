#pragma once

#include "common_aux/boost_aliases.h"
#include "common_aux/debug_message.h"

#include <boost/bind/bind.hpp>

class Server;

class Session
{
public:
    Session(TcpSocket socket);
    ~Session();

    void startAsyncReadData();

private: // methods
    void readData(const BoostErrorCode& error, size_t bytes_transferred);
    void writeData(std::size_t length);

private: // data
    static inline constexpr char messageDelimiter_ = '\n';

    TcpSocket        socket_;
    basio::streambuf buffer_;
    Server* server_ = nullptr;
};