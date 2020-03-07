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

		void join();

	private:
		std::queue<std::function<void(void)>> _jobs;
		std::mutex _jobsMutex;

		std::vector<std::thread> threads;
	};

	inline void JobQueue::dispatch(size_t threadCount) {
		threads.reserve(threadCount);

		for (size_t i = 0; i < std::min(threadCount, _jobs.size()); ++i) {
			threads.emplace_back([this]() {
				while (true) {
					std::function<void(void)> job;

					{
						std::lock_guard<std::mutex> lock(_jobsMutex);

						if (_jobs.empty())
							return;

						job = _jobs.front();
						_jobs.pop();
					}

					job();
				}
			});
		}
	}

	inline void JobQueue::addJob(const std::function<void(void)>& job) {
		_jobs.push(job);
	}

	inline void JobQueue::join() {
		for (auto& thread : threads)
			thread.join();
	}

}