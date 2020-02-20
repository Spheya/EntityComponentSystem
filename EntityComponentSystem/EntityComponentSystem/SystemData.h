#pragma once

#include <vector>

namespace ecs {
	template<typename Entity>
	class EntityGroup {
	public:
		EntityGroup(const std::vector<Entity>::iterator& begin, const std::vector<Entity>::iterator& end) :
			_begin(begin),
			_end(end) {}

		std::vector<Entity>::iterator begin() {
			return _begin;
		};
		std::vector<Entity>::iterator end() {
			return _end;
		};

	private:
		std::vector<Entity>::iterator _begin;
		std::vector<Entity>::iterator _end;
	};
};