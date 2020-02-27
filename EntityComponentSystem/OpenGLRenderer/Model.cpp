#include "Model.h"

renderer::Model::Model(size_t nVertices, GLenum drawMode) :
	m_nVertices(nVertices),
	m_drawMode(drawMode)
{}

void renderer::Model::bindVao() const {
	m_vao.bind();
}

void renderer::Model::prepareDraw() const {
	m_vao.prepareDraw();
}

void renderer::Model::draw() const {
	glDrawArrays(m_drawMode, 0, GLsizei(m_nVertices));
}

void renderer::Model::finishDraw() const {
	m_vao.finishDraw();
}

void renderer::Model::setDrawMode(GLenum drawMode) {
	m_drawMode = drawMode;
}

GLenum renderer::Model::getDrawMode() const {
	return m_drawMode;
}

void renderer::Model::setVerticesAmount(size_t nVertices) {
	m_nVertices = nVertices;
}

size_t renderer::Model::getVerticesAmount() const {
	return m_nVertices;
}
