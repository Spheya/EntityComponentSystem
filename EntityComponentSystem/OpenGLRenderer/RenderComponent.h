#pragma once
#include <memory>

#include "Model.h"
#include "TexturePack.h"
#include "ShaderProgram.h"
#include "UniformPack.h"

namespace renderer {
	struct RenderComponent {
		RenderComponent(std::shared_ptr<const Model> model, std::shared_ptr<const TexturePack> textures, std::shared_ptr<const ShaderProgram> shader, UniformPack uniformPack) :
			model(std::move(model)),
			textures(std::move(textures)),
			shader(std::move(shader)),
			uniformPack(std::move(uniformPack))
		{}

		std::shared_ptr<const Model> model;
		std::shared_ptr<const TexturePack> textures;
		std::shared_ptr<const ShaderProgram> shader;
		
		UniformPack uniformPack;
	};
}