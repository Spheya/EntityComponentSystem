#pragma once

#include <memory>

#include "MemoryPoolAllocator.hpp"
#include "Entity.hpp"

namespace ecs {
	class ComponentStorage {
	public:
		template<typename T>
		void createComponent(Entity& entity, const T& component);

		template<typename T>
		void createComponent(Entity& entity, T&& component);

		template<typename T>
		void deleteComponent(Entity& entity);

	private:
		void createComponent(Entity& entity, Component::Id id, const void* data);

		void deleteComponent(Entity& entity, Component::Id id);
		
		template<typename T>
		MemoryPoolAllocator<T>* getAllocator();
		IMemoryPoolAllocator* getAllocator(Component::Id id);

		std::unordered_map<Component::Id, std::unique_ptr<IMemoryPoolAllocator>> _allocators;

	};
	
	template<typename T>
	inline void ComponentStorage::createComponent(Entity& entity, const T& component) {
		const auto allocator = getAllocator<T>();
		const auto comp = allocator->allocate(component);
		static const auto id = Component::GetId<T>();
		entity.addComponent(comp, id);
	}

	template<typename T>
	inline void ComponentStorage::createComponent(Entity& entity, T&& component) {
		const auto allocator = getAllocator<T>();
		const auto comp = allocator->allocate(std::move(component));
		static const auto id = Component::GetId<T>();
		entity.addComponent(comp, id);
	}

	template<typename T>
	inline void ComponentStorage::deleteComponent(Entity& entity) {
		static const auto id = Component::GetId<T>();
		entity.removeComponent(id);
	}

	template<typename T>
	inline MemoryPoolAllocator<T>* ComponentStorage::getAllocator() {
		static const auto id = Component::GetId<T>();
		return reinterpret_cast<MemoryPoolAllocator<T>*>(getAllocator(id));
	}

	inline void ComponentStorage::createComponent(Entity& entity, Component::Id id, const void* data) {
		assert(!entity.hasComponent(id));
		const auto allocator = getAllocator(id);
		const auto comp = allocator->insert(data);
		entity.addComponent(comp, id);
	}

	inline void ComponentStorage::deleteComponent(Entity& entity, Component::Id id) {
		assert(entity.hasComponent(id));
		const auto allocator = getAllocator(id);
		const auto comp = entity.getComponent(id);
		allocator->free(comp);
		entity.removeComponent(id);
	}

	inline IMemoryPoolAllocator* ecs::ComponentStorage::getAllocator(Component::Id id) {
		const auto it = _allocators.find(id);
		if (it == _allocators.end()) {
			auto allocator = Component::createAllocator(id);
			auto temp = allocator.get();
			_allocators.emplace(id, std::move(allocator));
			return temp;
		} else {
			return it->second.get();
		}
	}
}
