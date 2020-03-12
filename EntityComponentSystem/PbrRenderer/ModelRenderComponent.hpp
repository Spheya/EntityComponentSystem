#pragma once

#include <memory>

#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "Material.hpp"
#include "InstanceData.hpp"

namespace renderer {
	struct ModelRenderComponent {
		ModelRenderComponent(const Transform& transform, std::shared_ptr<Model> model, std::shared_ptr<Material> material) :
			transform(transform), model(std::move(model)), material(std::move(material))
		{}

		Transform transform;

		std::shared_ptr<Model> model;
		std::shared_ptr<Material> material;
	};

}
