#pragma once

#include <memory>

#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "Material.hpp"
#include "InstanceData.hpp"

namespace renderer {
	struct ModelRenderComponent {
		ModelRenderComponent(const Transform& transform, std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Material> material) :
			transform(transform), model(std::move(model)), shader(std::move(shader)), material(std::move(material))
		{}

		Transform transform;
		InstanceData instanceData;

		std::shared_ptr<Model> model;
		std::shared_ptr<ShaderProgram> shader;
		std::shared_ptr<Material> material;
	};

}
