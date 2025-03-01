#pragma once

#include <deque>
#include <chrono>
#include <thread>
#include <random>
#include <cmath>

#include "EngineEntity.h"
#include "Point2D.h"
#include "EntityCache.h"
#include "Bounds.h"

using namespace PositionalCache;

class Area2D
{
public:
	Area2D(Point2D lowerRight);
	~Area2D();

	std::deque<EngineEntity> getEntities();
	void addEntity(EngineEntity&& entity);
	void clear();
	void startRandomMovements();
	void stopRandomMovements();
	double getWidth();
	double getHeight();
	std::vector<EntityHandle<EngineEntity>> squareSelection(PositionalCache::Bounds boundingBox);
	void setTestingStatus(bool status);
	void addNEntities(int n);
	void shuffleEntityPositions();
	bool isTesting;
private:
	int nextId = 0;
	EntityCache<EngineEntity> entityCache;
	void randomMovementLoop();
	std::deque<EngineEntity> entities;
	Point2D lowerRight; // Coordinates for the lower right corner of the 2D area
	std::thread movementThread;
	bool stopFlag;	

};

