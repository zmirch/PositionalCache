#include "World.h"

namespace FrameworkUser
{

World::World(Point2D lowerRight) : lowerRight(lowerRight), stopFlag(false), isTesting(false) {}

World::~World()
{
    stopRandomMovements(); // Ensure thread is stopped before destruction
}

void World::addEntity(std::unique_ptr<WorldEntity>&& entity)
{
    // Register EntityCache as an observer for the newly added entity
    entity->addObserver(&entityCache);
    entityCache.addEntity(std::move(entity), entity->getPosition(), entity->getId());
}

void World::clear()
{
    entityCache.clear();
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
        addEntity(std::unique_ptr<WorldEntity>(new WorldEntity(Point2D(randomX, randomY), nextId++)));
    }
}

void World::getAllEntities(std::function<void(EntityView<WorldEntity>& safeView)> consumer) {
    entityCache.getAllEntities(consumer);
}

void World::selectArea(PositionalCache::Bounds boundingBox, std::function<void(EntityView<WorldEntity>& safeView)> consumer)
{
    entityCache.selectArea(boundingBox, consumer);
}

bool World::isValidEntity(int id) {
    return entityCache.isValidEntity(id);
}

WorldEntity& World::getEntityById(int id) {
    return entityCache.getEntityById(id).getEntity();
}
void World::randomMovementLoop() // Simulate movement of entities
{
    // Random number generator setup
    std::random_device rd; // Seed generator
    std::mt19937 gen(rd()); // Random number generator
    std::uniform_real_distribution<> moveChance(0.0, 1.0);
    std::uniform_int_distribution<> sleepDuration(100, 1000);

    // Distributions for newPosition generation
    std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
    std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

    while (!stopFlag) {
        if (!isTesting)
        {
            entityCache.getAllEntities([&](EntityView<WorldEntity>& safeView) {
                WorldEntity& entity = safeView.getHandle()->getEntity();
                if (moveChance(gen) < 0.1) // 10% chance to move
                {
                    Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
                    entity.updatePosition(newPosition);
                }
            });
        }

        // Sleep for a random time between 100ms and 1000ms
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration(gen)));
    }
}

void World::shuffleEntityPositions()
{
    if (entityCache.entityCount() == 0) return;

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
    std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

    // Shuffle all entity positions
    entityCache.getAllEntities([&](EntityView<WorldEntity>& safeView) {
        WorldEntity& entity = safeView.getHandle()->getEntity();
        Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
        entity.updatePosition(newPosition);
    });
}

}