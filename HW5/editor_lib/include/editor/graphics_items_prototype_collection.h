#pragma once

#include "primitive_items.h"

#include <unordered_map>
#include <stdexcept>
#include <string>

class GraphicsItemsPrototypeCollection
{
public:
    GraphicsItemsPrototypeCollection()
    {
        auto addPrototype = []<typename Type>(std::unordered_map<int, std::shared_ptr<GraphicsItem>>& collection) {
            auto item = std::make_shared<Type>();
            collection.emplace(item->uuid(), std::move(item));
        };

        addPrototype.template operator()<GraphicsLineItem>(collection_);
        addPrototype.template operator()<GraphicsRectItem>(collection_);
        addPrototype.template operator()<GraphicsEllipseItem>(collection_);
        addPrototype.template operator()<GraphicsTextItem>(collection_);
    }

    std::shared_ptr<GraphicsItem> create(int uuid) const
    {
        auto iter = collection_.find(uuid);
        if (iter == collection_.cend()) {
            throw std::invalid_argument("Cann't find prototype for UUID " + std::to_string(uuid));
        } else {
            return iter->second->clone();
        }
    }

private:
    std::unordered_map<int, std::shared_ptr<GraphicsItem>> collection_;
};