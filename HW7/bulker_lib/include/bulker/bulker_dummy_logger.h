#pragma once

#include "bulker_logger.h"

class BulkerDummyLogger : public BulkerLogger
{
public:
    void log(const std::string& message) override {}
    void setLogFile(const std::string& filePath) override {}
};