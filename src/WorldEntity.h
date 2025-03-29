#pragma once

#include "Cache.h"
#include "Point2D.h"
using namespace PositionalCache;

namespace FrameworkUser
{
enum class EntityColor {
    Red,
    Blue,
    Green
};

class WorldEntity
{
public:
    WorldEntity(Point2D position, int id);
    void setColor(EntityColor color);
    EntityColor getColor() const;
    void updatePosition(Point2D newCoordinates);
    Point2D getPosition() const;
    void addObserver(Observer<WorldEntity>&& observer);
    int getId() const;

private:
    Point2D coordinates;
    EntityColor color;
    int id;

    Observer<WorldEntity> observer;
    void notifyObserver();  // Notify observer about the position change
};
}
