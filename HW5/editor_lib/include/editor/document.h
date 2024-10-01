#pragma once

#include "graphics_item.h"

#include <vector>
#include <unordered_set>
#include <functional>
#include <memory>

class Document
{
public:
    enum class ItemChangeType
    {
        Added,
        Removed,
        Modified
    };

    using DocumentObserver = std::function<void(ItemChangeType changeType, const GraphicsItem* item)>;

    virtual void addObserver(const std::function<void(ItemChangeType changeType, const GraphicsItem* item)>& observer)
    {
        observers.push_back(observer);
    }
    virtual void addItem(const std::shared_ptr<GraphicsItem>& item)
    {
        graphicsItems.insert(item);
        onGraphicsItemAdded(item.get());
    }
    virtual void notifyAboutItemChanged(const std::shared_ptr<GraphicsItem>& item)
    {
        onGraphicsItemRemoved(item.get());
    }
    virtual void removeItem(const std::shared_ptr<GraphicsItem>& item)
    {
        graphicsItems.erase(item);
        onGraphicsItemRemoved(item.get());
    }

protected:
    virtual void onGraphicsItemAdded(const GraphicsItem* item) { notifyAboutItemChanged(ItemChangeType::Added, item); }
    virtual void onGraphicsItemRemoved(const GraphicsItem* item)
    {
        notifyAboutItemChanged(ItemChangeType::Removed, item);
    }
    virtual void onGraphicsItemModified(const GraphicsItem* item)
    {
        notifyAboutItemChanged(ItemChangeType::Modified, item);
    }

    void notifyAboutItemChanged(ItemChangeType changeType, const GraphicsItem* item)
    {
        for (const auto& observer : observers) {
            observer(changeType, item);
        }
    }

protected:
    std::vector<DocumentObserver>                     observers;
    std::unordered_set<std::shared_ptr<GraphicsItem>> graphicsItems;
};