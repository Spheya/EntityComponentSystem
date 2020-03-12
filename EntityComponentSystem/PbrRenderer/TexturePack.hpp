#pragma once
#include <vector>
#include "Texture.hpp"
#include <memory>

namespace renderer {
	// Not used anymore, but might come in handy later on
	class TexturePack {
	public:
		TexturePack() = default;
		TexturePack(std::initializer_list<std::shared_ptr<const Texture>> textures);

		GLint add(std::shared_ptr<const Texture> texture);

		void bind() const;
		void unbind() const;

	private:
		std::vector<std::shared_ptr<const Texture>> _textures;

	};
}
