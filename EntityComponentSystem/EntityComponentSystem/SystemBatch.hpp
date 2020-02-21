#pragma once

#include <thread>

#include "System.hpp"

#ifndef ECS_KEY_SIZE
#define ECS_KEY_SIZE 256
#endif

namespace ecs {
	class SystemBatch {
	public:
		void addSystem(ISystem* system);
		void removeSystem(ISystem* system);
		bool containsSystem(ISystem* system);
		bool fitsSystem(const ISystem* system) const;
		void update(float deltatime, std::vector<ChangeBuffer>& changeBuffers);

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

	inline void SystemBatch::update(float deltatime, std::vector<ChangeBuffer>& changeBuffers) {
		size_t threadsPerSystem = (changeBuffers.size() - (_systems.size() - _multiThreadedCount)) / _multiThreadedCount;
		size_t extraThreads = changeBuffers.size() % _systems.size();
		size_t offset = 0;

		std::vector<std::thread> threadsVector;

		// Launch threads
		for (auto& system : _systems) {
			if (system->isMultithreaded()) {
				size_t threads = threadsPerSystem;
				if (extraThreads) {
					++threads;
					--extraThreads;
				}
				system->update(deltatime, changeBuffers.begin() + offset, threadsVector, threads);
			} else {
				system->update(deltatime, changeBuffers.begin() + offset, threadsVector, 1);
			}
		}

		// Join all the threads
		for (auto& thread : threadsVector)
			thread.join();
	}
}