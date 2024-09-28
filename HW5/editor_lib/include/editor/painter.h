#pragma once

#include "dummy_converter.h"

#include <string>

class Color;
class Point;
class Font;
class Rect;

template<typename StringType = std::string, class StringConverter = DummyConverter>
class Painter
{
public:
    virtual void setForegroundColor(const Color& color) = 0;
    virtual void setBackgroundColor(const Color& color) = 0;

    virtual void drawPoint(const Point& point)                               = 0;
    virtual void drawLine(const Point& first, const Point& last)             = 0;
    virtual void drawRect(const Point& topLeft, const Point& bottomRight)    = 0;
    virtual void drawEllipse(const Point& topLeft, const Point& bottomRight) = 0;
    virtual void drawText(const StringType& text, const Point& topLeft, const Point& bottomRight, const Font& font = {})
    {
        const auto& utf8Text = StringConverter::toUtf8(text);
        drawUtf8Text(utf8Text, topLeft, bottomRight, font);
    }

protected: // methods
    virtual void drawUtf8Text(const std::string& text,
                              const Point&      topLeft,
                              const Point&      bottomRight,
                              const Font&       font) = 0;

protected: // possible inner compononents
           // PaintDevice& paintDevice; // A class representing a paintable canvas - a screen, a printer, etc.
};