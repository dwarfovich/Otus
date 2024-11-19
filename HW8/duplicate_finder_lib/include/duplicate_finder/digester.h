#pragma once

#include <string>
#include <memory>

class Digester
{
public:
    virtual ~Digester() = default;

    virtual const std::string&        name() const { return name_; }
    virtual std::string               calculate(const std::string_view& message) const = 0;
    virtual std::string               calculate(const std::string& message)
    {
        return calculate(std::string_view { message.cbegin(), message.cend() });
    }

private:
    inline static const std::string name_ = "Unknown";
};

class DummyDigester : public Digester
{
public:
    std::string               calculate(const std::string_view& message) const override
    {
        return { message.cbegin(), message.cend() };
    }
    const std::string& name() const override
    {
        static const std::string name_ = "Dummy";

        return name_;
    }
    };