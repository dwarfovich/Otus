#pragma once

#include <iostream>

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = false>
void print_ip(const T& v)
{
    std::cout << "integral: " << v << std::endl;
}

template<typename T, std::enable_if_t<std::is_same_v<std::string, T>, bool> = false>
void print_ip(const T& str)
{
    std::cout << "string: " << str << std::endl;
}

template<typename T, std::enable_if_t<(sizeof(T::const_iterator) > 0) && !std::is_same_v<std::string, T>, bool> = false>
void print_ip(const T& c)
{
    if (c.empty()) {
        return;
    }

    std::cout << *c.cbegin();
    for (auto i = ++c.cbegin(); i != c.cend(); ++i) {
        std::cout << '.' << *i;
    }
    std::cout << std::endl;
}

namespace details {

template<size_t I = 0, typename... Ts, typename std::enable_if_t<I == 0, bool>  = false>
void printTuple(const std::tuple<Ts...>& tuple)
{
    std::cout << std::get<0>(tuple);
    printTupleAfter0<I + 1>(tuple);
}

template<size_t I, typename... Ts, typename std::enable_if_t<I == sizeof...(Ts), bool> = false>
void printTupleAfter0(const std::tuple<Ts...>& tuple)
{
    return;
}

template<size_t I, typename... Ts, typename std::enable_if_t<(I < sizeof...(Ts)), bool> = false>
void printTupleAfter0(const std::tuple<Ts...>& tuple)
{
    std::cout << '.' << get<I>(tuple);
    printTupleAfter0<I + 1>(tuple);
}

} // namespace details

template<typename T, std::enable_if_t<(std::tuple_size<T> {} > 0), bool> = false>
void print_ip(const T& t)
{
    details::    printTuple(t);
}
