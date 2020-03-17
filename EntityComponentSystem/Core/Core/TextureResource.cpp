#include "TextureResource.hpp"

void renderer::TextureResource::load() {
	_texture = std::make_unique<renderer::Texture>();
	_texture->loadFromFile(getPath());
}

renderer::Texture* renderer::TextureResource::getTexture() {
	return _texture.get();
}

const renderer::Texture* renderer::TextureResource::getTexture() const {
	return _texture.get();
}
