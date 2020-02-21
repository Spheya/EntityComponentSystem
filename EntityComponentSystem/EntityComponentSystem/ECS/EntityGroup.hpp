#pragma once

#include <vector>

namespace ecs {
	template<typename Entity>
	class EntityGroup {
	public:
		EntityGroup(const typename std::vector<Entity>::iterator& begin, const typename std::vector<Entity>::iterator& end) :
			_begin(begin),
			_end(end) {}

		typename std::vector<Entity>::iterator begin() const {
			return _begin;
		};
		typename std::vector<Entity>::iterator end() const {
			return _end;
		};

	private:
		typename std::vector<Entity>::iterator _begin;
		typename std::vector<Entity>::iterator _end;
	};
};