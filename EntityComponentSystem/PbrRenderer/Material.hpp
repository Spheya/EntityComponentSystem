#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Texture.hpp"

namespace renderer {
	class MaterialTexture {
	public:
		MaterialTexture(std::shared_ptr<Texture> texture);
		MaterialTexture(float value);
		MaterialTexture(const glm::vec3& value);

		const std::shared_ptr<Texture>& getTexture() const;

	private:
		std::shared_ptr<Texture> _texture;
	};
	
	struct Material {
		Material(
			MaterialTexture baseColour,
			MaterialTexture metalness,
			MaterialTexture roughness,
			MaterialTexture normal
		) :
			baseColour(baseColour),
			metalness(metalness),
			roughness(roughness),
			normal(normal)
		{}

		MaterialTexture baseColour;
		MaterialTexture metalness;
		MaterialTexture roughness;
		MaterialTexture normal;
	};
}

