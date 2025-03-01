#pragma once
#include <memory>

namespace PositionalCache
{
	template <typename E>
	class EntityHandle
	{
	public:
		EntityHandle(E* entity, int id)
		{
			this->engineEntity = entity;
			this->id = id;
		}

		E* getEntity() const
		{
			return engineEntity;
		}

		bool hasEntity() const
		{
			return engineEntity;
		}

		void disable()
		{
			engineEntity = nullptr;
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
		E* engineEntity = nullptr;
		//std::unique_ptr<E> engineEntity = nullptr;
		int id;
	};
}

