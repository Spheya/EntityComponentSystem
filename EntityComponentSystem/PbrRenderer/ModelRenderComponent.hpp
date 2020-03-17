#pragma once

#include <memory>

#include "Model.hpp"
#include <Core/ShaderProgram.hpp>
#include "Material.hpp"
#include "InstanceData.hpp"

namespace renderer {
	struct ModelRenderComponent {
		ModelRenderComponent(const Transform& transform, const Model* model, Material* material) :
			transform(transform), model(model), material(material)
		{}

		Transform transform;

		const Model* model;
		Material* material;
	};

}
