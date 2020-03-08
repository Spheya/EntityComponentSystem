#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Transform.hpp>

#include "Vao.hpp"
#include "InstanceData.hpp"

namespace renderer {
	class Model {
		friend class ModelRenderSystem;

	public:
		Model(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords = {}, const std::vector<glm::vec3>& normals = {});

		InstanceData& getInstanceData();
		const InstanceData& getInstanceData() const;

		void setTransform(const Transform& transform);
		Transform& getTransform();
		const Transform& getTransform() const;

	private:
		void updateInstanceData();
		Vao& getVao();

		InstanceData _instanceData;
		Transform _transform;
		Vao _vao;
	};
}

