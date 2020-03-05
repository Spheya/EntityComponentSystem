#pragma once

#include <GL/glew.h>

#include <stdint.h>

namespace renderer {
	class Vbo {

	public:
		Vbo();
		Vbo(const Vbo&) = delete;
		Vbo& operator=(const Vbo&) = delete;
		Vbo(Vbo&& other) noexcept;
		Vbo& operator=(Vbo&& other) noexcept;
		~Vbo();

		void write(size_t length, const void* data);
		
		void bindAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, GLsizei first = 0);

	private:
		GLuint _id;
		size_t _capacaty = 0;

	};
}