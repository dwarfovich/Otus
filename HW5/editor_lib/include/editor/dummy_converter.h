#pragma once

#include <string>

class DummyConverter{
public:
    static const std::string toUtf8(const std::string& text){
        return text;
    }
};