#pragma once

#include <typeindex>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <filesystem>

#include "Resource.hpp"

class ResourceManager {
public:
	static ResourceManager& getInstance();

	template<typename T, typename = typename std::enable_if<std::is_base_of<Resource, T>::value, T>::type>
	void load(std::string file);

	template<typename T, typename = typename std::enable_if<std::is_base_of<Resource, T>::value, T>::type>
	void unload(std::string file);

	// TODO: make this return a const ptr when all functionality is added
	template<typename T, typename = typename std::enable_if<std::is_base_of<Resource, T>::value, T>::type>
	T* get(std::string name);

	void unloadAll();

private:
	ResourceManager() = default;

	static std::unique_ptr<ResourceManager> instance;

	std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<Resource>>> _resources;
};

template<typename T, typename>
inline void ResourceManager::load(std::string file) {
	// Convert file path to absolute path
	file = std::filesystem::absolute(std::filesystem::path(file)).u8string();

	// Check if the resourcemanager hasn't loaded this resource already
	auto id = std::type_index(typeid(T));
	assert(_resources[id].find(file) == _resources[id].end());

	// Load the resource
	std::unique_ptr<Resource> resource = std::make_unique<T>(file);
	resource->load();
	_resources[id][file] = std::move(resource);
}

template<typename T, typename>
inline void ResourceManager::unload(std::string file) {
	// Convert file path to absolute path
	file = std::filesystem::absolute(std::filesystem::path(file)).u8string();

	// Make sure the resource is loaded
	auto id = std::type_index(typeid(T));
	assert(_resources[id].find(file) != _resources[id].end());

	// Unload the resource
	_resources[id].erase(file);
}

template<typename T, typename>
inline T* ResourceManager::get(std::string file) {
	// Convert file path to absolute path
	file = std::filesystem::absolute(std::filesystem::path(file)).u8string();

	// Get the location of the resource in memory
	auto id = std::type_index(typeid(T));
	auto it = _resources[id].find(file);
	
	// Load the resource if it wasn't loaded already
	if (it == _resources[id].end()) {
		load<T>(file);
		it = _resources[id].find(file);
	}

	return reinterpret_cast<T*>(it->second.get());
}
