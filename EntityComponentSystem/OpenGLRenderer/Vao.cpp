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

void renderer::Vao::setIndexBuffer(std::vector<GLuint> indices) {
	_vertexCount = indices.size();

	glBindVertexArray(_vao);
	if (_indexBuffer)
		glGenBuffers(1, &_indexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	_usesIndexBuffer = true;
}

void renderer::Vao::bind() {
	glBindVertexArray(_vao);
}

void renderer::Vao::draw() {
	if (!_usesIndexBuffer) {
		glDrawArrays(_mode, 0, _vertexCount);
	} else {
		glDrawElements(_mode, _vertexCount, GL_UNSIGNED_INT, (void*) 0);
	}
}

GLsizei renderer::Vao::getVertexCount() {
	return _vertexCount;
}

void renderer::Vao::setVertexCount(GLsizei count) {
	_vertexCount = count;
}

GLenum renderer::Vao::getDrawMode() {
	return _mode;
}

void renderer::Vao::setDrawMode(GLenum mode) {
	_mode = mode;
}
