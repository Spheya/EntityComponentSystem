#pragma once

#include <memory>

#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "Material.hpp"
#include "InstanceData.hpp"

namespace renderer {
	struct ModelRenderComponent {
		ModelRenderComponent(const Transform& transform, Model* model, Material* material) :
			transform(transform), model(model), material(material)
		{}

		Transform transform;

		Model* model;
		Material* material;
	};

}
