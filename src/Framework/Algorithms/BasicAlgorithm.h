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
        Entity<E> newHandle (std::move(entity), id);
        std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
        std::pair<std::shared_ptr<Entity<E>>, Point2D> newPair (std::move(newEntity), position);

        entitiesMap.emplace(id, std::move(newPair));
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
    void getAllEntities(std::function<void(EntityView<E>& view)> consumer)
    {
        for (auto& [entryId, pair] : entitiesMap) {
            auto copyOfShared = pair.first;
            EntityView<E> safeView(copyOfShared);
            consumer(safeView);
        }
    }
    Entity<E>& getEntityById(int id)
    {
        return *entitiesMap.at(id).first.get();
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
    std::unordered_map <int, std::pair<std::shared_ptr<PositionalCache::Entity<E>>, Point2D>> entitiesMap{};
};
}