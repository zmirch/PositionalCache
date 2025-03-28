#pragma once

#include <memory>
#include "Entity.h"

namespace PositionalCache {
template <typename E>
class EntityHandle {

public:
    explicit EntityHandle(std::shared_ptr<Entity<E>>&& ptr) : entityPtr(std::move(ptr)) {}

    Entity<E>* operator->() const
    {   // overload arrow operator to allow accessing to the entity
        return entityPtr.get();
    }

private:
    std::shared_ptr<Entity<E>> entityPtr;

};
}