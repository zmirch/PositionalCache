#pragma once
#include <memory>
#include <vector>


namespace PositionalCache
{
/// @brief
template <typename E>
class CacheEntity
{
public:
	CacheEntity(std::unique_ptr<E>&& entity, int id)
		: engineEntityPtr(entity.release()), id(id){}

	E& getEntity()
	{
		return *engineEntityPtr;
	}

	bool hasEntity()
	{
		return engineEntityPtr;
	}

	int getId()
	{
		return id;
	}

	struct Compare
	{
		bool operator() (const CacheEntity<E>& lhs, const CacheEntity<E>& rhs) const
		{
			return lhs.id < rhs.id;
		}
	};

	~CacheEntity()
	{
		if (engineEntityPtr)
		{
			delete engineEntityPtr;
			engineEntityPtr = nullptr;
		}
	}

	CacheEntity(const CacheEntity& entityHandle) = delete;
	CacheEntity& operator= (const CacheEntity& entityHandle) = delete;
	CacheEntity(CacheEntity&& other) noexcept {
		engineEntityPtr = other.engineEntityPtr;
		id = other.id;
		other.engineEntityPtr = nullptr;
	}
	CacheEntity& operator=(CacheEntity&& other) noexcept {
		engineEntityPtr = other.engineEntityPtr;
		id = other.id;
		other.engineEntityPtr = nullptr;
		return *this;
	}

private:
	E* engineEntityPtr = nullptr;
	int id;
};
}

