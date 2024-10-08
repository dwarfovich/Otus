#pragma once

#include "logger.h"

#include <ostream>

class BulkerLogger : public Logger
{
public:
    BulkerLogger(std::ostream& stream) : stream_ { stream } {}

    void log(const std::string& message) override { stream_ << message << '\n'; }

private:
    std::ostream& stream_;
};