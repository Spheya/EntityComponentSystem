#include "Material.hpp"

renderer::MaterialTexture::MaterialTexture(const Texture* texture) :
	_texture(texture)
{}

renderer::MaterialTexture::MaterialTexture(float value) :
	_textureOwner(true)
{
	auto tex = new Texture();
	const uint8_t byteVal = uint8_t(value * 255.0f);
	tex->loadFromMemory(&byteVal, 1, 1, 1);
	_texture = tex;
}

renderer::MaterialTexture::MaterialTexture(const glm::vec3& value) {
	auto tex = new Texture();
	const uint8_t byteVal[] = { uint8_t(value.x * 255.0f), uint8_t(value.y * 255.0f), uint8_t(value.z * 255.0f) };
	tex->loadFromMemory(&byteVal[0], 3, 1, 1);
	_texture = tex;
}

renderer::MaterialTexture::MaterialTexture(MaterialTexture&& other) :
	_texture(other._texture),
	_textureOwner(other._textureOwner)
{
	other._texture = nullptr;
	other._textureOwner = false;
}

renderer::MaterialTexture& renderer::MaterialTexture::operator=(MaterialTexture&& other) {
	_texture = other._texture;
	_textureOwner = other._textureOwner;
	other._texture = nullptr;
	other._textureOwner = false;

	return *this;
}

renderer::MaterialTexture::~MaterialTexture() {
	if (_textureOwner)
		delete _texture;
}

const renderer::Texture* renderer::MaterialTexture::getTexture() const {
	return _texture;
}
