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

//template<typename T, typename ... Ts>
//void print_tuple_except_first(T head, Ts... args, bool ignore = true)
//{
//
//    std::cout << std::get<i>(t);
//}

template<std::size_t I, std::enable_if<I == 0, bool> = false, typename... Ts>
void print(Ts ... ts)
{
    std::cout << std::get<0>(ts);
    print<1>(ts)
}

template<std::size_t I, std::enable_if<(I > 0), bool> = false, typename T, typename... Ts>
void print(T head, Ts... ts)
{
    std::cout << '.' << std::get<0>(ts);
    constexpr if (I < std::tuple_size<T> {}){

    }
}

template<typename T, std::enable_if_t<(std::tuple_size<T>{} > 0), bool> = false>
void print_ip(const T& t)
{
    std::cout << "tuple" << std::endl;
    std::cout << 
    std::apply(
        [](auto&&... args) {
            ((std::cout << args << '\n'), ...);
        },
        t);
}
