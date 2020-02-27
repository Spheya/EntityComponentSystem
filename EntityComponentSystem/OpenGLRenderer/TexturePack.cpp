#include "TexturePack.hpp"
#include <memory>

renderer::TexturePack::TexturePack(std::initializer_list<std::shared_ptr<const Texture>> textures) :
	_textures(textures)
{}

GLuint renderer::TexturePack::add(std::shared_ptr<const Texture> texture) {
	_textures.push_back(std::move(texture));
	return GLuint(_textures.size()) - 1;
}

void renderer::TexturePack::bind() const {
	for(size_t i = 0; i < _textures.size(); ++i)
		_textures[i]->bind(GLuint(i));
}

void renderer::TexturePack::unbind() const {
	for(size_t i = 0; i < _textures.size(); ++i) {
		_textures[i]->unbind(GLuint(i));
	}
}
