#pragma once
#include <memory>
#include "Point2D.h"

namespace PositionalCache
{
/// @brief
template <typename E>
class Entity
{
public:
	Entity(std::unique_ptr<E>&& entity, int id, const Point2D& position, const std::function<void (Entity<E>&, const Point2D&)>& updateCallback)
		: engineEntityPtr(entity.release()), id(id), position(position), updateCallback(updateCallback) {}

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
		Point2D oldCoordinates = position;
		position.setX(newCoordinates.getX());
		position.setY(newCoordinates.getY());
		//if (updateCallback)
		updateCallback(*this, oldCoordinates);
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
	Entity(Entity&& other) noexcept
	: engineEntityPtr(other.engineEntityPtr),
	  id(other.id),
	  position(other.position),
	  updateCallback(std::move(other.updateCallback))
	{
			other.engineEntityPtr = nullptr;
	}

	Entity& operator=(Entity&& other) noexcept {
		engineEntityPtr = other.engineEntityPtr;
		id = other.id;
		position = other.position;
		other.engineEntityPtr = nullptr;
		return *this;
	}

	void Update()
	{
		updateCallback(position);
	}

private:
	E* engineEntityPtr = nullptr;
	int id;
	Point2D position;
	std::function<void (Entity<E>&, const Point2D&)> updateCallback;
};
}

