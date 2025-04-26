#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include "../Entity.h"
#include "../Point2D.h"
#include "../EntityView.h"
#include "../Bounds.h"
#include "../Error.h"

namespace PositionalCache
{

template <typename E>
class BasicAlgorithm {
public:
    void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
    {
        Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");
        // TODO rename these
        Entity<E> newHandle (std::move(entity), id, position);
        std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
        entitiesMap.emplace(id, std::move(newEntity));
    }
    void removeEntityById(int id)
    {
        entitiesMap.erase(id);
    }
    int getEntityCount()
    {
        return entitiesMap.size();
    }
    void selectArea(const PositionalCache::Bounds& boundingBox,
                std::function<void(EntityView<E>& handle)> consumer)
    {
        for (auto& [entityId, entity] : entitiesMap)
        {
            Error::ASSERT(entity->hasEntity(), "Handle doesn't have an entity.");
            if (boundingBox.containsPosition(entity->getPosition()))
            {
                EntityView<E> safeView(entity);
                consumer(safeView);
            }
        }
    }
    void getAllEntities(std::function<void(EntityView<E>& view)> consumer)
    {
        for (auto& [entityId, entity] : entitiesMap) {
            EntityView<E> safeView(entity);
            consumer(safeView);
        }
    }
    Entity<E>& getEntityById(int id)
    {
        return *entitiesMap.at(id).get();
    }
    bool contains(int id)
    {
        return entitiesMap.find(id) != entitiesMap.end();
    }
    void clear()
    {
        entitiesMap.clear();
    }
private:
    std::unordered_map <int, std::shared_ptr<Entity<E>>> entitiesMap{};
};
}