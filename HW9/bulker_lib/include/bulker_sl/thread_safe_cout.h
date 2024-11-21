#pragma once

#include <mutex>
#include <iostream>

class ThreadSafeCout : private std::streambuf, public std::ostream
{
public:
    ThreadSafeCout() : std::ostream(this) {}

    template<typename T>
    ThreadSafeCout& operator<<(const T& t)
    {
        std::lock_guard lock { mutex_ };
        std::cout << t;

        return *this;
    }

private:
    int overflow(int c) override
    {
        putCharToCout(c);
        return 0;
    }

    void putCharToCout(char c) { std::cout.put(c); }

private:
    std::mutex mutex_;
};

inline static ThreadSafeCout threadSafeCout;