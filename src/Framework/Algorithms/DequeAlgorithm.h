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
    class DequeAlgorithm {
    public:
        void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id)
        {
            Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");
            Entity<E> newHandle (std::move(entity), id, position, [](Entity<E>& entity, const Point2D& position){});
            std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
            entitiesMap.emplace(id, newEntity);
            entitiesDeque.emplace_back(newEntity);
        }
        void removeEntityById(int id)
        {
            entitiesMap.erase(id);
            auto it = std::remove_if(entitiesDeque.begin(), entitiesDeque.end(),
            [id](const std::shared_ptr<Entity<E>>& e) {
                return e->getId() == id;
            });
            entitiesDeque.erase(it, entitiesDeque.end());
        }
        int getEntityCount()
        {
            return entitiesDeque.size();
        }
        void selectArea(const PositionalCache::Bounds& boundingBox,
                    std::function<void(EntityView<E>& handle)> consumer)
        {
            for (auto& entity : entitiesDeque)
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
            for (auto& entity : entitiesDeque)
            {
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
            entitiesDeque.clear();
        }
    private:
        std::unordered_map <int, std::shared_ptr<Entity<E>>> entitiesMap{};
        std::deque<std::shared_ptr<Entity<E>>> entitiesDeque{};
    };
}