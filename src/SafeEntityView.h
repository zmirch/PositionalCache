#pragma once

#include "SafeEntityHandle.h"

namespace PositionalCache {

template <typename E>
class SafeEntityView {
// for when we simply want to view entity variables rather than store it
public:
    SafeEntityView(std::shared_ptr<CacheEntity<E>>& ptr) : entityPtr(&ptr) {}
    SafeEntityHandle<E> getHandle()
    {
        auto copyOfPointer = *entityPtr;
        SafeEntityHandle<E> result(std::move(copyOfPointer));
        return std::move(result); // std::move so we don't make a copy
    }

    E& getEntity()
    {
        return entityPtr->get()->getEntity();
    }

    SafeEntityView(const SafeEntityView &other) = delete;

    SafeEntityView(SafeEntityView &&other) noexcept = delete;

    SafeEntityView & operator=(const SafeEntityView &other) = delete;

    SafeEntityView & operator=(SafeEntityView &&other) noexcept = delete;

    bool hasEntity() {
        return entityPtr->get()->hasEntity();
    };

private:
    std::shared_ptr<CacheEntity<E>>* entityPtr;
};

}
