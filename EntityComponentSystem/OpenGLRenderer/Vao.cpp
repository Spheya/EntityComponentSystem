#include "Vao.hpp"
renderer::Vao::Vao() {
	glGenVertexArrays(1, &_id);
}

renderer::Vao::Vao(Vao&& other) noexcept :
	_id(other._id),
	_vbos(std::move(other._vbos))
{
	other._id = 0;
}

renderer::Vao& renderer::Vao::operator=(Vao&& other) noexcept {
	_id = other._id;
	_vbos = std::move(other._vbos);

	other._id = 0;

	return *this;
}

renderer::Vao::~Vao() {
	if(_id != 0)
		glDeleteVertexArrays(1, &_id);
}

bool renderer::Vao::isValid() const {
	return _id == 0;
}

void renderer::Vao::bind() const {
	glBindVertexArray(_id);
}

void renderer::Vao::unbind() {
	glBindVertexArray(0);
}

void renderer::Vao::prepareDraw() const {
	glBindVertexArray(_id);

	for(const GLuint attrib : _attributes)
		glEnableVertexAttribArray(attrib);
}

void renderer::Vao::finishDraw() const {
	for (const GLuint attrib : _attributes)
		glDisableVertexAttribArray(attrib);

	glBindVertexArray(0);
}
