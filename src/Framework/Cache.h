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
class Cache {
public:
    template<typename... Args>
    Cache(Args&&... args) : algorithm(std::forward<Args>(args)...) // construct algorithm with args
    {}

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

    void selectArea(const PositionalCache::Bounds& boundingBox,
                std::function<void(EntityView<E>& handle)> consumer)
    {
        algorithm.selectArea(boundingBox, consumer);
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

}
