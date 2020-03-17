#include "ResourceManager.hpp"

std::unique_ptr<ResourceManager> ResourceManager::instance;

ResourceManager& ResourceManager::getInstance() {
	if (!instance)
		instance.reset(new ResourceManager());
	return *instance;
}

void ResourceManager::unloadAll() {
	_resources.clear();
}
