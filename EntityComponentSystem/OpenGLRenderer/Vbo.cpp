#include "Vbo.hpp"

renderer::Vbo::Vbo() {
	glGenBuffers(1, &_id);
}

renderer::Vbo::Vbo(Vbo&& other) noexcept :
	_id(other._id),
	_capacaty(other._capacaty)
{
	other._id = 0;
}

renderer::Vbo& renderer::Vbo::operator=(Vbo&& other) noexcept {
	_id = other._id;
	_capacaty = other._capacaty;
	other._id = 0;

	return *this;
}

renderer::Vbo::~Vbo() {
	glDeleteBuffers(1, &_id);
}

void renderer::Vbo::write(size_t length, const void* data) {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);
}