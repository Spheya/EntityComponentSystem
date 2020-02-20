#pragma once

#include <vector>
#include <unordered_map>

#include "Entity.hpp"

namespace ecs {
	class ChangeBuffer {
		friend class Engine;
		
	public:
		size_t createEntity();

		template<typename Component>
		void addComponent(Entity& entity, Component component);
		template<typename Component>
		void addComponent(size_t entity, Component component);

		template<typename Component>
		void removeComponent(Entity& entity);
		template<typename Component>
		void removeComponent(size_t entity);

	private:
		size_t _entitiesToBeCreated;
		std::vector<std::vector<std::pair<Component::Id, void*>>> _newEntityComponentAddBuffer;
		std::unordered_map<Entity*, std::vector<std::pair<Component::Id, void*>>> _componentAddBuffer;
		std::unordered_map<Entity*, std::vector<Component::Id>> _componentRemovalBuffer;
	};

	inline size_t ChangeBuffer::createEntity() {
		_newEntityComponentAddBuffer.emplace_back();
		return _entitiesToBeCreated++;
	}
}