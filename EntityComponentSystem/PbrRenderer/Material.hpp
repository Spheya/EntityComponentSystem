#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Core/Texture.hpp>

namespace renderer {
	class MaterialTexture {
	public:
		MaterialTexture(const Texture* texture);
		MaterialTexture(float value);
		MaterialTexture(const glm::vec3& value);

		MaterialTexture(const MaterialTexture&) = delete;
		MaterialTexture& operator=(const MaterialTexture&) = delete;
		MaterialTexture(MaterialTexture&& other);
		MaterialTexture& operator=(MaterialTexture&& other);

		~MaterialTexture();

		const Texture* getTexture() const;

	private:
		const Texture* _texture;
		bool _textureOwner;
	};
	
	struct Material {
		Material(
			MaterialTexture baseColour,
			MaterialTexture metalness,
			MaterialTexture roughness,
			MaterialTexture normal
		) :
			baseColour(std::move(baseColour)),
			metalness(std::move(metalness)),
			roughness(std::move(roughness)),
			normal(std::move(normal))
		{}

		MaterialTexture baseColour;
		MaterialTexture metalness;
		MaterialTexture roughness;
		MaterialTexture normal;
	};
}

