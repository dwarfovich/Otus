#pragma once

#include "painter.h"

template<template<typename> class CharConverter, class CharType>
class DefaultPainter : public Painter
{
protected:
    template<typename CharType>
    const std::string& toUtf8(const CharType& inputChar)
    {
    }
    virtual const Rect& utf8CharRect(const std::string& utf8Char,
                                     const Point&       textTopLeft,
                                     const Point&       textBottomRight,
                                     const Font&        font)
    {
        return tempUtf8CharRect;
    }


private:
    CharConverter<CharType> charConverter;
    std::string tempUtf8Char;
    Rect tempUtf8CharRect;
};