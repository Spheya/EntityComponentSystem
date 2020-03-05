#pragma once

#include <vector>
#include <unordered_map>
#include <mutex>

#include "Entity.hpp"

namespace ecs {
	class ChangeBuffer {
		friend class Engine;

		class IComponentPrefab {
		public:
			IComponentPrefab() = default;
			IComponentPrefab(const IComponentPrefab&) = default;
			IComponentPrefab& operator=(const IComponentPrefab&) = default;
			IComponentPrefab(IComponentPrefab&&) = default;
			IComponentPrefab& operator=(IComponentPrefab&&) = default;
			virtual ~IComponentPrefab() = default;

			virtual void* get() const = 0;
		};

		template<typename T>
		class ComponentPrefab : IComponentPrefab {
		public:
			template<typename ... Args>
			ComponentPrefab(Args&&... args);

			void* get() const override;

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

		ChangeBuffer() = default;
		ChangeBuffer(const ChangeBuffer&) = default;
		ChangeBuffer& operator=(const ChangeBuffer&) = default;
		ChangeBuffer(ChangeBuffer&&) = default;
		ChangeBuffer& operator=(ChangeBuffer&&) = default;
		~ChangeBuffer();

		EntityPrefab createEntity();

		template<typename Component, typename ... Args>
		void addComponent(Entity::Handle entity, Args&&... component);
		template<typename Component, typename ... Args>
		void addComponent(EntityPrefab entity, Args&&... component);

		template<typename Component>
		void removeComponent(Entity::Handle entity);

	private:

		std::mutex _createEntityMutex;
		std::mutex _addNewEntityComponentMutex;
		std::mutex _addComponentMutex;
		std::mutex _removeComponentMutex;

		size_t _entitiesToBeCreated = 0;
		std::vector<std::vector<std::pair<Component::Id, IComponentPrefab*>>> _newEntityComponentAddBuffer;
		std::unordered_map<Entity::Handle, std::vector<std::pair<Component::Id, IComponentPrefab*>>> _componentAddBuffer;
		std::unordered_map<Entity::Handle, std::vector<Component::Id>> _componentRemovalBuffer;
	};

	template<typename T>
	template<typename ...Args>
	inline ChangeBuffer::ComponentPrefab<T>::ComponentPrefab(Args&& ...args) :
		_ptr(std::make_unique<T>(std::forward<Args>(args)...))
	{}

	template<typename T>
	inline void* ChangeBuffer::ComponentPrefab<T>::get() const {
		return _ptr.get();
	}

	inline ChangeBuffer::~ChangeBuffer() {
		for (auto& vec : _newEntityComponentAddBuffer)
			for (auto& component : vec)
				delete component.second;

		for (auto& vec : _componentAddBuffer)
			for (auto& component : vec.second)
				delete component.second;
	}

	inline ChangeBuffer::EntityPrefab ChangeBuffer::createEntity() {
		std::lock_guard<std::mutex> guard(_createEntityMutex);

		_newEntityComponentAddBuffer.emplace_back();
		return EntityPrefab(_entitiesToBeCreated++);
	}

	template<typename Component, typename ...Args>
	inline void ChangeBuffer::addComponent(Entity::Handle entity, Args&& ... args) {
		std::lock_guard<std::mutex> guard(_addNewEntityComponentMutex);

		auto it = _componentAddBuffer.find(entity);
		if (it == _componentAddBuffer.end()) {
			_componentAddBuffer.emplace(std::make_pair(entity, std::vector<std::pair<ecs::Component::Id, IComponentPrefab*>>()));
			it = _componentAddBuffer.find(entity);
		}

		it->second.push_back(std::make_pair(ecs::Component::getId<Component>(), new ComponentPrefab<Component>(Args...)));
	}
	template<typename Component, typename ...Args>
	inline void ChangeBuffer::addComponent(ChangeBuffer::EntityPrefab entity, Args&& ... args) {
		std::lock_guard<std::mutex> guard(_addComponentMutex);

		_newEntityComponentAddBuffer[entity.id].push_back(std::make_pair(ecs::Component::getId<Component>(), (IComponentPrefab*)new ComponentPrefab<Component>(std::forward<Args>(args)...)));
	}
	template<typename Component>
	inline void ChangeBuffer::removeComponent(Entity::Handle entity) {
		std::lock_guard<std::mutex> guard(_removeComponentMutex);

		auto it = _componentRemovalBuffer.find(entity);
		if (it == _componentRemovalBuffer.end()) {
			_componentRemovalBuffer.emplace(std::make_pair(entity, std::vector<ecs::Component::Id>()));
			it = _componentRemovalBuffer.find(entity);
		}

		it->second.push_back(ecs::Component::getId<Component>());
	}
}