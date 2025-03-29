#include "EngineEntity.h"

#include <iostream>

using namespace PositionalCache;

namespace FrameworkUser
{
EngineEntity::EngineEntity(Point2D position, int id)
: observer(nullptr), id(id), color(EntityColor::Blue)
{
    coordinates.setX(position.getX());
    coordinates.setY(position.getY());
}

void EngineEntity::setColor(EntityColor color)
{
    this->color = color;
}

EntityColor EngineEntity::getColor() const
{
    return color;
}

void EngineEntity::updatePosition(Point2D newCoordinates)
{
    coordinates.setX(newCoordinates.getX());
    coordinates.setY(newCoordinates.getY());
    notifyObserver();  // Notify observers whenever position changes
}

Point2D EngineEntity::getPosition() const
{
    return coordinates;
}

void EngineEntity::addObserver(Observer<EngineEntity>&& observer) {
    this->observer = observer;
}

void EngineEntity::notifyObserver() {
    observer.onPositionChanged(this->id, this->coordinates);
}

int EngineEntity::getId() const
{
    return id;
}
}
