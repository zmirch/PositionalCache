#pragma once
#include <memory>

namespace PositionalCache
{
	template <typename E>
	class EntityHandle
	{
	public:
		EntityHandle(std::unique_ptr<E>&& entity, int id)
			: engineEntity(std::move(entity)), id(id) {}

		E* getEntity()
		{
			return engineEntity.get();
		}

		bool hasEntity()
		{
			return engineEntity.get();
		}

		void disable()
		{
			engineEntity.reset();
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

	private:
		// Pe viitor: Posibil ca nu e nevoie sa contina un pointer, ci sa aiba suficienta informatie ca motorul sa poata 
		// identifica la care engineEntity se refera
		std::unique_ptr<E> engineEntity = nullptr;
		int id;
	};
}

