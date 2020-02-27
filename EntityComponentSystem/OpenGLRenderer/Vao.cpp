#include "Vao.h"
renderer::Vao::Vao() {
	glGenVertexArrays(1, &m_id);
}

renderer::Vao::Vao(Vao&& other) noexcept :
	m_id(other.m_id),
	m_vbos(std::move(other.m_vbos))
{
	other.m_id = 0;
}

renderer::Vao& renderer::Vao::operator=(Vao&& other) noexcept {
	m_id = other.m_id;
	m_vbos = std::move(other.m_vbos);

	other.m_id = 0;

	return *this;
}

renderer::Vao::~Vao() {
	if(m_id != 0)
		glDeleteVertexArrays(1, &m_id);
}

bool renderer::Vao::isValid() const {
	return m_id == 0;
}

void renderer::Vao::bind() const {
	glBindVertexArray(m_id);
}

void renderer::Vao::unbind() {
	glBindVertexArray(0);
}

void renderer::Vao::prepareDraw() const {
	glBindVertexArray(m_id);

	for(const GLuint attrib : m_attributes)
		glEnableVertexAttribArray(attrib);
}

void renderer::Vao::finishDraw() const {
	for (const GLuint attrib : m_attributes)
		glDisableVertexAttribArray(attrib);

	glBindVertexArray(0);
}
