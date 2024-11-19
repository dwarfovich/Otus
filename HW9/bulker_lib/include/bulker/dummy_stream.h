#pragma once

#include <ostream>

class NullBuffer : public std::streambuf
{
public:
    int overflow(int c) { return c; }
};

class DummyStream : public std::ostream
{
public:
    DummyStream() : std::ostream{&buffer_} {}

private:
    NullBuffer buffer_;
};
