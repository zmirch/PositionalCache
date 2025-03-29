#include "WorldEntity.h"
#include <iostream>

using namespace PositionalCache;

namespace FrameworkUser
{
WorldEntity::WorldEntity(int id)
: id(id), color(EntityColor::Blue)
{}

void WorldEntity::setColor(EntityColor color)
{
    this->color = color;
}

EntityColor WorldEntity::getColor() const
{
    return color;
}

int WorldEntity::getId() const
{
    return id;
}
}
