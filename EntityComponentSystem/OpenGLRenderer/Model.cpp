#include "Model.hpp"

renderer::Model::Model(size_t nVertices, GLenum drawMode) :
	_nVertices(nVertices),
	_drawMode(drawMode)
{}

void renderer::Model::bindVao() const {
	_vao.bind();
}

void renderer::Model::prepareDraw() const {
	_vao.prepareDraw();
}

void renderer::Model::draw() const {
	glDrawArrays(_drawMode, 0, GLsizei(_nVertices));
}

void renderer::Model::finishDraw() const {
	_vao.finishDraw();
}

void renderer::Model::setDrawMode(GLenum drawMode) {
	_drawMode = drawMode;
}

GLenum renderer::Model::getDrawMode() const {
	return _drawMode;
}

void renderer::Model::setVerticesAmount(size_t nVertices) {
	_nVertices = nVertices;
}

size_t renderer::Model::getVerticesAmount() const {
	return _nVertices;
}
