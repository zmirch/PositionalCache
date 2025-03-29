#pragma once

#include <deque>
#include <thread>
#include <random>

#include "WorldEntity.h"
#include "Point2D.h"
#include "Cache.h"
#include "Bounds.h"

using namespace PositionalCache;

namespace FrameworkUser
{

class World
{
public:
	World(Point2D lowerRight);
	~World();

	std::deque<WorldEntity> getEntities();
	void addEntity(std::unique_ptr<WorldEntity>&& entity);
	void clear();
	void startRandomMovements();
	void stopRandomMovements();
	double getWidth();
	double getHeight();
	void setTestingStatus(bool status);
	void addNEntities(int n);
	void shuffleEntityPositions();
	bool isTesting;
	void getAllEntities(std::function<void(EntityView<WorldEntity>& safeView)> consumer);
	void selectArea(PositionalCache::Bounds boundingBox, std::function<void(EntityView<WorldEntity>& safeView)> consumer);
	bool isValidEntity(int id);

	WorldEntity& getEntityById(int id);
private:
	Cache<WorldEntity> entityCache;
	int nextId = 0;
	void randomMovementLoop();
	Point2D lowerRight; // Coordinates for the lower right corner of the 2D area
	std::thread movementThread;
	bool stopFlag;

};

}