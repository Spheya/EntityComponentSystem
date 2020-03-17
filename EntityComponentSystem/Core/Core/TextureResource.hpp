#pragma once

#include <memory>

#include "Texture.hpp"
#include "Resource.hpp"

namespace renderer {
	class TextureResource : public Resource {
	public:
		TextureResource(std::string path) : Resource(path) {}

		void load() override;

		renderer::Texture* getTexture();
		const renderer::Texture* getTexture() const;

	private:
		std::unique_ptr<renderer::Texture> _texture;

	};
}
