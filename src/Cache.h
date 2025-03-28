#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <functional>

#include "CacheEntity.h"
#include "EntityHandle.h"
#include "Point2D.h"
#include "Bounds.h"
#include "Error.h"
#include "EntityView.h"

namespace PositionalCache
{
template <typename E>
class Observer;

template <typename E>
class EntityCache {
public:
    void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
    {
        Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");

        CacheEntity<E> newHandle (std::move(entity), id);
        std::shared_ptr<CacheEntity<E>> newEntity = std::make_shared<CacheEntity<E>>(std::move(newHandle));
        std::pair<std::shared_ptr<CacheEntity<E>>, Point2D> newPair (std::move(newEntity), position);

        entitiesMap.emplace(id, std::move(newPair));
    }

    void removeEntity(int id)
    {
        entitiesMap.erase(id);

    }

    int entityCount() {
        return entitiesMap.size();
    }

    void updateEntityPosition(int id, const Point2D& position)
    {
        Error::ASSERT(entitiesMap.find(id) != entitiesMap.end(), "Couldn't find entity.");

        auto res = entitiesMap.find(id);
        res->second.second = position;
    }

    void selectArea(const PositionalCache::Bounds& boundingBox,
                std::function<void(EntityView<E>& handle)> consumer)
    {
        for (auto& [entityId, pair] : entitiesMap)
        {
            Error::ASSERT(pair.first->hasEntity(), "Handle doesn't have an entity.");
            if (boundingBox.containsPosition(pair.second))
            {
                EntityView<E> safeView(pair.first);
                consumer(safeView);
            }
        }
    }

    Observer<E> createObserver() {
        return { this };
    }

    void onPositionChanged(int id, const Point2D& position)
    {
        updateEntityPosition(id, position);
    }

    void getAllEntities(std::function<void(EntityView<E>& view)> consumer) {
        for (auto& [entryId, pair] : entitiesMap) {

            auto copyOfShared = pair.first;
            EntityView<E> safeView(copyOfShared);
            consumer(safeView);

        }
    }

    CacheEntity<E>& getEntityById(int id) {
        return *entitiesMap.at(id).first.get();
    }

    bool isValidEntity(int id) {
        return entitiesMap.find(id) != entitiesMap.end();
    }

    void clear() {
        entitiesMap.clear();
    }

private:
    std::unordered_map <int, std::pair<std::shared_ptr<CacheEntity<E>>, Point2D>> entitiesMap{};

};

template <typename E>
class Observer {
    EntityCache<E>* cache{ nullptr };

public:
    Observer(EntityCache<E>* cache) : cache(cache) {}

    void onPositionChanged(int id, Point2D position) {

        Error::ASSERT(cache, "Invalid state.");

        cache->onPositionChanged(id, position);
    }

    bool isActive() {
        return cache;
    }
};
}
