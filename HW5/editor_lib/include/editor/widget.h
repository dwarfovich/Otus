#pragma once

class Widget
{
public:
    int width() const noexcept { return width_; }
    int height() const noexcept { return height_;}

    private:
        int width_ = 0;
        int height_ = 0;

};