#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>

#include "Texture.hpp"

namespace renderer {
	class Fbo {
	public:
		Fbo(int width, int height);
		~Fbo();

		void bind();
		static void unbind();

		size_t createTexture();

		Texture* getDepthBuffer();
		Texture* getTexture(size_t handle);

	private:
		int _width, _height;

		GLuint _id;

		std::unique_ptr<Texture> _depthBuffer;
		std::vector<std::unique_ptr<Texture>> _textures;

		std::unique_ptr<Texture> generateTexture(GLenum type, GLenum format);
	};
}

