#pragma once

#include <boost/asio/thread_pool.hpp>

int testFunction() {
    boost::asio::thread_pool pool;

    return 8;
}