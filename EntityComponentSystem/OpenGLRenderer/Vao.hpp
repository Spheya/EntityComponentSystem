#pragma once

#include <GL/glew.h>

#include <vector>

#include "Vbo.hpp"

namespace renderer {
	class Vao {
	public:
		using VboHandle = size_t;

		Vao(GLenum mode, GLsizei vertexCount);
		Vao(const Vao&) = delete;
		Vao& operator=(const Vao&) = delete;
		Vao(Vao&& other) noexcept;
		Vao& operator=(Vao&& other) noexcept;
		~Vao();

		VboHandle createVbo();
		Vbo& getVbo(VboHandle handle);
		const Vbo& getVbo(VboHandle handle) const;

		void setIndexBuffer(std::vector<GLuint> indices);

		void bind();
		void draw();

		GLsizei getVertexCount();
		void setVertexCount(GLsizei count);

		GLenum getDrawMode();
		void setDrawMode(GLenum mode);

	private:
		GLuint _vao;
		GLuint _indexBuffer = 0;
		std::vector<Vbo> _vbos;

		bool _usesIndexBuffer = false;

		GLenum _mode;
		GLsizei _vertexCount;
	};
}