#include "Mesh.hpp"

renderer::Mesh::Mesh(GLenum mode, GLsizei vertexCount) :
	_mode(mode),
	_vertexCount(vertexCount)
{
	glGenVertexArrays(1, &_vao);
}

renderer::Mesh::Mesh(Mesh&& other) noexcept :
	_vao(other._vao),
	_vbos(std::move(other._vbos))
{
	other._vao = 0;
}

renderer::Mesh& renderer::Mesh::operator=(Mesh&& other) noexcept {
	_vao = other._vao;
	_vbos = std::move(other._vbos);
	other._vao = 0;

	return *this;
}

renderer::Mesh::~Mesh() {
	glDeleteVertexArrays(1, &_vao);
}

renderer::Mesh::VboHandle renderer::Mesh::createVbo() {
	glBindVertexArray(_vao);
	_vbos.emplace_back();
	return _vbos.size() - 1;
}

renderer::Vbo& renderer::Mesh::getVbo(VboHandle handle) {
	return _vbos[handle];
}

const renderer::Vbo& renderer::Mesh::getVbo(VboHandle handle) const {
	return _vbos[handle];
}

void renderer::Mesh::bind() {
	glBindVertexArray(_vao);
}

void renderer::Mesh::draw() {
	glDrawArrays(_mode, 0, _vertexCount);
}
