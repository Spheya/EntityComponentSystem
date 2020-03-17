#include "Texture.hpp"
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

renderer::Texture::~Texture() {
	if (isValid())
		glDeleteTextures(1, &_textureId);
}

void renderer::Texture::load() {
	unsigned char* data = stbi_load(getPath().c_str(), &_width, &_height, &_channels, 0);

	loadFromMemory(data, _channels, _width, _height, GL_LINEAR);

	stbi_image_free(data);
}

void renderer::Texture::loadFromMemory(const unsigned char* data, int channels, int width, int height, GLenum filter) {
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void renderer::Texture::loadFromMemory(const unsigned char* data, int channels, int width, int height, int depth, GLenum filter) {
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

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void renderer::Texture::setFilter(GLenum filter) {
	glBindTexture(_type, _textureId);
	glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, filter);
}

void renderer::Texture::bind(unsigned slot) const {
	assert(isValid());
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(_type, _textureId);
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
