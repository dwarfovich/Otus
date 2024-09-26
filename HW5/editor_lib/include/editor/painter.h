#pragma once

#include <string>

class Color;
class Point;
class Font;
class Rect;

class Painter
{
public:
    virtual void setForegroundColor(const Color& color) = 0;
    virtual void setBackgroundColor(const Color& color) = 0;

    virtual void drawPoint(const Point& point)                               = 0;
    virtual void drawLine(const Point& first, const Point& last)             = 0;
    virtual void drawRect(const Point& topLeft, const Point& bottomRight)    = 0;
    virtual void drawEllipse(const Point& topLeft, const Point& bottomRight) = 0;
    template<typename StringType>
    void drawText(const StringType& text, const Point& topLeft, const Point& bottomRight, const Font& font = {})
    {
        for (const auto& textChar : text) {
            const auto& utf8Char = toUtf8(textChar);
            const auto& charRect = utf8CharRect(utf8Char);
            drawUtf8Char(utf8Char, charRect, font);
        }
    }

protected:
    virtual void drawUtf8Char(const std::string& utf8Char, const Rect& charRect, const Font& font) {} = 0;
    template<typename CharType>
    const std::string& toUtf8(const CharType& inputChar)
    {
    }
    virtual const Rect& utf8CharRect(const std::string& utf8Char,
                             const Point&       textTopLeft,
                             const Point&       textBottomRight,
                             const Font&        font) = 0;
};