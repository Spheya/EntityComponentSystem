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

		void bind();

		void draw();

	private:
		GLuint _vao;
		std::vector<Vbo> _vbos;
		GLenum _mode;
		GLsizei _vertexCount;
	};
}