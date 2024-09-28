#pragma once

#include "painter.h"
#include "rect.h"

#include <iostream>

class View;

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

    void drawEllipse(const Point& topLeft, const Point& bottomRight) = 0;
    void drawText(const std::string& text, const Point& topLeft, const Point& bottomRight, const Font& font = {})
    {
        drawUtf8Text(text, topLeft, bottomRight, font);
    }

protected: // methods
    void drawUtf8Text(const std::string& text,
                              const Point&      topLeft,
                              const Point&      bottomRight,
                              const Font&       font) {
        std::cout << "ViewPainter is drawing text: " + text + '\n';
    }
};