#pragma once

#include <tuple>
#include <cassert>
#include <algorithm>

#include "Entity.hpp"

#ifndef ECS_KEY_SIZE
#define ECS_KEY_SIZE 256
#endif

namespace ecs {
	class ISystem {
	public:
		virtual bool containsEntity(const Entity& entity) const = 0;
		virtual bool fitsEntity(const Entity& entity) const = 0;
		virtual void addEntity(Entity& entity) = 0;
		virtual void removeEntity(Entity& entity) = 0;

		virtual void update() = 0;
	};

	template<typename ... Components>
	class System : public ISystem {
	public:
		struct EntityData {
			friend class System;
		private:
			EntityData(Entity::Handle handle) : handle(handle) {};

			Entity::Handle handle;
			std::tuple<Components*...> components;
		};

		System();

		bool containsEntity(const Entity& entity) const override;
		bool fitsEntity(const Entity& entity) const override;
		void addEntity(Entity& entity) override;
		void removeEntity(Entity& entity) override;

		virtual void onAdd(Entity& entity);
		virtual void onRemove(Entity& entity);

	protected:
		const std::vector<EntityData>& getEntities() const;

	private:
		std::bitset<ECS_KEY_SIZE> _key;
		std::vector<EntityData> _entities;

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
	inline System<Components...>::System() {
		buildKey<Components...>();
	}

	template<typename ... Components>
	inline bool System<Components...>::containsEntity(const Entity& entity) const {
		return std::find_if(_entities.begin(), _entities.end(), [&](const EntityData& data) { return data.handle == entity.getHandle(); }) != _entities.end();
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
			return std::get<0>(data1.components) < std::get<0>(data2.components);
		}), data);
		onAdd(entity);
	}

	template<typename ... Components>
	inline void System<Components...>::removeEntity(Entity& entity) {
		assert(containsEntity(entity));
		_entities.erase(std::find_if(_entities.begin(), _entities.end(), [&](const EntityData& data) {
			return data.handle == entity.getHandle();
		}));
		onRemove(entity);
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
		static Component::Id id = Component::GetId<First>();
		_key.set(id, true);
		buildKey<Second, T...>();
	}

	template<typename ... Components>
	template<typename Last>
	inline void System<Components...>::buildKey() {
		static Component::Id id = Component::GetId<Last>();
		_key.set(id, true);
	}

	template<typename ... Components>
	template<typename First, typename Second, typename ... T>
	inline void System<Components...>::putComponentsInEntityData(Entity& entity, EntityData& data) {
		std::get<First*>(data.components) = entity.getComponent<First>();
		putComponentsInEntityData<Second, T...>(entity, data);
	}

	template<typename ... Components>
	template<typename Last>
	inline void System<Components...>::putComponentsInEntityData(Entity& entity, EntityData& data) {
		std::get<Last*>(data.components) = entity.getComponent<Last>();
	}
}
