#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <functional>

#include "Entity.h"
#include "EntityHandle.h"
#include "Point2D.h"
#include "Bounds.h"
#include "Error.h"
#include "EntityView.h"

namespace PositionalCache
{
template <typename E, typename Algorithm>
class Observer;

template <typename E, typename Algorithm>
class Cache {
public:
    void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
    {
        algorithm.addEntity(std::move(entity), position, id);
    }

    void removeEntity(int id)
    {
        algorithm.removeEntity(id);
    }

    int entityCount()
    {
        return algorithm.getEntityCount();
    }

    void updateEntityPosition(int id, const Point2D& position)
    {
        //Error::ASSERT(entitiesMap.find(id) != entitiesMap.end(), "Couldn't find entity.");
        Error::ASSERT(algorithm.contains(id), "Couldn't find entity.");
        Entity<E>& res = algorithm.getEntityById(id);
        res.updatePosition(position);
    }

    void selectArea(const PositionalCache::Bounds& boundingBox,
                std::function<void(EntityView<E>& handle)> consumer)
    {
        algorithm.selectArea(boundingBox, consumer);
    }

    Observer<E, Algorithm> createObserver() {
        return { this };
    }

    void onPositionChanged(int id, const Point2D& position)
    {
        updateEntityPosition(id, position);
    }

    void getAllEntities(std::function<void(EntityView<E>& view)> consumer) {
        algorithm.getAllEntities(consumer);
    }

    Entity<E>& getEntityById(int id)
    {
        return algorithm.getEntityById(id);
    }

    bool isValidEntity(int id)
    {
        return algorithm.contains(id);
    }

    void clear()
    {
        algorithm.clear();
    }

private:
    Algorithm algorithm;

};

template <typename E, typename Algorithm>
class Observer {
    Cache<E, Algorithm>* cache{ nullptr };

public:
    Observer(Cache<E, Algorithm>* cache) : cache(cache) {}

    void onPositionChanged(int id, Point2D position) {
        Error::ASSERT(cache, "Invalid state.");
        cache->onPositionChanged(id, position);
    }

    bool isActive() {
        return cache;
    }
};
}
