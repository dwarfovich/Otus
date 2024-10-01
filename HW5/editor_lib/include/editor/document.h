#pragma once

#include "graphics_item.h"
#include "serializable.h"
#include "graphics_items_prototype_collection.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <functional>
#include <memory>

class Document : public Serializeble
{
    using ItemsCollection = std::unordered_set<std::shared_ptr<GraphicsItem>>;

public:
    enum class ItemChangeType
    {
        Added,
        Removed,
        Modified
    };

    using DocumentObserver   = std::function<void(ItemChangeType changeType, const GraphicsItem* item)>;
    using ItemsConstIterator = ItemsCollection::const_iterator;

    void read(std::istream& stream) override
    {
        std::cout << "Reading document\n";
        GraphicsItemsPrototypeCollection prototypes;
        int                              uuid = 0;
        stream >> uuid;
        if(stream.eof()){
            return;
        }
        try {
            auto item = prototypes.create(uuid);
            item->read(stream);
            addItem(std::move(item));
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << '\n';
        }
    }
    void write(std::ostream& stream) const override {
        std::cout << "Writing document\n";
        for (const auto& item : graphicsItems){
            stream << item->uuid();
            item->write(stream);
        }
    }

    ItemsConstIterator firstItem() const { return graphicsItems.cbegin(); }
    ItemsConstIterator endItem() const { return graphicsItems.cend(); }

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
        auto t= graphicsItems.erase(item);
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