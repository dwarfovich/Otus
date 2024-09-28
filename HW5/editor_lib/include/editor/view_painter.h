#pragma once

#include "painter.h"
#include "rect.h"

#include <iostream>

class ViewPainter : public Painter<>
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

protected:
    void drawUtf8Char(const std::string& utf8Char, const Rect& charRect, const Font& font)
    {
        std::cout << "ViewPainter is drawing UTF-8 char: " << utf8Char << '\n';
    }
    
    const Rect& utf8CharRect(const std::string& utf8Char,
                             const Point&       textTopLeft,
                             const Point&       textBottomRight,
                             const Font&        font) const
    {
        return tempUtf8CharRect;
    }


};