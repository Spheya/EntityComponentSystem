#pragma once

#include <typeindex>
#include <memory>
#include <unordered_map>
#include <cassert>

#include "Resource.hpp"

class ResourceManager {
public:
	static ResourceManager& getInstance();

	template<typename T>
	void load(std::string file);

	template<typename T>
	void unload(std::string file);

	template<typename T>
	T* get(std::string name);

private:
	ResourceManager() = default;

	static std::unique_ptr<ResourceManager> instance;

	std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<Resource>>> _resources;
};

template<typename T>
inline void ResourceManager::load(std::string file) {
	auto id = std::type_index(typeid(T));

	assert(_resources[id].find(file) == _resources[id].end());

	std::unique_ptr<Resource> resource = std::make_unique<T>(file);
	resource->load();

	_resources[id][file] = std::move(resource);
}

template<typename T>
inline void ResourceManager::unload(std::string file) {
	auto id = std::type_index(typeid(T));

	assert(_resources[id].find(file) != _resources[id].end());

	_resources[id].erase(file);
}

template<typename T>
inline T* ResourceManager::get(std::string file) {
	auto id = std::type_index(typeid(T));
	auto it = _resources[id].find(file);
	
	if (it == _resources[id].end()) {
		load<T>(file);
		it = _resources[id].find(file);
	}

	return reinterpret_cast<T*>(it->second.get());
}
