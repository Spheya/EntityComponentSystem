#pragma once

#include <vector>
#include <unordered_map>

#include "Entity.hpp"

namespace ecs {
	class ChangeBuffer {
		class IComponentPrefab {
		public:
			IComponentPrefab() = default;
			IComponentPrefab(IComponentPrefab&) = default;
			IComponentPrefab& operator=(IComponentPrefab&) = default;
			IComponentPrefab(IComponentPrefab&&) = default;
			IComponentPrefab& operator=(IComponentPrefab&&) = default;
			virtual ~IComponentPrefab() = default;

			virtual const void* get() const = 0;
		};

		template<typename T>
		class ComponentPrefab : IComponentPrefab {
		public:
			template<typename ... Args>
			ComponentPrefab(Args&&... args);

			const void* get() const override;

		private:
			std::unique_ptr<T> _ptr;
		};

	public:
		size_t createEntity();

		template<typename Component, typename ... Args>
		void addComponent(Entity& entity, Args&&... component);
		template<typename Component, typename ... Args>
		void addComponent(size_t entity, Args&&... component);

		template<typename Component>
		void removeComponent(Entity& entity);

	private:
		size_t _entitiesToBeCreated;
		std::vector<std::vector<std::pair<Component::Id, std::unique_ptr<IComponentPrefab>>>> _newEntityComponentAddBuffer;
		std::unordered_map<Entity*, std::vector<std::pair<Component::Id, std::unique_ptr<IComponentPrefab>>>> _componentAddBuffer;
		std::unordered_map<Entity*, std::vector<Component::Id>> _componentRemovalBuffer;
	};

	template<typename T>
	template<typename ...Args>
	inline ChangeBuffer::ComponentPrefab<T>::ComponentPrefab(Args&& ...args) :
		_ptr(std::make_unique<T>(args...))
	{}
	template<typename T>
	inline const void* ChangeBuffer::ComponentPrefab<T>::get() const {
		return _ptr.get();
	}

	inline size_t ChangeBuffer::createEntity() {
		_newEntityComponentAddBuffer.emplace_back();
		return _entitiesToBeCreated++;
	}

	template<typename Component, typename ...Args>
	inline void ChangeBuffer::addComponent(Entity& entity, Args&& ... args) {
		auto it = _componentAddBuffer.find(&entity);
		if (it == _componentAddBuffer.end()) {
			_componentAddBuffer.emplace(std::make_pair(entity, std::vector<Component::Id, ComponentPrefab>()));
			it = _componentAddBuffer.find(&entity);
		}

		it->second.emplace_back(args...);
	}
	template<typename Component, typename ...Args>
	inline void ChangeBuffer::addComponent(size_t entity, Args&& ...component) {
		assert(_entitiesToBeCreated > entity);

		auto it = _newEntityComponentAddBuffer.find(entity);
		if (it == _newEntityComponentAddBuffer.end()) {
			_newEntityComponentAddBuffer.emplace(std::make_pair(entity, std::vector<Component::Id, ComponentPrefab>()));
			it = _newEntityComponentAddBuffer.find(entity);
		}

		it->second.emplace_back(args...);
	}
	template<typename Component>
	inline void ChangeBuffer::removeComponent(Entity& entity) {
		auto it = _componentRemovalBuffer.find(&entity);
		if (it == _componentRemovalBuffer.end()) {
			_componentRemovalBuffer.emplace(std::make_pair(entity, std::vector<Component::Id, ComponentPrefab>()));
			it = _componentRemovalBuffer.find(&entity);
		}

		it->second.push_back(ecs::Component::GetId<Component>());
	}
}