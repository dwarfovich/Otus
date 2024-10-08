#pragma once

#include "logger.h"

#include <memory>
#include <string>

class Loggable
{
public:
    Loggable(const std::shared_ptr<Logger>& logger = nullptr) : logger_{logger}{}

    
    protected: // methods
        void log(const std::string& message){
            logger_->log(message);
        }

    protected: // data
        std::shared_ptr<Logger> logger_;
};