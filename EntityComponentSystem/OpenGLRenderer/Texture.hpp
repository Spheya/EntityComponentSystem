#pragma once
#include <string>

#include <GL/glew.h>

// TODO: Allow editing the texture

namespace renderer {
	class Texture {
	public:
		Texture() = default;
		explicit Texture(const std::string& fileName);
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
		~Texture();

		void loadFromFile(const std::string& fileName, GLenum filter = GL_LINEAR);
		void loadFromMemory(const unsigned char* data, int channels, int width, int height, GLenum filter = GL_LINEAR);
		void loadFromMemory(const unsigned char* data, int channels, int width, int height, int depth, GLenum filter = GL_LINEAR);

		void bind(unsigned slot) const;
		void unbind(unsigned slot) const;

		[[nodiscard]] unsigned getWidth() const;
		[[nodiscard]] unsigned getHeight() const;

		[[nodiscard]] GLenum getType() const;

		[[nodiscard]] bool isValid() const;

	private:
		GLuint _textureId = 0;
		GLenum _type;

		int _width;
		int _height;
		int _channels;
	};
}
