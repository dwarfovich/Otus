#pragma once

#include <boost/signals2.hpp>

template<typename T>
using Signal = boost::signals2::signal<T>;