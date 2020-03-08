#pragma once

#include <memory>

#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "TexturePack.hpp"
#include "InstanceData.hpp"

namespace renderer {
	struct ModelRenderComponent {
		ModelRenderComponent(const Transform& transform, std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shader, std::shared_ptr<TexturePack> textures = nullptr) :
			transform(transform), model(std::move(model)), shader(std::move(shader)), textures(std::move(textures))
		{}

		Transform transform;
		InstanceData instanceData;

		std::shared_ptr<Model> model;
		std::shared_ptr<ShaderProgram> shader;
		std::shared_ptr<TexturePack> textures;
	};

}
