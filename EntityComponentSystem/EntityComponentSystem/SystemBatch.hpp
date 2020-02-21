#pragma once

#include <thread>

#include "System.hpp"
#include "JobQueue.hpp"

#ifndef ECS_KEY_SIZE
#define ECS_KEY_SIZE 256
#endif

#ifndef ENTITIES_PER_THREAD
#define ENTITIES_PER_THREAD 32
#endif

namespace ecs {
	class SystemBatch {
	public:
		void addSystem(ISystem* system);
		void removeSystem(ISystem* system);
		bool containsSystem(ISystem* system);
		bool fitsSystem(const ISystem* system) const;
		void update(float deltatime, ChangeBuffer& changeBuffers);

	private:
		size_t _multiThreadedCount;
		std::vector<ISystem*> _systems;
		std::bitset<ECS_KEY_SIZE> _key;
	};

	inline void SystemBatch::addSystem(ISystem* system) {
		assert(fitsSystem(system));

		if (system->isMultithreaded())
			++_multiThreadedCount;

		_key |= system->getKey();
		_systems.push_back(std::move(system));
	}

	inline void SystemBatch::removeSystem(ISystem* system) {
		assert(containsSystem(system));

		if (system->isMultithreaded())
			--_multiThreadedCount;

		_systems.erase(std::find(_systems.begin(), _systems.end(), system));
	}

	inline bool SystemBatch::containsSystem(ISystem* system) {
		return std::find(_systems.begin(), _systems.end(), system) != _systems.end();
	}

	inline bool SystemBatch::fitsSystem(const ISystem* system) const {
		auto temp = _key;
		temp &= system->getKey();
		return temp.none();
	}

	inline void SystemBatch::update(float deltatime, ChangeBuffer& changeBuffer) {
		JobQueue queue;

		for (auto& system : _systems) {
			if (system->isMultithreaded()) {
				// Add every complete group to the queue
				for (size_t i = ENTITIES_PER_THREAD; i < system->getEntityCount(); i += ENTITIES_PER_THREAD) {
					queue.addJob([&]() {
									system->update(deltatime, changeBuffer, i - ENTITIES_PER_THREAD, ENTITIES_PER_THREAD);
								 });
				}

				// Add the last group of entities as a smaller group
				size_t size = system->getEntityCount() % ENTITIES_PER_THREAD;
				if (size != 0) {
					size_t begin = system->getEntityCount() - size;
					queue.addJob([&]() {
									system->update(deltatime, changeBuffer, begin, size);
								 });
				}
			} else {
				// Add the whole system as a single job when the system cannot be run on multiple threads
				queue.addJob([&]() {
								system->update(deltatime, changeBuffer, 0, system->getEntityCount());
							 });
			}
		}

		queue.dispatch(std::thread::hardware_concurrency());
	}
}