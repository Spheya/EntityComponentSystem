#pragma once

#include <GL/glew.h>

#include <vector>

#include "Vbo.hpp"

namespace renderer {
	class Mesh {
	public:
		using VboHandle = size_t;

		Mesh(GLenum mode, GLsizei vertexCount);
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		VboHandle createVbo();

		Vbo& getVbo(VboHandle handle);
		const Vbo& getVbo(VboHandle handle) const;

		void bind();

		void draw();

	private:
		GLuint _vao;
		std::vector<Vbo> _vbos;
		GLenum _mode;
		GLsizei _vertexCount;
	};
}