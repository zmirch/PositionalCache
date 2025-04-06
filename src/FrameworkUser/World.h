#pragma once

#include <deque>
#include <thread>
#include <random>
#include <functional>

#include "WorldEntity.h"
#include "../Framework/Point2D.h"
#include "../Framework/Cache.h"
#include "../Framework/Bounds.h"
#include "../Framework/Algorithms/BasicAlgorithm.h"
#include "../Framework/Algorithms/VectorAlgorithm.h"

using namespace PositionalCache;

namespace FrameworkUser
{

template <typename Algorithm>
class World
{
public:
    World(Point2D lowerRight)
        : lowerRight(lowerRight), stopFlag(false), isTesting(false)
    {
    }

    World(const World& other)
        : lowerRight(other.lowerRight),
          entityCache(other.entityCache),
          nextId(other.nextId),
          stopFlag(other.stopFlag),
          isTesting(other.isTesting)
    {
    }

    World(World&& other) noexcept
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

    World& operator=(const World& other)
    {
        if (this != &other) {
            stopRandomMovements();
            lowerRight = other.lowerRight;
            entityCache = other.entityCache;
            nextId = other.nextId;
            stopFlag = other.stopFlag;
            isTesting = other.isTesting;
        }
        return *this;
    }

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

    ~World()
    {
        stopRandomMovements();
    }

    void addEntity(std::unique_ptr<WorldEntity>&& entity, const Point2D& position)
    {
        entityCache.addEntity(std::move(entity), position, entity->getId());
    }

    void clear()
    {
        entityCache.clear();
    }

    void startRandomMovements()
    {
        stopFlag = false;
        movementThread = std::thread(&World::randomMovementLoop, this);
    }

    void stopRandomMovements()
    {
        stopFlag = true;
        if (movementThread.joinable()) {
            movementThread.join();
        }
    }

    double getWidth()
    {
        return lowerRight.getX();
    }

    double getHeight()
    {
        return lowerRight.getY();
    }

    void setTestingStatus(bool status)
    {
        isTesting = status;
    }

    void addNEntities(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            int randomX = std::rand() % (int)getWidth();
            int randomY = std::rand() % (int)getHeight();
            addEntity(std::unique_ptr<WorldEntity>(new WorldEntity(nextId++)), Point2D(randomX, randomY));
        }
    }

    void getAllEntities(std::function<void(EntityView<WorldEntity>& safeView)> consumer)
    {
        entityCache.getAllEntities(consumer);
    }

    void selectArea(PositionalCache::Bounds boundingBox, std::function<void(EntityView<WorldEntity>& safeView)> consumer)
    {
        entityCache.selectArea(boundingBox, consumer);
    }

    bool isValidEntity(int id)
    {
        return entityCache.isValidEntity(id);
    }

    WorldEntity& getEntityById(int id)
    {
        return entityCache.getEntityById(id).getEntity();
    }

    void shuffleEntityPositions()
    {
        if (entityCache.entityCount() == 0) return;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
        std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

        entityCache.getAllEntities([&](EntityView<WorldEntity>& safeView) {
            Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
            safeView.getEntity().updatePosition(newPosition);
        });
    }

    bool isTesting;

private:
    Cache<WorldEntity, Algorithm> entityCache;
    int nextId = 0;
    void randomMovementLoop()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> moveChance(0.0, 1.0);
        std::uniform_int_distribution<> sleepDuration(100, 1000);
        std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
        std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);

        while (!stopFlag) {
            if (!isTesting)
            {
                entityCache.getAllEntities([&](EntityView<WorldEntity>& safeView) {
                    if (moveChance(gen) < 0.1) // 10% chance to move
                    {
                        Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
                        safeView.getEntity().updatePosition(newPosition);
                    }
                });
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration(gen)));
        }
    }

    Point2D lowerRight;
    std::thread movementThread;
    bool stopFlag;
};

} // namespace FrameworkUser