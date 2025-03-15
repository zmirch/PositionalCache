#include "Area2D.h"

#include <iostream>

Area2D::Area2D(Point2D lowerRight) : lowerRight(lowerRight), stopFlag(false), isTesting(false) {}

Area2D::~Area2D() 
{
    stopRandomMovements(); // Ensure thread is stopped before destruction
}


void Area2D::addEntity(std::unique_ptr<EngineEntity>&& entity)
{
    // Register EntityCache as an observer for the newly added entity
    entity->addObserver(&entityCache);
    entityCache.addEntity(std::move(entity), entity->getPosition(), entity->getId());
}

void Area2D::clear()
{
    entityCache.clear();
}

void Area2D::startRandomMovements()
{
    stopFlag = false;
    movementThread = std::thread(&Area2D::randomMovementLoop, this);
}

void Area2D::stopRandomMovements()
{
    stopFlag = true;
    if (movementThread.joinable()) {
        movementThread.join();
    }
}

double Area2D::getWidth()
{
    return lowerRight.getX();
}

double Area2D::getHeight()
{
    return lowerRight.getY();
}

void Area2D::setTestingStatus(bool status)
{
    isTesting = status;
}

void Area2D::addNEntities(int n)
{
    for (int i = 0; i < n; ++i)
    {
        int randomX = std::rand() % (int)getWidth();
        int randomY = std::rand() % (int)getHeight();
        addEntity(std::unique_ptr<EngineEntity>(new EngineEntity(Point2D(randomX, randomY), nextId++)));
    }
}

void Area2D::getAllEntities(std::function<void(CacheEntity<EngineEntity>& handle)> consumer) {
    entityCache.getAllEntities(consumer);
}

void Area2D::selectArea(PositionalCache::Bounds boundingBox, std::function<void(std::shared_ptr<CacheEntity<EngineEntity>>& handle)> consumer)
{
    entityCache.selectArea(boundingBox, consumer);
}

bool Area2D::isValidEntity(int id) {
    return entityCache.isValidEntity(id);
    //return entityCache.getEntityById(id).hasEntity();
}

EngineEntity& Area2D::getEntityById(int id) {
    return entityCache.getEntityById(id).getEntity();
}
void Area2D::randomMovementLoop() // Simulate movement of entities
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
            entityCache.getAllEntities([&](CacheEntity<EngineEntity>& handle) {
                EngineEntity& entity = handle.getEntity();
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

void Area2D::shuffleEntityPositions()
{
    if (entityCache.entityCount() == 0) return;

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
    std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

    // Shuffle all entity positions
    entityCache.getAllEntities([&](CacheEntity<EngineEntity>& handle) {
        EngineEntity& entity = handle.getEntity();
        Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
        entity.updatePosition(newPosition);
    });
}
