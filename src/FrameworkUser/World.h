#pragma once

#include <deque>
#include <thread>
#include <random>
#include <functional>
#include <variant>

#include "WorldEntity.h"
#include "../Framework/Point2D.h"
#include "../Framework/Cache.h"
#include "../Framework/Bounds.h"
#include "../Framework/Algorithms/BasicAlgorithm.h"
#include "../Framework/Algorithms/DequeAlgorithm.h"
#include "../Framework/Algorithms/QuadtreeAlgorithm.h"
#include "../Framework/Algorithms/GridAlgorithm.h"

using namespace PositionalCache;

namespace FrameworkUser
{

using BasicCache = Cache<WorldEntity, BasicAlgorithm<WorldEntity>>;
using DequeCache = Cache<WorldEntity, DequeAlgorithm<WorldEntity>>;
using QuadtreeCache = Cache<WorldEntity, QuadtreeAlgorithm<WorldEntity>>;
using GridCache = Cache<WorldEntity, GridAlgorithm<WorldEntity>>;
using CacheVariant = std::variant<BasicCache, DequeCache, QuadtreeCache, GridCache>;

enum class CacheType { Deque, Basic, Quadtree, Grid };

class World
{
public:
    //Invokes the provided callable with the currently active Cache instance held by the variant.
    template <typename Func>
    decltype(auto) withEntityCache(Func&& f)
    {
        return std::visit(std::forward<Func>(f), entityCache);
    }

    void setCacheType(CacheType type);

    CacheType getCurrentCacheType() const;

    World(Point2D lowerRight);

    World(const World& other);

    World(World&& other) noexcept;

    World& operator=(World&& other) noexcept
    {
        if (this != &other) {
            stopRandomMovements();
            lowerRight = std::move(other.lowerRight);
            entityCache = std::move(other.entityCache);
            nextId = other.nextId;
            stopFlag = other.stopFlag;
            isTesting = other.isTesting;
            movementThread = std::move(other.movementThread);
            other.nextId = 0;
            other.stopFlag = true;
        }
        return *this;
    }

    ~World();

    void addEntity(std::unique_ptr<WorldEntity>&& entity, const Point2D& position);

    void clear();

    void startRandomMovements();

    void stopRandomMovements();

    double getWidth();

    double getHeight();

    void setTestingStatus(bool status);

    void addNEntities(int n);

    void addNEntitiesAlongLine(int n);

    void addNEntitiesCluster(int n);

    void getAllEntities(std::function<void(EntityView<WorldEntity>& safeView)> consumer);

    void selectArea(PositionalCache::Bounds boundingBox, std::function<void(EntityView<WorldEntity>& safeView)> consumer);

    bool isValidEntity(int id);

    WorldEntity& getEntityById(int id);

    void shuffleEntityPositions();

    bool isTesting;

    void forEachNodeBounds(std::function<void(const Bounds&)> consumer);

private:
    CacheVariant entityCache;
    int nextId = 0;
    void randomMovementLoop();

    Point2D lowerRight;
    std::thread movementThread;
    bool stopFlag;
};

} // namespace FrameworkUser