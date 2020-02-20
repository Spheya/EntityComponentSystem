#pragma once

#include <vector>
#include <unordered_map>

#include "Entity.hpp"

namespace ecs {
	class ChangeBuffer {
		friend class Engine;

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
		struct EntityPrefab {
			friend class ChangeBuffer;
		private:
			EntityPrefab(size_t id) : id(id) {}
			size_t id;
		};

		EntityPrefab createEntity();

		template<typename Component, typename ... Args>
		void addComponent(Entity::Handle entity, Args&&... component);
		template<typename Component, typename ... Args>
		void addComponent(EntityPrefab entity, Args&&... component);

		template<typename Component>
		void removeComponent(Entity::Handle entity);

	private:
		size_t _entitiesToBeCreated;
		std::vector<std::vector<std::pair<Component::Id, std::unique_ptr<IComponentPrefab>>>> _newEntityComponentAddBuffer;
		std::unordered_map<Entity::Handle, std::vector<std::pair<Component::Id, std::unique_ptr<IComponentPrefab>>>> _componentAddBuffer;
		std::unordered_map<Entity::Handle, std::vector<Component::Id>> _componentRemovalBuffer;
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

	inline ChangeBuffer::EntityPrefab ChangeBuffer::createEntity() {
		_newEntityComponentAddBuffer.emplace_back();
		return EntityPrefab(_entitiesToBeCreated++);
	}

	template<typename Component, typename ...Args>
	inline void ChangeBuffer::addComponent(Entity::Handle entity, Args&& ... args) {
		auto it = _componentAddBuffer.find(entity);
		if (it == _componentAddBuffer.end()) {
			_componentAddBuffer.emplace(std::make_pair(entity, std::vector<std::pair<ecs::Component::Id, std::unique_ptr<IComponentPrefab>>>()));
			it = _componentAddBuffer.find(entity);
		}

		it->second.push_back(std::make_pair(ecs::Component::getId<Component>(), Component(Args...)));
	}
	template<typename Component, typename ...Args>
	inline void ChangeBuffer::addComponent(ChangeBuffer::EntityPrefab entity, Args&& ...component) {
		assert(_entitiesToBeCreated > entity.id);

		_newEntityComponentAddBuffer[entity.id].push_back(std::make_pair(ecs::Component::getId<Component>(), Component(Args...)));
	}
	template<typename Component>
	inline void ChangeBuffer::removeComponent(Entity::Handle entity) {
		auto it = _componentRemovalBuffer.find(entity);
		if (it == _componentRemovalBuffer.end()) {
			_componentRemovalBuffer.emplace(std::make_pair(entity, std::vector<ecs::Component::Id>()));
			it = _componentRemovalBuffer.find(entity);
		}

		it->second.push_back(ecs::Component::getId<Component>());
	}
}