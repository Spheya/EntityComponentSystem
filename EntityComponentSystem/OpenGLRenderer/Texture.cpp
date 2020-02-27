#include "Texture.hpp"
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


renderer::Texture::Texture(const std::string& fileName) {
	loadFromFile(fileName);
}

renderer::Texture::Texture(Texture&& other) noexcept {
	_textureId = other._textureId;
	_type = other._type;
	_width = other._width;
	_height = other._height;
	_channels = other._channels;
	other._textureId = 0;
}

renderer::Texture& renderer::Texture::operator=(Texture&& other) noexcept {
	_textureId = other._textureId;
	_type = other._type;
	_width = other._width;
	_height = other._height;
	_channels = other._channels;
	other._textureId = 0;

	return *this;
}

renderer::Texture::~Texture() {
	if (isValid())
		glDeleteTextures(1, &_textureId);
}

void renderer::Texture::loadFromFile(const std::string& fileName) {
	unsigned char* data = stbi_load(fileName.c_str(), &_width, &_height, &_channels, 0);

	loadFromMemory(data, _channels, _width, _height);

	stbi_image_free(data);
}

void renderer::Texture::loadFromMemory(const unsigned char* data, int channels, int width, int height) {
	if (_textureId == 0)
		glGenTextures(1, &_textureId);

	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Pick an OpenGL format
	GLenum format = 0;
	switch(channels) {
		case 1: format = GL_RED; break;
		case 2: format = GL_RG; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		default: std::terminate(); // Not a valid amount of channels
	}


	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	_type = GL_TEXTURE_2D;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void renderer::Texture::loadFromMemory(const unsigned char* data, int channels, int width, int height, int depth) {
	if (_textureId == 0)
		glGenTextures(1, &_textureId);

	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Pick an OpenGL format
	GLenum format = 0;
	switch (channels) {
	case 1: format = GL_RED; break;
	case 2: format = GL_RG; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: std::terminate(); // Not a valid amount of channels
	}


	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage3D(
		GL_TEXTURE_3D,
		0,
		format,
		width,
		height,
		depth,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	_type = GL_TEXTURE_3D;

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void renderer::Texture::bind(unsigned slot) const {
	assert(isValid());
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(_type, _textureId);
}

void renderer::Texture::unbind(unsigned slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(_type, 0);
}

unsigned renderer::Texture::getWidth() const {
	assert(isValid());
	return _width;
}

unsigned renderer::Texture::getHeight() const {
	assert(isValid());
	return _height;
}

GLenum renderer::Texture::getType() const {
	assert(isValid());
	return _type;
}

bool renderer::Texture::isValid() const {
	return _textureId != 0;
}
