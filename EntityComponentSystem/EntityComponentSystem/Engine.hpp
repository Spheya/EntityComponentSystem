#pragma once

#include "System.hpp"
#include "ComponentStorage.hpp"

#include <queue>

namespace ecs {

	class Engine {
	public:
		Entity& createEntity();

		void deleteEntity(Entity::Handle handle);

		template<typename Component>
		void addComponent(Entity& entity, const Component& component);

		template<typename ... Components>
		void addComponents(Entity& entity, const Components& ... components);

		template<typename Component>
		void removeComponent(Entity& entity);

		template<typename ... Components>
		void removeComponents(Entity& entity);

		template<typename T, typename = typename std::enable_if<std::is_base_of<ISystem, T>::value, T>::type>
		void registerSystem(std::unique_ptr<T> system);

		template<typename T, typename = typename std::enable_if<std::is_base_of<ISystem, T>::value, T>::type>
		void unregisterSystem();

		template<typename T>
		bool containsSystem() const;

		void updateSystems(float deltatime);

	private:
		std::vector<std::unique_ptr<Entity>> _entities;
		std::queue<Entity::Handle> _freeEntitySpaces;

		std::vector<std::pair<std::unique_ptr<ISystem>, std::type_index>> _systems;

		ComponentStorage _components;

		void systemCheckEntity(Entity& entity);

		template<typename First, typename Second, typename ... T>
		void _addComponents(Entity& entity, const First& first, const Second& second, const T&... t);

		template<typename Last>
		void _addComponents(Entity& entity, const Last& last);

		template<typename First, typename Second, typename ... T>
		void _removeComponents(Entity& entity);

		template<typename Last>
		void _removeComponents(Entity& entity);
	};

	inline Entity& Engine::createEntity() {
		Entity* e;
		if (_freeEntitySpaces.empty()) {
			_entities.emplace_back(new Entity(_entities.size()));
			e = &*_entities.back();
		} else {
			_entities[_freeEntitySpaces.front()].reset(new Entity(_freeEntitySpaces.front()));
			auto& entity = *_entities[_freeEntitySpaces.front()];
			_freeEntitySpaces.pop();
			e = &entity;
		}

		systemCheckEntity(*e);
		return *e;
	}
	
	inline void Engine::deleteEntity(Entity::Handle handle) {
		assert(_entities.size() > handle);
		assert(_entities[handle]);

		Entity& entity = *_entities[handle];

		while (entity._components.size())
			_components.deleteComponent(entity, entity._components.begin()->first);

		_entities[handle] = nullptr;
		_freeEntitySpaces.push(handle);
	}

	template<typename Component>
	inline void Engine::addComponent(Entity& entity, const Component& component) {
		static const auto id = ecs::Component::getId<Component>();
		_components.createComponent(entity, id, &component);

		systemCheckEntity(entity);
	}

	template<typename ...Components>
	inline void Engine::addComponents(Entity& entity, const Components& ...components) {
		_addComponents(entity, components...);

		systemCheckEntity(entity);
	}

	template<typename Component>
	inline void Engine::removeComponent(Entity& entity) {
		_components.deleteComponent<Component>(entity);

		systemCheckEntity(entity);
	}

	template<typename ... Components>
	inline void Engine::removeComponents(Entity& entity) {
		_removeComponents<Components...>(entity);

		systemCheckEntity(entity);
	}

	template<typename T, typename>
	inline void Engine::registerSystem(std::unique_ptr<T> system) {
		assert(!containsSystem<T>());
		_systems.push_back(std::make_pair<std::unique_ptr<ISystem>, std::type_index>(std::move(system), std::type_index(typeid(T))));
	}

	template<typename T, typename>
	inline void Engine::unregisterSystem() {
		assert(containsSystem<T>());
		_systems.erase(std::find_if(_systems.begin(), _systems.end(),
			[&](const std::pair<std::unique_ptr<ISystem>, std::type_index>& other) { 
				other.second == std::type_index(typeid(T)); 
			}
		));
	}

	template<typename T>
	inline bool Engine::containsSystem() const {
		const auto id = std::type_index(typeid(T));

		for (const auto& system : _systems)
			if (id == system.second)
				return true;
		return false;
	}

	template<typename First, typename Second, typename ... T>
	inline void Engine::_addComponents(Entity& entity, const First& first, const Second& second, const T&... t) {
		_components.createComponent(entity, first);
		_addComponents(entity, second, t...);
	}

	template<typename Last>
	inline void Engine::_addComponents(Entity& entity, const Last& last) {
		_components.createComponent(entity, last);
	}

	template<typename First, typename Second, typename ...T>
	inline void Engine::_removeComponents(Entity& entity) {
		_components.deleteComponent<First>(entity);
		_removeComponents<Second, T...>(entity);
	}

	template<typename Last>
	inline void Engine::_removeComponents(Entity& entity) {
		_components.deleteComponent<Last>(entity);
	}

	inline void Engine::updateSystems(float deltatime) {
		ChangeBuffer buffer;

		for (auto& system : _systems)
			system.first->update(deltatime, buffer);


		// Create new entities and add components to them
		for (int i = 0; i < buffer._entitiesToBeCreated; ++i) {
			auto& entity = createEntity();

			for (const auto& component : buffer._newEntityComponentAddBuffer[i])
				_components.createComponent(entity, component.first, component.second.get());

			systemCheckEntity(entity);
		}

		// Add components to existing entities
		for (const auto& componentBatch : buffer._componentAddBuffer) {
			Entity& entity = *_entities[componentBatch.first];

			for (const auto& component : componentBatch.second)
				_components.createComponent(entity, component.first, component.second->get());
		
			systemCheckEntity(entity);
		}

		// Remove components from existing entities
		for (const auto& componentBatch : buffer._componentRemovalBuffer) {
			Entity& entity = *_entities[componentBatch.first];

			for (const auto component : componentBatch.second)
				_components.deleteComponent(entity, component);
			
			systemCheckEntity(entity);
		}
	}

	inline void Engine::systemCheckEntity(Entity& entity) {
		for (auto& system : _systems) {
			if (system.first->fitsEntity(entity)) {
				if (!system.first->containsEntity(entity))
					system.first->addEntity(entity);
			}else{
				if (system.first->containsEntity(entity))
					system.first->removeEntity(entity);
			}
		}
	}
}
