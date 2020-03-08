#pragma once

#include "System.hpp"
#include "ComponentStorage.hpp"
#include "SystemBatch.hpp"
#include "SystemDependencies.hpp"

#include <queue>

namespace ecs {

	class Engine {
	public:
		Engine() :
			_threads(std::thread::hardware_concurrency())
		{}

		Entity& createEntity();

		void deleteEntity(Entity::Handle handle);

		template<typename Component>
		void addComponent(Entity& entity, const Component& component);

		template<typename Component>
		void addComponent(Entity& entity, Component&& component);

		template<typename ... Components>
		void addComponents(Entity& entity, const Components& ... components);

		template<typename Component>
		void removeComponent(Entity& entity);

		template<typename ... Components>
		void removeComponents(Entity& entity);

		template<typename T, typename = typename std::enable_if<std::is_base_of<ISystem, T>::value, T>::type>
		void registerSystem(std::unique_ptr<T> system);

		template<typename T, typename = typename std::enable_if<std::is_base_of<ISystem, T>::value, T>::type>
		void registerSystem(std::unique_ptr<T> system, SystemDependencies dependencies);

		template<typename T, typename = typename std::enable_if<std::is_base_of<ISystem, T>::value, T>::type>
		void unregisterSystem();

		template<typename T>
		bool containsSystem() const;

		void updateSystems(float deltatime);

		void updateSystems(float deltatime, size_t nThreads);

	private:
		struct SystemData {
			SystemData(std::unique_ptr<ISystem> system, std::type_index type, SystemDependencies dependencies) :
				system(std::move(system)), type(type), dependencies(std::move(dependencies))
			{}
			std::unique_ptr<ISystem> system;
			std::type_index type;
			SystemDependencies dependencies;
		};

		std::vector<std::unique_ptr<Entity>> _entities;
		std::queue<Entity::Handle> _freeEntitySpaces;

		std::vector<SystemData> _systems;
		std::vector<SystemBatch> _systemBatches;
		bool _dirtySystemBatches = false;

		ComponentStorage _components;

		void systemCheckEntity(Entity& entity);

		void buildSystemBatches();
		std::vector<SystemBatch>::iterator addSystemFrom(std::vector<SystemBatch>::iterator begin, ISystem* system);

		template<typename First, typename Second, typename ... T>
		void _addComponents(Entity& entity, const First& first, const Second& second, const T&... t);

		template<typename Last>
		void _addComponents(Entity& entity, const Last& last);

		template<typename First, typename Second, typename ... T>
		void _removeComponents(Entity& entity);

		template<typename Last>
		void _removeComponents(Entity& entity);

		ctpl::thread_pool _threads;
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
		_components.createComponent(entity, &component);

		systemCheckEntity(entity);
	}

	template<typename Component>
	inline void Engine::addComponent(Entity& entity, Component&& component) {
		_components.createComponent(entity, std::move(component));

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
		registerSystem(std::move(system), SystemDependencies::create());
	}

	template<typename T, typename>
	inline void Engine::registerSystem(std::unique_ptr<T> system, SystemDependencies dependencies) {
		assert(!containsSystem<T>());
		_systems.emplace_back(std::move(system), std::type_index(typeid(T)), std::move(dependencies));
		_dirtySystemBatches = true;
	}

	template<typename T, typename>
	inline void Engine::unregisterSystem() {
		assert(containsSystem<T>());
		auto system = std::find_if(_systems.begin(), _systems.end(),
			[&](const std::pair<std::unique_ptr<ISystem>, std::type_index>& other) { 
				other.second == std::type_index(typeid(T)); 
			}
		);

		for (auto& batch : _systemBatches) {
			if (batch.containsSystem(system->get()))
				batch.removeSystem(system->get());
		}

		_systems.erase(system);
	}

	template<typename T>
	inline bool Engine::containsSystem() const {
		const auto id = std::type_index(typeid(T));

		for (const auto& system : _systems)
			if (id == system.type)
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
		if (_dirtySystemBatches)
			buildSystemBatches();

		ChangeBuffer buffer;

		for (auto& systemBatch : _systemBatches) {
			systemBatch.update(deltatime, buffer, _threads);
		}


		// Create new entities and add components to them
		for (size_t i = 0; i < buffer._entitiesToBeCreated; ++i) {
			auto& entity = createEntity();

			for (const auto& component : buffer._newEntityComponentAddBuffer[i])
				_components.createComponent(entity, component.first, component.second);

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

	inline void Engine::updateSystems(float deltatime, size_t nThreads) {
		_threads.resize(nThreads);
		updateSystems(deltatime);
	}

	inline void Engine::systemCheckEntity(Entity& entity) {
		for (auto& system : _systems) {
			if (system.system->fitsEntity(entity)) {
				if (!system.system->containsEntity(entity))
					system.system->addEntity(entity);
			}else{
				if (system.system->containsEntity(entity))
					system.system->removeEntity(entity);
			}
		}
	}

	inline void Engine::buildSystemBatches() {
		// Start with a clean slate
		_systemBatches.clear();

		// Create a list of all systems which need to be added
		std::unordered_map<std::type_index, SystemData*> systemsToBeAdded;
		systemsToBeAdded.reserve(_systems.size());
		for (auto& system : _systems)
			systemsToBeAdded.emplace(system.type, &system);

		// Create a structure of systems which have dependencies
		std::unordered_map<std::type_index, std::pair<SystemData*, bool>> dependentSystems;
		for (auto system : systemsToBeAdded)
			if (!system.second->dependencies.empty())
				dependentSystems.emplace(system.first, std::make_pair(system.second, true));

		// Check which of the systems are on top of the dependency hierarchy
		for (const auto& system : dependentSystems) {
			for (const auto dep : system.second.first->dependencies.getDependencies()) {
				auto it = dependentSystems.find(dep);
				if(it != dependentSystems.end())
					it->second.second = false;
			}
		}

		// Create a queue of dependent systems to be added to the batches
		std::queue<std::pair<SystemData*, std::vector<SystemBatch>::iterator>> processQueue;
		for (const auto& system : dependentSystems) {
			if (system.second.second) {
				processQueue.emplace(system.second.first, _systemBatches.begin());
				systemsToBeAdded.erase(system.first);
			}
		}

		// Add the dependent systems to the batches
		while (!processQueue.empty()) {
			auto& system = processQueue.front();

			auto it = addSystemFrom(system.second, system.first->system.get());
			for (const auto dep : system.first->dependencies.getDependencies()) {
				auto newSystem = systemsToBeAdded.find(dep);
				if (newSystem != systemsToBeAdded.end()) {
					processQueue.emplace(newSystem->second, it);
					systemsToBeAdded.erase(newSystem);
				}
			}

			processQueue.pop();
		}

		// The dependencies are reversed, so let's reverse the batches
		std::reverse(_systemBatches.begin(), _systemBatches.end());

		// Add the remaining systems
		for (auto& system : systemsToBeAdded)
			addSystemFrom(_systemBatches.begin(), system.second->system.get());

		_dirtySystemBatches = false;
	}

	inline std::vector<SystemBatch>::iterator Engine::addSystemFrom(std::vector<SystemBatch>::iterator begin, ISystem* system) {
		for (auto it = begin; it != _systemBatches.end(); ++it) {
			if (it->fitsSystem(system)) {
				it->addSystem(system);
				return it;
			}
		}
		_systemBatches.emplace_back();
		_systemBatches.back().addSystem(system);
		return _systemBatches.end() - 1;
	}
}
