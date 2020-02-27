#include "TexturePack.h"
#include <memory>

renderer::TexturePack::TexturePack(std::initializer_list<std::shared_ptr<const Texture>> textures) :
	m_textures(textures)
{}

GLuint renderer::TexturePack::add(std::shared_ptr<const Texture> texture) {
	m_textures.push_back(std::move(texture));
	return GLuint(m_textures.size()) - 1;
}

void renderer::TexturePack::bind() const {
	for(size_t i = 0; i < m_textures.size(); ++i)
		m_textures[i]->bind(GLuint(i));
}

void renderer::TexturePack::unbind() const {
	for(size_t i = 0; i < m_textures.size(); ++i) {
		m_textures[i]->unbind(GLuint(i));
	}
}
