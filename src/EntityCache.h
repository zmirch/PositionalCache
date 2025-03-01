#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <functional>

#include "EntityHandle.h"
#include "Point2D.h"
#include "Bounds.h"
#include "Error.h"

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

            EntityHandle<E> newHandle (std::move(entity), id);
            std::pair<EntityHandle<E>, Point2D> newPair (std::move(newHandle), position);

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

        std::vector<EntityHandle<E>> squareSelection(const PositionalCache::Bounds& boundingBox)
        {
            std::vector<EntityHandle<E>> selectedEntities{};
            for (auto& [entryId, pair] : entitiesMap)
            {
                EntityHandle<E>& entityHandle = pair.first;
                Point2D& entityPosition = pair.second;

                Error::ASSERT(entityHandle.hasEntity(), "Handle doesn't have an entity.");
                if (boundingBox.containsPosition(entityPosition))
                {
                    selectedEntities.push_back(entityHandle);
                }
            }

            //std::cout << "Selected " << selectedEntities.size() << " entities.\n";
            return selectedEntities;
        }
        void selectArea(PositionalCache::Bounds boundingBox, std::function<void(const EntityHandle<E>& handle)> consumer)
        {
            for (auto& [entityId, pair] : entitiesMap)
            {
                EntityHandle<E>& entityHandle = pair.first;
                Point2D& entityPosition = pair.second;

                Error::ASSERT(entityHandle.hasEntity(), "Handle doesn't have an entity.");

                if (boundingBox.containsPosition(entityPosition))
                    consumer(entityHandle);
            }
        }

        Observer<E> createObserver() {
            return { this };
        }

        void onPositionChanged(int id, const Point2D& position)
        {
            updateEntityPosition(id, position);
        }

        void getAllEntities(std::function<void(EntityHandle<E>& handle)> consumer) {
            for (auto& [entryId, pair] : entitiesMap) {
                EntityHandle<E>& entityHandle = pair.first;
                consumer(entityHandle);
            }
        }

        EntityHandle<E>& getEntityById(int id) {
            return entitiesMap.at(id).first;
        }

    private:
        std::unordered_map <int, std::pair<EntityHandle<E>, Point2D>> entitiesMap{};
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