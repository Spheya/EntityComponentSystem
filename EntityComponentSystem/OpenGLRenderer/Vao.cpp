#include "Vao.hpp"

renderer::Vao::Vao(GLenum mode, GLsizei vertexCount) :
	_mode(mode),
	_vertexCount(vertexCount)
{
	glGenVertexArrays(1, &_vao);
}

renderer::Vao::Vao(Vao&& other) noexcept :
	_vao(other._vao),
	_vbos(std::move(other._vbos)),
	_mode(other._mode),
	_vertexCount(other._vertexCount)
{
	other._vao = 0;
}

renderer::Vao& renderer::Vao::operator=(Vao&& other) noexcept {
	_vao = other._vao;
	_vbos = std::move(other._vbos);
	_mode = other._mode;
	_vertexCount = other._vertexCount;

	other._vao = 0;

	return *this;
}

renderer::Vao::~Vao() {
	glDeleteVertexArrays(1, &_vao);
}

renderer::Vao::VboHandle renderer::Vao::createVbo() {
	glBindVertexArray(_vao);
	_vbos.emplace_back();
	return _vbos.size() - 1;
}

renderer::Vbo& renderer::Vao::getVbo(VboHandle handle) {
	return _vbos[handle];
}

const renderer::Vbo& renderer::Vao::getVbo(VboHandle handle) const {
	return _vbos[handle];
}

void renderer::Vao::bind() {
	glBindVertexArray(_vao);
}

void renderer::Vao::draw() {
	glDrawArrays(_mode, 0, _vertexCount);
}
