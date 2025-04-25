#pragma once

#include "../Framework/Cache.h"
#include "../Framework/Point2D.h"
#include "../Framework/Algorithms/BasicAlgorithm.h"
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
    WorldEntity(int id);
    void setColor(EntityColor color);
    EntityColor getColor() const;
    int getId() const;

private:
    EntityColor color;
    int id;
};
}
