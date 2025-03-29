#include "VectorAlgorithm.h"

template<typename E>
void PositionalCache::VectorAlgorithm<E>::addEntity(std::unique_ptr<E> &&entity, const Point2D &position, int id)
{
    Error::ASSERT(entitiesMap.find(id) == entitiesMap.end(), "Entity has already been added.");
    // TODO rename these
    Entity<E> newHandle (std::move(entity), id);
    std::shared_ptr<Entity<E>> newEntity = std::make_shared<Entity<E>>(std::move(newHandle));
    std::pair<std::shared_ptr<Entity<E>>, Point2D> newPair (std::move(newEntity), position);

    entitiesMap.emplace(id, std::move(newPair));
    entitiesVector.emplace_back(std::move(newPair));
}

template<typename E>
void PositionalCache::VectorAlgorithm<E>::removeEntityById(int id)
{
    entitiesMap.erase(id);
    entitiesVector.erase(id);
}

template<typename E>
int PositionalCache::VectorAlgorithm<E>::getEntityCount()
{
    return entitiesVector.size();
}

template<typename E>
void PositionalCache::VectorAlgorithm<E>::selectArea(const PositionalCache::Bounds &boundingBox,
    std::function<void(EntityView<E> &handle)> consumer)
{
    for (auto& pair : entitiesVector)
    {
        Error::ASSERT(pair.first->hasEntity(), "Handle doesn't have an entity.");
        if (boundingBox.containsPosition(pair.second))
        {
            EntityView<E> safeView(pair.first);
            consumer(safeView);
        }
    }
}

template<typename E>
void PositionalCache::VectorAlgorithm<E>::getAllEntities(std::function<void(EntityView<E> &view)> consumer)
{
    for (auto& pair : entitiesVector) {
        auto copyOfShared = pair.first;
        EntityView<E> safeView(copyOfShared);
        consumer(safeView);
    }
}

template<typename E>
PositionalCache::Entity<E> & PositionalCache::VectorAlgorithm<E>::getEntityById(int id)
{
    return *entitiesMap.at(id).first.get();
}

template<typename E>
bool PositionalCache::VectorAlgorithm<E>::contains(int id)
{
    return entitiesMap.contains(id); // return entitiesMap.find(id) != entitiesMap.end();
}

template<typename E>
void PositionalCache::VectorAlgorithm<E>::clear()
{
    entitiesMap.clear();
    entitiesVector.clear();
}
