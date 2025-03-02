#pragma once
#include <memory>

namespace PositionalCache
{
	template <typename E>
	class EntityHandle
	{
	public:
		EntityHandle(std::unique_ptr<E>&& entity, int id)
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
			bool operator() (const EntityHandle<E>& lhs, const EntityHandle<E>& rhs) const
			{
				return lhs.id < rhs.id;
			}
		};

		~EntityHandle()
		{
			if (engineEntityPtr)
			{
				delete engineEntityPtr;
				engineEntityPtr = nullptr;
			}
		}

		EntityHandle(const EntityHandle& entityHandle) = delete;
		EntityHandle& operator= (const EntityHandle& entityHandle) = delete;
		EntityHandle(EntityHandle&& other) noexcept {
			engineEntityPtr = other.engineEntityPtr;
			id = other.id;
			other.engineEntityPtr = nullptr;
		}
		EntityHandle& operator=(EntityHandle&& other) noexcept {
			engineEntityPtr = other.engineEntityPtr;
			id = other.id;
			other.engineEntityPtr = nullptr;
			return *this;
		}

	private:
		// Pe viitor: Posibil ca nu e nevoie sa contina un pointer, ci sa aiba suficienta informatie ca motorul sa poata 
		// identifica la care engineEntity se refera
		E* engineEntityPtr = nullptr;
		int id;
	};
}

