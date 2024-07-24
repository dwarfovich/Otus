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
    if(c.empty()){
        return;
    }

    std::cout << *c.cbegin();
    for (auto i = ++c.cbegin(); i != c.cend(); ++i){
        std::cout << '.' << *i;
    }
    std::cout << std::endl;
}