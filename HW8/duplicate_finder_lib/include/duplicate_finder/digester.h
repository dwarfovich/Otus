#pragma once

#include <string>
#include <memory>

class Digester {
public:
    virtual std::unique_ptr<Digester> clone() const = 0;
    virtual std::string calculate(const std::string& message) const = 0;
};

class TestDigester : public Digester
{
};