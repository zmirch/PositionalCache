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
    void addEntity(std::unique_ptr<E>&& entity, const Point2D& position, int id);
    void removeEntityById(int id);
    int getEntityCount();
    void selectArea(const PositionalCache::Bounds& boundingBox,
                std::function<void(EntityView<E>& handle)> consumer);
    void getAllEntities(std::function<void(EntityView<E>& view)> consumer);
    Entity<E>& getEntityById(int id);
    bool contains(int id);
    void clear();
private:
    std::unordered_map <int, std::pair<std::shared_ptr<PositionalCache::Entity<E>>, Point2D>> entitiesMap{};
    std::vector<std::pair<std::shared_ptr<PositionalCache::Entity<E>>, Point2D>> entitiesVector{};
};
}