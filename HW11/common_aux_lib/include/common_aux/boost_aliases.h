#pragma once

#include <boost/asio.hpp>

namespace basio      = boost::asio;
using BoostTcp       = basio::ip::tcp;
using TcpSocket      = BoostTcp::socket;
using BoostErrorCode = boost::system::error_code;