#include "Material.hpp"

renderer::MaterialTexture::MaterialTexture(std::shared_ptr<Texture> texture) {
	_texture = texture;
}

renderer::MaterialTexture::MaterialTexture(float value) {
	_texture = std::make_shared<Texture>();
	const uint8_t byteVal = uint8_t(value * 255.0f);
	_texture->loadFromMemory(&byteVal, 1, 1, 1);
}

renderer::MaterialTexture::MaterialTexture(const glm::vec3& value) {
	_texture = std::make_shared<Texture>();
	const uint8_t byteVal[] = { uint8_t(value.x * 255.0f), uint8_t(value.y * 255.0f), uint8_t(value.z * 255.0f) };
	_texture->loadFromMemory(&byteVal[0], 3, 1, 1);
}

const std::shared_ptr<renderer::Texture>& renderer::MaterialTexture::getTexture() const {
	return _texture;
}
