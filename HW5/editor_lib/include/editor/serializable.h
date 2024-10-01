#pragma once

#include <iosfwd>

class Serializeble
{
public:
    virtual void read(std::istream& stream) = 0;
    virtual void write(std::ostream& stream) const = 0;
};