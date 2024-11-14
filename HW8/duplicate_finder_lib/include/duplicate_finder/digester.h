#pragma once

#include <string>
#include <memory>

class Digester {
public:
    virtual std::unique_ptr<Digester> clone() const = 0;
    virtual std::string               calculate(const std::string_view& message) const = 0;
    virtual std::string calculate(const std::string& message){
        return calculate(std::string_view{message.cbegin(), message.cend()});
    }
};

class DummyDigester : public Digester
{
public:
    std::unique_ptr<Digester> clone() const override{ return std::make_unique<DummyDigester>();
    }
    std::string               calculate(const std::string_view& message) const override {
        return "0";
    }
};