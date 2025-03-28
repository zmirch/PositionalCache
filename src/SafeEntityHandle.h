#pragma once

#include <memory>
#include "CacheEntity.h"

namespace PositionalCache {
template <typename E>
class SafeEntityHandle {

public:
    explicit SafeEntityHandle(std::shared_ptr<CacheEntity<E>>&& ptr) : entityPtr(std::move(ptr)) {}

    CacheEntity<E>* operator->() const
    {   // overload arrow operator to allow accessing to the entity
        return entityPtr.get();
    }

private:
    std::shared_ptr<CacheEntity<E>> entityPtr;

};
}