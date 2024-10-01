#pragma once

#include <vector>

class Widget
{
public:
    int width() const noexcept { return width_; }
    int height() const noexcept { return height_;}
    void addChild(const std::shared_ptr<Widget>& child) {
        children_.push_back(child);
    }

    private:
        int width_ = 0;
        int height_ = 0;
        std::vector<std::shared_ptr<Widget>> children_;
};