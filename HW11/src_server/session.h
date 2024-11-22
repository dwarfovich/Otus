#pragma once

#include "common_aux/boost_aliases.h"
#include "common_aux/debug_message.h"

#include <boost/bind/bind.hpp>

class Server;

class Session
{
public:
    Session(TcpSocket socket, Server* server);
    ~Session();

    void startAsyncReadData();

private: // methods
    void readData(const BoostErrorCode& error, size_t bytes_transferred);
    void sendAnswer(std::size_t length);

private: // data
    static inline constexpr char messageDelimiter_ = '\n';

    TcpSocket        socket_;
    basio::streambuf buffer_;
    Server* server_ = nullptr;
    std::string answer_;
};

struct SessionHasher
{
    using is_transparent = std::true_type;

    template<class P>
    size_t operator()(const P& p) const
    {
        return std::hash<P> {}(p);
    }
};

struct SessionComparator
{
    using is_transparent = std::true_type;

    template<class P, class Q>
    bool operator()(const P& lhs, const Q& rhs) const
    {
        return std::to_address(lhs) == std::to_address(rhs);
    }
};