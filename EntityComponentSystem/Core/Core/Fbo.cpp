#include "Fbo.hpp"

#include <cassert>
#include <iostream>

renderer::Fbo::Fbo(int width, int height) :
	_width(width),
	_height(height)
{
	glGenFramebuffers(1, &_id);

	bind();

	_depthBuffer = generateTexture(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);

	unbind();
}

renderer::Fbo::~Fbo() {
	glDeleteFramebuffers(1, &_id);
}

void renderer::Fbo::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void renderer::Fbo::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

size_t renderer::Fbo::createTexture() {
	_textures.push_back(generateTexture(GL_COLOR_ATTACHMENT0 + _textures.size(), GL_RGB));
	return _textures.size() - 1;
}

renderer::Texture* renderer::Fbo::getDepthBuffer() {
	return _depthBuffer.get();
}

renderer::Texture* renderer::Fbo::getTexture(size_t handle) {
	return _textures[handle].get();
}

std::unique_ptr<renderer::Texture> renderer::Fbo::generateTexture(GLenum type, GLenum format) {

	assert(glGetError() == GL_NO_ERROR);
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	assert(glGetError() == GL_NO_ERROR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, tex, 0);

	assert(glGetError() == GL_NO_ERROR);
	auto texture = std::make_unique<Texture>();
	texture->_width = _width;
	texture->_height = _height;
	texture->_type = GL_TEXTURE_2D;
	texture->_channels = 3;
	texture->_textureId = tex;

	assert(glGetError() == GL_NO_ERROR);
	return std::move(texture);
}
