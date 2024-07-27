#pragma once

#include <iostream>
#include <tuple>
#include <climits>

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = false>
void print_ip(const T& v)
{
    std::cout << (v >> (sizeof(v) - 1) * CHAR_BIT);
    if (sizeof(v) == 1) {
        return;
    }

    for (int i = static_cast<int>(sizeof(v)) - 2; i >= 0; --i) {
        const auto shift = static_cast<T>(i * CHAR_BIT);
        const auto mask  = static_cast<T>(0xFF) << shift;
        std::cout << '.' << ((v & mask) >> shift);
    }
}

template<typename T, std::enable_if_t<std::is_same_v<std::string, T>, bool> = false>
void print_ip(const T& str)
{
    std::cout << str;
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
}

namespace details {

template<typename Head, typename... Tail>
constexpr bool tuple_has_equal_types()
{
    return (std::is_same_v<Head, Tail> && ...);
}

template<size_t I = 0, typename... Ts, std::enable_if_t<I == 0, bool> = false>
void print_tuple(const std::tuple<Ts...>& tuple)
{
    std::cout << std::get<0>(tuple);
    print_tuple_except_first_element<I + 1>(tuple);
}

template<size_t I, typename... Ts, std::enable_if_t<I == sizeof...(Ts), bool> = false>
void print_tuple_except_first_element(const std::tuple<Ts...>& tuple)
{
    return;
}

template<size_t I, typename... Ts, std::enable_if_t<(I < sizeof...(Ts)), bool> = false>
void print_tuple_except_first_element(const std::tuple<Ts...>& tuple)
{
    std::cout << '.' << get<I>(tuple);
    print_tuple_except_first_element<I + 1>(tuple);
}

} // namespace details

template<typename... Ts, std::enable_if_t<(sizeof...(Ts) > 0), bool> = false>
void print_ip(const std::tuple<Ts...>& tuple)
{
    static_assert(details::tuple_has_equal_types<Ts ...>() && "tuple has to has elements of the single type");
    details::print_tuple(tuple);
}
