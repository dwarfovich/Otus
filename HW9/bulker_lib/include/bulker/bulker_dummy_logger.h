#pragma once

#include "bulker_logger.h"
#include "dummy_stream.h"

class BulkerDummyLogger : public BulkerLogger
{
public:
    BulkerDummyLogger() : BulkerLogger { stream_, std::ref(stream_) } {}

    void log(const std::string&) override {}
    void setLogFile(const std::string&) override {}

private:
    DummyStream stream_;
};