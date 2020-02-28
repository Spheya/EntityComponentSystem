#pragma once

#include <vector>
#include <memory>

#include "MemoryPoolAllocator.hpp"

#ifndef COMPONENT_POOL_SIZE
#define COMPONENT_POOL_SIZE 512
#endif

namespace ecs {
	class Component final {
	public:
		using Id = uint8_t;

		static std::unique_ptr<IMemoryPoolAllocator> createAllocator(Id id) {
			return ALLOCATORS[id]->copy();
		}

		template<typename T>
		static Id getId() {
			const static Component::Id id = _getId();
			static bool allocatorBuilt = false;
			if (!allocatorBuilt) {
				buildAllocatorPrototype<T>();
				allocatorBuilt = true;
			}

			return id;
		}

	private:
		static std::vector<std::unique_ptr<IMemoryPoolAllocator>> ALLOCATORS;

		static Id _getId() {
			static Id id = 0;
			return id++;
		}

		template<typename T>
		static void buildAllocatorPrototype() {
			ALLOCATORS.push_back(std::make_unique<MemoryPoolAllocator<T>>(COMPONENT_POOL_SIZE));
		}
	};

	inline std::vector<std::unique_ptr<IMemoryPoolAllocator>> Component::ALLOCATORS;
}