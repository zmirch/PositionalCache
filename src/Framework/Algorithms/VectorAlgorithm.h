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
    class VectorAlgorithm {
    public:
        void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
        {
            Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");
            // TODO rename these
            Entity<E> newHandle (std::move(entity), id, position);
            std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
            // entitiesMap.emplace(id, std::move(newEntity));
            entitiesMap.emplace(id, newEntity);
            entitiesVector.emplace_back(newEntity);
        }
        void removeEntityById(int id)
        {
            entitiesMap.erase(id);
            entitiesVector.erase(id);
        }
        int getEntityCount()
        {
            return entitiesVector.size();
        }
        void selectArea(const PositionalCache::Bounds& boundingBox,
                    std::function<void(EntityView<E>& handle)> consumer)
        {
            for (auto& entity : entitiesVector)
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
            for (auto& entity : entitiesVector) {
                auto copyOfShared = entity;
                EntityView<E> safeView(copyOfShared);
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
            entitiesVector.clear();
        }
    private:
        std::unordered_map <int, std::shared_ptr<PositionalCache::Entity<E>>> entitiesMap{};
        std::vector<std::shared_ptr<Entity<E>>> entitiesVector{};
    };
}