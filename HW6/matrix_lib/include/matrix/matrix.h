#pragma once

#include <cstddef>
#include <unordered_map>
#include <array>

template<typename ElementType, std::size_t Dimension = 2>
class Matrix
{
public:
    class ElementProxyRef
    {
    public:
        ElementProxyRef(Matrix<ElementType, Dimension>& matrix, ElementType* element = nullptr)
            : matrix_ { matrix }, element_ { element }
        {
        }

    private:
        ElementType*                    element_ = nullptr;
        Matrix<ElementType, Dimension>& matrix_;
    };

    using Position = std::array<std::size_t, Dimension>;

    Matrix(const ElementType& defaultElement = {}) : defaultElement_ { defaultElement } {}
    
    const ElementType& defaultElement() const noexcept{ return defaultElement_;}
    ElementProxyRef refAt(const Position& position) const noexcept;
    void               set(const Position& position, const ElementType& element){
        elements_[position] = element;
    }
    void               set(const Position& position, ElementType&& element) { elements_[position] = element; }

private:
    std::unordered_map<Position, ElementType> elements_;
    ElementType                        defaultElement_;
};

template<typename ElementType, std::size_t Dimension>
auto Matrix<ElementType, Dimension>::refAt(const Position& position) const noexcept -> ElementProxyRef
{
    ElementProxyRef element { *this };
    auto            iter = elements_.find(position);
    if (iter == elements_.cend()) {
        ElementProxyRef element { *this, &defaultElement_ };
    } else {
        ElementProxyRef element { *this, &iter->second };
    }
}
