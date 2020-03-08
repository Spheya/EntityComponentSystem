#pragma once

#include <vector>
#include <typeindex>

namespace ecs {

	class SystemDependencies {
	public:
		template<typename T, typename ... Deps>
		static SystemDependencies create();

		static SystemDependencies create();

		const std::vector<std::type_index>& getDependencies() const;

		bool empty() const;

	private:
		SystemDependencies() = default;

		std::vector<std::type_index> _deps;

		template<typename First, typename Second, typename ... Deps>
		void processDependencies();

		template<typename Last>
		void processDependencies();

	};

	template<typename T, typename ... Deps>
	inline SystemDependencies SystemDependencies::create() {
		SystemDependencies result;
		result._deps.reserve(sizeof...(Deps) + 1);
		result.processDependencies<T, Deps...>();
		return result;
	}

	inline SystemDependencies SystemDependencies::create() {
		return SystemDependencies();
	}

	inline const std::vector<std::type_index>& SystemDependencies::getDependencies() const {
		return _deps;
	}

	inline bool ecs::SystemDependencies::empty() const {
		return _deps.empty();
	}

	template<typename First, typename Second, typename ... Deps>
	inline void SystemDependencies::processDependencies() {
		_deps.push_back(std::type_index(typeid(First)));
		processDependencies<Second, Deps...>();
	}

	template<typename Last>
	inline void SystemDependencies::processDependencies() {
		_deps.push_back(std::type_index(typeid(Last)));
	}
}