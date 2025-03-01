#include "Area2D.h"

#include <iostream>

Area2D::Area2D(Point2D lowerRight) : lowerRight(lowerRight), stopFlag(false), isTesting(false) {}

Area2D::~Area2D() 
{
    stopRandomMovements(); // Ensure thread is stopped before destruction
}

std::deque<EngineEntity> Area2D::getEntities()
{
    return entities;
}

void Area2D::addEntity(EngineEntity&& entity)
{
    entities.emplace_back(entity);
    entityCache.addEntity(&entities.back(), entity.getPosition(), entity.getId());

    // Register EntityCache as an observer for the newly added entity
    entities.back().addObserver(&entityCache);
}

void Area2D::clear()
{
    for (const EngineEntity& entity : entities) {
        //entityCache.removeEntity(entity); // Remove from cache
        entityCache.removeEntity(entity.getId());
    }

    entities.clear();
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

std::vector<EntityHandle<EngineEntity>> Area2D::squareSelection(PositionalCache::Bounds boundingBox)
{
    return entityCache.squareSelection(boundingBox);
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
        addEntity(EngineEntity(Point2D(randomX, randomY), nextId++));
    }
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
            for (EngineEntity& entity : entities)
            {
                if (moveChance(gen) < 0.1) // 10% chance to move
                {
                    Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
                    entity.updatePosition(newPosition);
                }
            }
        }

        // Sleep for a random time between 100ms and 1000ms
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration(gen)));
    }
}

void Area2D::shuffleEntityPositions()
{
    if (entities.empty()) return;

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
    std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

    // Shuffle all entity positions
    for (EngineEntity& entity : entities)
    {
        Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
        entity.updatePosition(newPosition);
    }
}
