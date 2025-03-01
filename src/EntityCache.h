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
        void addEntity(E* entity, const Point2D& position, int id)
        {
            Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");

            EntityHandle newHandle{ entity, id };
            std::pair<EntityHandle<E>, Point2D> newPair{ newHandle, position };

            entitiesMap.insert({ id, newPair });
        }

        void removeEntity(int id)
        {
            entitiesMap.erase(id);
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
            for (auto& [entryId, pair] : entitiesMap)
            {
                EntityHandle<E>& entryHandle = pair.first;
                Point2D& entryPosition = pair.second;

                Error::ASSERT(entryHandle.hasEntity(), "Handle doesn't have an entity.");

                if (boundingBox.containsPosition(entryPosition))
                    consumer(entryHandle);
            }
        }

        Observer<E> createObserver() {
            return { this };
        }

        void onPositionChanged(int id, const Point2D& position)
        {
            updateEntityPosition(id, position);
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