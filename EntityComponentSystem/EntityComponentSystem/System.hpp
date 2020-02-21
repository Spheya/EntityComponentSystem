#pragma once

#include <tuple>
#include <cassert>
#include <algorithm>
#include <thread>

#include "Entity.hpp"
#include "EntityGroup.hpp"
#include "ChangeBuffer.hpp"

#ifndef ECS_KEY_SIZE
#define ECS_KEY_SIZE 256
#endif

namespace ecs {
	class ISystem {
	public:
		ISystem() = default;
		ISystem(ISystem&) = default;
		ISystem& operator=(ISystem&) = default;
		ISystem(ISystem&&) = default;
		ISystem& operator=(ISystem&&) = default;
		virtual ~ISystem() = default;

		virtual bool containsEntity(const Entity& entity) const = 0;
		virtual bool fitsEntity(const Entity& entity) const = 0;
		virtual void addEntity(Entity& entity) = 0;
		virtual void removeEntity(Entity& entity) = 0;
		virtual const std::bitset<ECS_KEY_SIZE>& getKey() const = 0;
		virtual bool isMultithreaded() const = 0;

		virtual void update(float deltatime, std::vector<ChangeBuffer>::iterator changeBuffer, std::vector<std::thread>& threads, size_t nThreads) = 0;
	};

	template<typename ... Components>
	class System : public ISystem {
	public:
		class EntityData {
			friend class System;
		private:
			EntityData(Entity::Handle handle) : _handle(handle) {};

			Entity::Handle _handle;
			std::tuple<Components*...> _components;

		public:
			template<typename T>
			T* getComponent() { return std::get<T*>(_components); }
			template<typename T>
			const T* getComponent() const { return std::get<T*>(_components); }
			Entity::Handle getHandle() const { return _handle; }
		};

		explicit System(bool multithreaded);

		bool containsEntity(const Entity& entity) const override;
		bool fitsEntity(const Entity& entity) const override;
		void addEntity(Entity& entity) override;
		void removeEntity(Entity& entity) override;
		const std::bitset<ECS_KEY_SIZE>& getKey() const override;
		bool isMultithreaded() const override;

		void update(float deltatime, std::vector<ChangeBuffer>::iterator changeBuffer, std::vector<std::thread>& threads, size_t nThreads) override;

		virtual void onAdd(Entity& entity);
		virtual void onRemove(Entity& entity);
		virtual void onUpdate(float deltatime, const EntityGroup<EntityData>& entityGroup, ChangeBuffer& changeBuffer) = 0;

	protected:
		using Base = System<Components...>;

		const std::vector<EntityData>& getEntities() const;

	private:
		std::bitset<ECS_KEY_SIZE> _key;
		std::vector<EntityData> _entities;
		bool _multithreaded;

		template<typename First, typename Second, typename ... T>
		void buildKey();

		template<typename Last>
		void buildKey();

		template<typename First, typename Second, typename ... T>
		void putComponentsInEntityData(Entity& entity, EntityData& data);

		template<typename Last>
		void putComponentsInEntityData(Entity& entity, EntityData& data);
	};

	template<typename ... Components>
	inline System<Components...>::System(bool multithreaded) :
		_multithreaded(multithreaded)
	{
		buildKey<Components...>();
	}

	template<typename ... Components>
	inline bool System<Components...>::containsEntity(const Entity& entity) const {
		return std::find_if(_entities.begin(), _entities.end(), [&](const EntityData& data) { return data.getHandle() == entity.getHandle(); }) != _entities.end();
	}

	template<typename ... Components>
	inline bool System<Components...>::fitsEntity(const Entity& entity) const {
		auto check = _key;
		check &= entity.getKey();
		return check == _key;
	}

	template<typename ... Components>
	inline void System<Components...>::addEntity(Entity& entity) {
		assert(fitsEntity(entity));
		assert(!containsEntity(entity));
		EntityData data(entity.getHandle());
		putComponentsInEntityData<Components...>(entity, data);
		_entities.insert(std::upper_bound(_entities.begin(), _entities.end(), data, [](const EntityData& data1, const EntityData& data2) {
			return std::get<0>(data1._components) < std::get<0>(data2._components);
		}), data);
		onAdd(entity);
	}

	template<typename ... Components>
	inline void System<Components...>::removeEntity(Entity& entity) {
		assert(containsEntity(entity));
		_entities.erase(std::find_if(_entities.begin(), _entities.end(), [&](const EntityData& data) {
			return data.getHandle() == entity.getHandle();
		}));
		onRemove(entity);
	}

	template<typename ...Components>
	inline const std::bitset<ECS_KEY_SIZE>& System<Components...>::getKey() const {
		return _key;
	}

	template<typename ...Components>
	inline bool System<Components...>::isMultithreaded() const {
		return _multithreaded;
	}

	template<typename ... Components>
	inline void System<Components...>::update(float deltatime, std::vector<ChangeBuffer>::iterator changeBuffer, std::vector<std::thread>& threads, size_t nThreads) {
		const size_t entitiesPerThread = _entities.size() / nThreads;
		size_t begin = 0;
		size_t count = 0;
		
		for (size_t i = 0; i < nThreads - 1; ++i) {
			begin += entitiesPerThread;
			threads.emplace_back(
				[=]() {
					onUpdate(deltatime, 
							 EntityGroup<EntityData>(_entities.begin() + begin, _entities.begin() + begin + entitiesPerThread), 
							 *(changeBuffer + count));
				}
			);
			++count;
		}

		threads.emplace_back(
			[=]() {
				onUpdate(deltatime,
						 EntityGroup<EntityData>(_entities.begin() + begin, _entities.end()),
						 *(changeBuffer + count));
			}
		);
	}

	template<typename ... Components>
	inline void System<Components...>::onAdd(Entity&) {}

	template<typename ... Components>
	inline void System<Components...>::onRemove(Entity&) {}

	template<typename ... Components>
	inline const std::vector<typename ecs::System<Components...>::EntityData>& System<Components...>::getEntities() const {
		return _entities;
	}

	template<typename ... Components>
	template<typename First, typename Second, typename ... T>
	inline void System<Components...>::buildKey() {
		static Component::Id id = Component::getId<First>();
		_key.set(id, true);
		buildKey<Second, T...>();
	}

	template<typename ... Components>
	template<typename Last>
	inline void System<Components...>::buildKey() {
		static Component::Id id = Component::getId<Last>();
		_key.set(id, true);
	}

	template<typename ... Components>
	template<typename First, typename Second, typename ... T>
	inline void System<Components...>::putComponentsInEntityData(Entity& entity, EntityData& data) {
		std::get<First*>(data._components) = entity.getComponent<First>();
		putComponentsInEntityData<Second, T...>(entity, data);
	}

	template<typename ... Components>
	template<typename Last>
	inline void System<Components...>::putComponentsInEntityData(Entity& entity, EntityData& data) {
		std::get<Last*>(data._components) = entity.getComponent<Last>();
	}
}
