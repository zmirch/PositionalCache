#pragma once

#include <deque>
#include <thread>
#include <random>

#include "EngineEntity.h"
#include "Point2D.h"
#include "Cache.h"
#include "Bounds.h"

using namespace PositionalCache;

class Area2D
{
public:
	Area2D(Point2D lowerRight);
	~Area2D();

	std::deque<EngineEntity> getEntities();
	void addEntity(std::unique_ptr<EngineEntity>&& entity);
	void clear();
	void startRandomMovements();
	void stopRandomMovements();
	double getWidth();
	double getHeight();
	void setTestingStatus(bool status);
	void addNEntities(int n);
	void shuffleEntityPositions();
	bool isTesting;
	void getAllEntities(std::function<void(EntityView<EngineEntity>& safeView)> consumer);
	void selectArea(PositionalCache::Bounds boundingBox, std::function<void(EntityView<EngineEntity>& safeView)> consumer);
	bool isValidEntity(int id);

	EngineEntity& getEntityById(int id);
private:
	Cache<EngineEntity> entityCache;
	int nextId = 0;
	void randomMovementLoop();
	Point2D lowerRight; // Coordinates for the lower right corner of the 2D area
	std::thread movementThread;
	bool stopFlag;	

};

