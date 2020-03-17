#pragma once
#include <string>

#include <GL/glew.h>

// TODO: Allow editing the texture

namespace renderer {
	class Texture {
		friend class Fbo;

	public:
		Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
		~Texture();

		void loadFromFile(const std::string& fileName);
		void loadFromMemory(const unsigned char* data, int channels, int width, int height);
		void loadFromMemory(const unsigned char* data, int channels, int width, int height, int depth);

		void setFilter(GLenum filter);

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
