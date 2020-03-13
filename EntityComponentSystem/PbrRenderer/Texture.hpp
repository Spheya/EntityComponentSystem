#pragma once
#include <string>

#include <GL/glew.h>

#include <Resource.hpp>

namespace renderer {
	class Texture : public Resource{
	public:
		explicit Texture(const std::string& path = "") : Resource(path) {}
		~Texture();

		void load();
		void loadFromMemory(const unsigned char* data, int channels, int width, int height, GLenum filter);
		void loadFromMemory(const unsigned char* data, int channels, int width, int height, int depth, GLenum filter);

		void setFilter(GLenum filter);

		void bind(unsigned slot) const;

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
