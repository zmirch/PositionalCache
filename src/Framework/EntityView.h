#pragma once

#include "EntityHandle.h"

namespace PositionalCache {

template <typename E>
class EntityView {
// for when we simply want to view entity variables rather than store it
public:
    EntityView(std::shared_ptr<Entity<E>>& ptr) : entityPtr(&ptr) {}
    EntityHandle<E> getHandle()
    {
        auto copyOfPointer = *entityPtr;
        EntityHandle<E> result(std::move(copyOfPointer));
        return std::move(result); // std::move so we don't make a copy
    }

    Entity<E>& getEntity() const
    {
        return *entityPtr->get();
    }

    EntityView(const EntityView &other) = delete;

    EntityView(EntityView &&other) noexcept = delete;

    EntityView & operator=(const EntityView &other) = delete;

    EntityView & operator=(EntityView &&other) noexcept = delete;

    bool hasEntity() {
        return entityPtr->get()->hasEntity();
    };

private:
    std::shared_ptr<Entity<E>>* entityPtr;
};

}