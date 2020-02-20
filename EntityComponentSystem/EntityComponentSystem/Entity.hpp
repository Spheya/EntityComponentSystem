#pragma once

#include <bitset>
#include <unordered_map>
#include <typeindex>
#include <cassert>

#include "Component.hpp"

#ifndef ECS_KEY_SIZE
#define ECS_KEY_SIZE 256
#endif

namespace ecs {
	class Entity {

		friend class ComponentStorage;
		friend class Engine;

	public:
		using Handle = size_t;

		Entity(Entity&) = delete;
		Entity& operator=(Entity&) = delete;
		Entity(Entity&&) = delete;
		Entity& operator=(Entity&&) = delete;

		template<typename T>
		bool hasComponent() const;
		bool hasComponent(Component::Id id) const;

		template<typename T>
		T* getComponent();

		template<typename T>
		const T* getComponent() const;

		Handle getHandle() const;

		std::bitset<ECS_KEY_SIZE>& getKey();
		const std::bitset<ECS_KEY_SIZE>& getKey() const;

	private:
		Entity(Handle handle) : _handle(handle) {};

		std::bitset<ECS_KEY_SIZE> _key;
		std::unordered_map<Component::Id, void*> _components;
		Handle _handle;

		void* getComponent(Component::Id id);
		const void* getComponent(Component::Id id) const;
		
		void addComponent(void* component, Component::Id id);
		void removeComponent(Component::Id id);
	};
}

template<typename T>
inline bool ecs::Entity::hasComponent() const {
	static const auto id = Component::getId<T>();
	return hasComponent(id);
}

template<typename T>
inline T* ecs::Entity::getComponent() {
	static const auto id = Component::getId<T>();
	return reinterpret_cast<T*>(getComponent(id));
}

template<typename T>
inline const T* ecs::Entity::getComponent() const {
	static const auto id = Component::getId<T>();
	return reinterpret_cast<const T*>(getComponent(id));
}

inline std::bitset<ECS_KEY_SIZE>& ecs::Entity::getKey() {
	return _key;
}

inline const std::bitset<ECS_KEY_SIZE>& ecs::Entity::getKey() const {
	return _key;
}

inline ecs::Entity::Handle ecs::Entity::getHandle() const {
	return _handle;
}

inline bool ecs::Entity::hasComponent(Component::Id id) const {
	return _key[id];
}

void* ecs::Entity::getComponent(Component::Id id) {
	assert(hasComponent(id));
	return _components.find(id)->second;
}

inline const void* ecs::Entity::getComponent(Component::Id id) const {
	assert(hasComponent(id));
	return _components.find(id)->second;
}

inline void ecs::Entity::addComponent(void* component, Component::Id id) {
	_components.emplace(id, component);
	_key.set(id);
}

inline void ecs::Entity::removeComponent(Component::Id id) {
	assert(getComponent(id) != nullptr);
	_components.erase(_components.find(id));
	_key.set(id, false);
}
