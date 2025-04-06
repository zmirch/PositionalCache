// #include "World.h"
//
// namespace FrameworkUser
// {
// template <typename Algorithm>
// World<Algorithm>::World(Point2D lowerRight) : lowerRight(lowerRight), stopFlag(false), isTesting(false) {}
// // Copy constructor
// template <typename Algorithm>
// World<Algorithm>::World(const World& other)
//     : lowerRight(other.lowerRight),
//       entityCache(other.entityCache), // Assumes Cache is copyable
//       nextId(other.nextId),
//       stopFlag(other.stopFlag),
//       isTesting(other.isTesting)
// {
//     // Note: movementThread is not copied; it remains default-constructed (not running).
//     // If you need the thread to be active, you'd need to restart it manually.
// }
//
// // Move constructor
// template <typename Algorithm>
// World<Algorithm>::World(World&& other) noexcept
//     : lowerRight(std::move(other.lowerRight)),
//       entityCache(std::move(other.entityCache)), // Assumes Cache is movable
//       nextId(other.nextId),
//       stopFlag(other.stopFlag),
//       isTesting(other.isTesting),
//       movementThread(std::move(other.movementThread)) // Transfer thread ownership
// {
//     other.nextId = 0; // Reset moved-from object's nextId (optional, for safety)
//     other.stopFlag = true; // Ensure the moved-from object doesn't interfere
// }
//
// // Copy assignment operator
// template <typename Algorithm>
// World<Algorithm>& World<Algorithm>::operator=(const World& other)
// {
//     if (this != &other) {
//         stopRandomMovements(); // Stop our current thread before overwriting
//         lowerRight = other.lowerRight;
//         entityCache = other.entityCache; // Assumes Cache is copyable
//         nextId = other.nextId;
//         stopFlag = other.stopFlag;
//         isTesting = other.isTesting;
//         // movementThread remains default-constructed (not copied)
//     }
//     return *this;
// }
//
// // Move assignment operator
// template <typename Algorithm>
// World<Algorithm>& World<Algorithm>::operator=(World&& other) noexcept
// {
//     if (this != &other) {
//         stopRandomMovements(); // Stop our current thread before overwriting
//         lowerRight = std::move(other.lowerRight);
//         entityCache = std::move(other.entityCache); // Assumes Cache is movable
//         nextId = other.nextId;
//         stopFlag = other.stopFlag;
//         isTesting = other.isTesting;
//         movementThread = std::move(other.movementThread); // Transfer thread ownership
//         other.nextId = 0; // Reset moved-from object (optional)
//         other.stopFlag = true; // Ensure moved-from object doesn't interfere
//     }
//     return *this;
// }
// template <typename Algorithm>
// World<Algorithm>::~World()
// {
//     stopRandomMovements(); // Ensure thread is stopped before destruction
// }
//
// template <typename Algorithm>
// void World<Algorithm>::addEntity(std::unique_ptr<WorldEntity>&& entity, const Point2D& position)
// {
//     entityCache.addEntity(std::move(entity), position, entity->getId());
// }
//
// template <typename Algorithm>
// void World<Algorithm>::clear()
// {
//     entityCache.clear();
// }
//
// template <typename Algorithm>
// void World<Algorithm>::startRandomMovements()
// {
//     stopFlag = false;
//     movementThread = std::thread(&World::randomMovementLoop, this);
// }
//
// template <typename Algorithm>
// void World<Algorithm>::stopRandomMovements()
// {
//     stopFlag = true;
//     if (movementThread.joinable()) {
//         movementThread.join();
//     }
// }
//
// template <typename Algorithm>
// double World<Algorithm>::getWidth()
// {
//     return lowerRight.getX();
// }
//
// template <typename Algorithm>
// double World<Algorithm>::getHeight()
// {
//     return lowerRight.getY();
// }
//
// template <typename Algorithm>
// void World<Algorithm>::setTestingStatus(bool status)
// {
//     isTesting = status;
// }
//
// template <typename Algorithm>
// void World<Algorithm>::addNEntities(int n)
// {
//     for (int i = 0; i < n; ++i)
//     {
//         int randomX = std::rand() % (int)getWidth();
//         int randomY = std::rand() % (int)getHeight();
//         addEntity(std::unique_ptr<WorldEntity>(new WorldEntity(nextId++)), Point2D(randomX, randomY));
//     }
// }
//
// template <typename Algorithm>
// void World<Algorithm>::getAllEntities(std::function<void(EntityView<WorldEntity>& safeView)> consumer) {
//     entityCache.getAllEntities(consumer);
// }
//
// template <typename Algorithm>
// void World<Algorithm>::selectArea(PositionalCache::Bounds boundingBox, std::function<void(EntityView<WorldEntity>& safeView)> consumer)
// {
//     entityCache.selectArea(boundingBox, consumer);
// }
//
// template <typename Algorithm>
// bool World<Algorithm>::isValidEntity(int id) {
//     return entityCache.isValidEntity(id);
// }
//
// template <typename Algorithm>
// WorldEntity& World<Algorithm>::getEntityById(int id) {
//     return entityCache.getEntityById(id).getEntity();
// }
//
// template <typename Algorithm>
// void World<Algorithm>::randomMovementLoop() // Simulate movement of entities
// {
//     // Random number generator setup
//     std::random_device rd; // Seed generator
//     std::mt19937 gen(rd()); // Random number generator
//     std::uniform_real_distribution<> moveChance(0.0, 1.0);
//     std::uniform_int_distribution<> sleepDuration(100, 1000);
//
//     // Distributions for newPosition generation
//     std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
//     std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);
//
//     while (!stopFlag) {
//         if (!isTesting)
//         {
//             entityCache.getAllEntities([&](EntityView<WorldEntity>& safeView) {
//                 if (moveChance(gen) < 0.1) // 10% chance to move
//                 {
//                     Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
//                     safeView.getEntity().updatePosition(newPosition);
//                 }
//             });
//         }
//
//         // Sleep for a random time between 100ms and 1000ms
//         std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration(gen)));
//     }
// }
//
// template <typename Algorithm>
// void World<Algorithm>::shuffleEntityPositions()
// {
//     if (entityCache.entityCount() == 0) return;
//
//     // Random number generator setup
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> posXDistribution(0, std::floor(lowerRight.getX()) - 1);
//     std::uniform_int_distribution<> posYDistribution(0, std::floor(lowerRight.getY()) - 1);
//
//     // Shuffle all entity positions
//     entityCache.getAllEntities([&](EntityView<WorldEntity>& safeView) {
//         Point2D newPosition(posXDistribution(gen), posYDistribution(gen));
//         safeView.getEntity().updatePosition(newPosition);
//     });
// }
//
// }