#pragma once

#include <functional>
#include <queue>
#include <thread>
#include <mutex>

namespace ecs {

	class JobQueue {

	public:
		void dispatch(size_t threadCount);
		void addJob(const std::function<void(void)>& job);

	private:
		std::queue<std::function<void(void)>> _jobs;
		std::mutex _jobsMutex;
	};

	inline void JobQueue::dispatch(size_t threadCount) {
		std::vector<std::thread> threads;
		threads.reserve(threadCount);

		for (size_t i = 0; i < threadCount; ++i) {
			threads.emplace_back([this]() {
				while (true) {
					std::function<void(void)> job;

					{
						std::lock_guard<std::mutex> lock(_jobsMutex);

						if (_jobs.empty())
							return;

						job = _jobs.front();
					}

					job();
				}
			});
		}

		for (auto& thread : threads)
			thread.join();
	}

	inline void JobQueue::addJob(const std::function<void(void)>& job) {
		_jobs.push(job);
	}

}