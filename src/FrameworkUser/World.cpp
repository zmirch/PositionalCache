#include "World.h"

namespace FrameworkUser
{
using BasicCache = Cache<WorldEntity, BasicAlgorithm<WorldEntity>>;
using VectorCache = Cache<WorldEntity, VectorAlgorithm<WorldEntity>>;
using CacheVariant = std::variant<BasicCache, VectorCache>;

void World::setCacheType(CacheType type)
{
    stopRandomMovements(); // Ensure thread is stopped before changing cache
    switch (type)
    {
        case CacheType::Vector:
            entityCache = VectorCache();
            break;
        case CacheType::Basic:
            entityCache = BasicCache();
            break;
    }
}

CacheType World::getCurrentCacheType() const
{
    if (std::holds_alternative<VectorCache>(entityCache))
        return CacheType::Vector;
    return CacheType::Basic;
}

World::World(Point2D lowerRight)
    : lowerRight(lowerRight), stopFlag(false), isTesting(false)
{
}

World::World(const World& other)
    : lowerRight(other.lowerRight),
      entityCache(other.entityCache),
      nextId(other.nextId),
      stopFlag(other.stopFlag),
      isTesting(other.isTesting)
{
}

World::World(World&& other) noexcept
    : lowerRight(std::move(other.lowerRight)),
      entityCache(std::move(other.entityCache)),
      nextId(other.nextId),
      stopFlag(other.stopFlag),
      isTesting(other.isTesting),
      movementThread(std::move(other.movementThread))
{
    other.nextId = 0;
    other.stopFlag = true;
}

World::~World()
{
    stopRandomMovements();
}

void World::addEntity(std::unique_ptr<WorldEntity>&& entity, const Point2D& position)
{
    withEntityCache([&](auto& cache)
    {
        cache.addEntity(std::move(entity), position, entity->getId());
    });
}

void World::clear()
{
    withEntityCache([&](auto& cache)
    {
        cache.clear();
    });
}

void World::startRandomMovements()
{
    stopFlag = false;
    movementThread = std::thread(&World::randomMovementLoop, this);
}

void World::stopRandomMovements()
{
    stopFlag = true;
    if (movementThread.joinable()) {
        movementThread.join();
    }
}

double World::getWidth()
{
    return lowerRight.getX();
}

double World::getHeight()
{
    return lowerRight.getY();
}

void World::setTestingStatus(bool status)
{
    isTesting = status;
}

void World::addNEntities(int n)
{
    for (int i = 0; i < n; ++i)
    {
        int randomX = std::rand() % (int)getWidth();
        int randomY = std::rand() % (int)getHeight();
        addEntity(std::unique_ptr<WorldEntity>(new WorldEntity(nextId++)), Point2D(randomX, randomY));
    }
}

void World::getAllEntities(std::function<void(EntityView<WorldEntity>& safeView)> consumer)
{
    withEntityCache([&](auto& cache)
    {
        cache.getAllEntities(consumer);
    });
}

void World::selectArea(Bounds boundingBox, std::function<void(EntityView<WorldEntity>& safeView)> consumer)
{
    withEntityCache([&](auto& cache)
    {
        cache.selectArea(boundingBox, consumer);
    });
}

bool World::isValidEntity(int id)
{
    return withEntityCache([&](auto& cache)
    {
        return cache.isValidEntity(id);
    });
}

WorldEntity& World::getEntityById(int id)
{
    return withEntityCache([&](auto& cache) -> WorldEntity&
    {
        return cache.getEntityById(id).getEntity();
    });
}

void World::shuffleEntityPositions()
{
    if (withEntityCache([&](auto& cache) { return cache.entityCount(); }) == 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
    std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

    withEntityCache([&](auto& cache)
    {
        cache.getAllEntities([&](EntityView<WorldEntity>& safeView)
        {
            Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
            safeView.getEntity().updatePosition(newPosition);
        });
    });
}

void World::randomMovementLoop()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> moveChance(0.0, 1.0);
    std::uniform_int_distribution<> sleepDuration(100, 1000);
    std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
    std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

    while (!stopFlag)
    {
        if (!isTesting)
        {
            withEntityCache([&](auto& cache)
            {
                cache.getAllEntities([&](EntityView<WorldEntity>& safeView)
                {
                    if (moveChance(gen) < 0.1) // 10% chance to move
                    {
                        Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
                        safeView.getEntity().updatePosition(newPosition);
                    }
                });
            });
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration(gen)));
    }
}

} // namespace FrameworkUser