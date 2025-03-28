#pragma once

#include "Cache.h"
#include "Point2D.h"
using namespace PositionalCache;

enum class EntityColor {
    Red,
    Blue,
    Green
};

class EngineEntity
{
public:
    EngineEntity(Point2D position, int id);
    void setColor(EntityColor color);
    EntityColor getColor() const;
    void updatePosition(Point2D newCoordinates);
    Point2D getPosition() const;
    void addObserver(Observer<EngineEntity>&& observer);
    int getId() const;

private:
    Point2D coordinates;
    EntityColor color;
    int id;

    Observer<EngineEntity> observer;
    void notifyObserver();  // Notify observer about the position change
};