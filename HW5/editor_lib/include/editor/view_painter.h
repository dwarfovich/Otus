#pragma once

#include "painter.h"
#include "rect.h"

#include <iostream>

class ViewPainter : public Painter
{
    void setForegroundColor(const Color& color) { std::cout << "ViewPainter is setting foreground color\n"; }
    void setBackgroundColor(const Color& color) { std::cout << "ViewPainter is setting background color\n"; }

    void drawPoint(const Point& point) { std::cout << "ViewPainter is drawing point\n"; }
    void drawLine(const Point& first, const Point& last) { std::cout << "ViewPainter is drawing line\n"; };
    void drawRect(const Point& topLeft, const Point& bottomRight) { std::cout << "ViewPainter is drawing rect\n"; }
    void drawEllipse(const Point& topLeft, const Point& bottomRight)
    {
        std::cout << "ViewPainter is drawing ellipse\n";
    }
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
    void drawUtf8Char(const std::string& utf8Char, const Rect& charRect, const Font& font)
    {
        std::cout << "ViewPainter is drawing UTF-8 char: " << utf8Char << '\n';
    }
    template<typename CharType>
    const std::string& toUtf8(const CharType& inputChar) const
    {
        return tempUtf8Char;
    }
    const Rect& utf8CharRect(const std::string& utf8Char,
                             const Point&       textTopLeft,
                             const Point&       textBottomRight,
                             const Font&        font) const
    {
        return tempUtf8CharRect;
    }

private:
    mutable std::string tempUtf8Char;
    mutable Rect        tempUtf8CharRect;
};