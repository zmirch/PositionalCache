#pragma once
#include <memory>
#include <vector>
#include "Point2D.h"

namespace PositionalCache
{
/// @brief
template <typename E>
class Entity
{
public:
	Entity(std::unique_ptr<E>&& entity, int id)
		: engineEntityPtr(entity.release()), id(id){}

	E& getEntity()
	{
		return *engineEntityPtr;
	}

	bool hasEntity()
	{
		return engineEntityPtr;
	}

	int getId()
	{
		return id;
	}

	void updatePosition(Point2D newCoordinates)
	{
		position.setX(newCoordinates.getX());
		position.setY(newCoordinates.getY());
	}

	Point2D getPosition() const
	{
		return position;
	}

	struct Compare
	{
		bool operator() (const Entity<E>& lhs, const Entity<E>& rhs) const
		{
			return lhs.id < rhs.id;
		}
	};

	~Entity()
	{
		if (engineEntityPtr)
		{
			delete engineEntityPtr;
			engineEntityPtr = nullptr;
		}
	}

	Entity(const Entity& entityHandle) = delete;
	Entity& operator= (const Entity& entityHandle) = delete;
	Entity(Entity&& other) noexcept {
		engineEntityPtr = other.engineEntityPtr;
		id = other.id;
		other.engineEntityPtr = nullptr;
	}
	Entity& operator=(Entity&& other) noexcept {
		engineEntityPtr = other.engineEntityPtr;
		id = other.id;
		other.engineEntityPtr = nullptr;
		return *this;
	}

private:
	E* engineEntityPtr = nullptr;
	Point2D position;
	int id;
};
}

