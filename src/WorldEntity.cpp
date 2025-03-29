#include "WorldEntity.h"

#include <iostream>

using namespace PositionalCache;

namespace FrameworkUser
{
WorldEntity::WorldEntity(Point2D position, int id)
: observer(nullptr), id(id), color(EntityColor::Blue)
{
    coordinates.setX(position.getX());
    coordinates.setY(position.getY());
}

void WorldEntity::setColor(EntityColor color)
{
    this->color = color;
}

EntityColor WorldEntity::getColor() const
{
    return color;
}

void WorldEntity::updatePosition(Point2D newCoordinates)
{
    coordinates.setX(newCoordinates.getX());
    coordinates.setY(newCoordinates.getY());
    notifyObserver();  // Notify observers whenever position changes
}

Point2D WorldEntity::getPosition() const
{
    return coordinates;
}

void WorldEntity::addObserver(Observer<WorldEntity>&& observer) {
    this->observer = observer;
}

void WorldEntity::notifyObserver() {
    observer.onPositionChanged(this->id, this->coordinates);
}

int WorldEntity::getId() const
{
    return id;
}
}
